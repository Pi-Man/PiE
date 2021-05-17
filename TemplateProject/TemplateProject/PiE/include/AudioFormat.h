#ifndef _AUDIO_FORMAT
#define _AUDIO_FORMAT

#include "portaudio.h"

// struct to hold audio format data
// * WARNING: will be switching from portaudio to OpenAL soon
typedef struct AudioFormat {
	int sampleRate;              /**< DSP frequency -- samples per second */
	PaSampleFormat format;       /**< Audio data format */
	unsigned char channels;      /**< Number of channels: 1 mono, 2 stereo... */
	unsigned char bitsPerSample;
	unsigned short samples;      /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
	//unsigned short padding;      /**< Necessary for some compile environments */
	PaStreamCallback *callback;  /**< Callback that feeds the audio device (NULL for default callback) */
	void *userdata;              /**< Userdata passed to callback (NULLable) */
} AudioFormat;



#endif