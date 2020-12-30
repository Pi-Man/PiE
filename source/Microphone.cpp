#include "Microphone.h"



extern long Microphone::_bufferSize = 0;
extern long Microphone::_bufferPos = 0;
extern PaStream *Microphone::stream = nullptr;
extern AudioFormat Microphone::_format{};
extern unsigned char *Microphone::_buffer = nullptr;

extern bool Microphone::open = false;

// TODO: switch to OpenAL

int Microphone::callback(const void * input, void * output, unsigned long frameCount, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData) {

	size_t byteCount = frameCount * _format.channels * _format.bitsPerSample;

	if (byteCount > _bufferSize) return -1;
	if (byteCount + _bufferPos > _bufferSize) {
		_bufferPos = 0;
	}

	memcpy(_buffer + _bufferPos, input, byteCount);
	_bufferPos += byteCount;

	return 0;
}

void Microphone::init(size_t bufferSize, AudioFormat &format, char *deviceName) {

	if (open) {
		puts("Warning, Microphone is Already Open!");
	}

	open = true;

	if (!format.callback) {
		format.callback = callback;
	}

	_bufferSize = bufferSize * format.channels * format.bitsPerSample;
	PaStreamParameters param;
	param.channelCount = format.channels;
	param.device = 1;
	param.hostApiSpecificStreamInfo = nullptr;
	param.sampleFormat = paInt8;
	param.suggestedLatency = 0;

	printf("device count %d\n", Pa_GetDeviceCount());
	for (int i = 2; i < Pa_GetDeviceCount(); i++) {
		const PaDeviceInfo * info = Pa_GetDeviceInfo(i);
		if (info) {
			printf("%d: %s, I%d, O%d\n", i, info->name, info->maxInputChannels, info->maxOutputChannels);
			if (info->maxInputChannels >= param.channelCount) {
				param.device = i;
				format.sampleRate = info->defaultSampleRate;
				if (Pa_OpenStream(&stream, &param, nullptr, format.sampleRate, format.samples, 0, format.callback, format.userdata)) {
					puts("Could Not Open Recording Device");
				}
				else {
					const PaStreamInfo *info = Pa_GetStreamInfo(stream);
					printf("%lf\n", info->sampleRate);
					break;
				}
			}
		}
		else {
			puts("info NULL");
		}
	}
	//if (Pa_OpenDefaultStream(&stream, format.channels, 0, format.format, format.sampleRate, format.samples, format.callback, nullptr)) {
	//	puts("Could Not Open Recording Device");
	//}
	_format = format;
	_buffer = (unsigned char*)malloc(_bufferSize);
}

void Microphone::stop() {
	if (!open) return;
	open = false;
	free(_buffer);
	_bufferPos = 0;
	_bufferSize = 0;
	_buffer = nullptr;
	Pa_CloseStream(stream);
	stream = nullptr;
}

void Microphone::pause() {
	Pa_StopStream(stream);
}

void Microphone::play() {
	Pa_StartStream(stream);
}

void Microphone::setPause(bool pause) {
	pause ? Microphone::pause() : Microphone::play();
}
