#ifndef _SPEAKER
#define _SPEAKER

#include <stdio.h>
#include <memory>

#include "portaudio.h"
#include "AudioFormat.h"

// * WARNING: will be switching from portaudio to OpenAL soon
namespace Speaker {
	// the callback to use for collecting audio data
	// * WARNING: will be switching from portaudio to OpenAL soon
	int callback(
		const void *input, void *output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	// the size of the speaker buffer
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern size_t _bufferSize;
	// the current position in the buffer
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern size_t _bufferPos;
	// the PA stream
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern PaStream *stream;
	// the format of the audio stream
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern AudioFormat _format;
	// the speaker data buffer
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern unsigned char *_buffer;
	// speeds up or slowsdown the sample rate of the speaker
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern double timeWarp;

	// if the PA stream is open
	// * WARNING: will be switching from portaudio to OpenAL soon
	extern bool open;

	// initialize the speaker
	// * WARNING: will be switching from portaudio to OpenAL soon
	void init(size_t bufferSize, AudioFormat format, const char *deviceName = nullptr);
	
	// close the speaker, can be re-initialized after closing
	// * WARNING: will be switching from portaudio to OpenAL soon
	void stop();

	// pause the speaker, this is the default state when initialized
	// * WARNING: will be switching from portaudio to OpenAL soon
	void pause();
	// start or resume the speaker, must be started after initializing
	// * WARNING: will be switching from portaudio to OpenAL soon
	void play();

	// set the pause state of the speaker
	// * WARNING: will be switching from portaudio to OpenAL soon
	void setPause(bool pause);
}

#endif