#ifndef _AUDIO_FORMAT_H
#define _AUDIO_FORMAT_H

#include "portaudio.h"

namespace PiE {

// struct to hold audio format data
	typedef struct AudioFormat {
		double sampleRate;              /**< DSP frequency -- samples per second */
		PaSampleFormat format;       /**< Audio data format */
		unsigned char channels;      /**< Number of channels: 1 mono, 2 stereo... */
		unsigned char bitsPerSample;
		unsigned short samples;      /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
		//unsigned short padding;      /**< Necessary for some compile environments */
		PaStreamCallback * callback;  /**< Callback that feeds the audio device (NULL for default callback) */
		void * userdata;              /**< Userdata passed to callback (NULLable) */
	} AudioFormat;
}


#endif