#include "Speaker.h"


extern size_t Speaker::_bufferSize = 0;
extern size_t Speaker::_bufferPos = 0;
extern PaStream *Speaker::stream = nullptr;
extern AudioFormat Speaker::_format{};
extern unsigned char *Speaker::_buffer = nullptr;
extern double Speaker::timeWarp = 1.0;

extern bool Speaker::open = false;

// TODO: switch to OpenAL

int Speaker::callback(
	const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData) {

	size_t byteCount = frameCount * _format.channels * _format.bitsPerSample;

	if (byteCount * timeWarp > _bufferSize) return -1;
	if (byteCount * timeWarp + _bufferPos > _bufferSize) {
		_bufferPos = 0;
	}
	if (timeWarp == 1.0) {
		memcpy(output, _buffer + _bufferPos, byteCount);
	}
	else {
		for (unsigned long i = 0; i < frameCount; i++) {
			for (int c = 0; c < _format.channels; c++) {
				double j = _bufferPos / _format.bitsPerSample + i * _format.channels * timeWarp + c;
				double p = j - (size_t)j;
				double v;
				switch (_format.bitsPerSample) {
				case 1:
					v = _buffer[(size_t)j] * (1 - p) + _buffer[(size_t)j + 1] * p;
					((char*)output)[i * _format.channels + c] = (char)(v + 0.5);
					break;
				case 2:
					v = ((short*)_buffer)[(size_t)j] * (1 - p) + ((short*)_buffer)[(size_t)j + 1] * p;
					((short*)output)[i * _format.channels + c] = (short)(v + 0.5);
					break;
				case 3:
				case 4:
					v = ((int*)_buffer)[(size_t)j] * (1 - p) + ((int*)_buffer)[(size_t)j + 1] * p;
					((int*)output)[i * _format.channels + c] = (int)(v + 0.5);
					break;
				}
			}
		}
	}
	_bufferPos += (size_t) (byteCount * timeWarp);
	return 0;
}

void Speaker::init(size_t bufferSize, AudioFormat format, const char *deviceName) {

	if (open) {
		puts("Warning, Speaker is Already Open!");
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
	param.sampleFormat = format.bitsPerSample == 1 ? paInt8 : format.bitsPerSample == 2 ? paInt16 : paInt32;
	param.suggestedLatency = 0;

	printf("device count %d\n", Pa_GetDeviceCount());
	for (int i = 4; i < Pa_GetDeviceCount(); i++) {
		const PaDeviceInfo * info = Pa_GetDeviceInfo(i);
		if (info) {
			printf("%d: %s, I%d, O%d\n", i, info->name, info->maxInputChannels, info->maxOutputChannels);
			if (info->maxOutputChannels >= param.channelCount) {
				param.device = i;
				format.sampleRate = info->defaultSampleRate;
				if (Pa_OpenStream(&stream, nullptr, &param, format.sampleRate, format.samples, 0, format.callback, format.userdata)) {
					puts("Could Not Open Player Device");
				}
				else {
					const PaStreamInfo *info = Pa_GetStreamInfo(stream);
					printf("Opened Device: %lfHz\n", info->sampleRate);
					break;
				}
			}
		}
		else {
			puts("info NULL");
		}
	}
	//Pa_OpenDefaultStream(&stream, 0, format.channels, format.format, format.sampleRate, format.samples, callback, nullptr);
	_format = format;
	_buffer = (unsigned char*)malloc(_bufferSize);
}

void Speaker::stop() {
	if (!open) return;
	open = false;
	free(_buffer);
	_bufferPos = 0;
	_bufferSize = 0;
	_buffer = nullptr;
	Pa_CloseStream(stream);
	stream = nullptr;
}

void Speaker::pause() {
	Pa_AbortStream(stream);
}

void Speaker::play() {
	if (Pa_StartStream(stream)) {
		printf("Could not start stream\n");
	}
}

void Speaker::setPause(bool pause) {
	pause ? Speaker::pause() : Speaker::play();
}
