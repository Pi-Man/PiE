#ifndef _MICROPHONE
#define _MICROPHONE

#include <stdio.h>
#include <memory>

#include "portaudio.h"
#include "AudioFormat.h"

// * WARNING: will be switching from portaudio to OpenAL soon
namespace Microphone {
	// the callback to use for collecting audio data
	// * WARNING: will be switching from portaudio to OpenAL soon
	int callback(
		const void *input, void *output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	// the size of the microphone buffer
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern long _bufferSize;
	// the current position in the buffer
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern long _bufferPos;
	// the PA stream
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern PaStream *stream;
	// the format of the audio stream
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern AudioFormat _format;
	// the microphone data buffer
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern unsigned char *_buffer;

	// if the PA stream is open
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern bool open;

	// initialize the microphone
	// * WARNING: will be switching from portaudio to OpenAL soon
	void init(size_t bufferSize, AudioFormat &format, char *deviceName = nullptr);
	// close the microphone, can be re-initialized after closing
	// * WARNING: will be switching from portaudio to OpenAL soon
	void stop();

	// pause the microphone, this is the default state when initialized
	// * WARNING: will be switching from portaudio to OpenAL soon
	void pause();
	// start or resume the microphone, must be started after initializing
	// * WARNING: will be switching from portaudio to OpenAL soon
	void play();

	// set the pause state of the microphone
	// * WARNING: will be switching from portaudio to OpenAL soon
	void setPause(bool pause);
}

#endif