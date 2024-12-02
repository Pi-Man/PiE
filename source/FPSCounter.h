#ifndef _FPS_COUNTER_H
#define _FPS_COUNTER_H

#include<list>
#include<utility>
#include<mutex>

#include<SDL.h>

namespace PiE {

// class for counting the tick rate of an update loop
	struct FPSCounter {
		std::list<std::pair<long, long>> times_;
		long lifetime_;
		long lastpush_ = 0;

		std::mutex mutex;

		bool hasExpired(const std::pair<long, long> & entry);

		// creat a counter with the specified average smoothing time
		FPSCounter(long timeLength = 1000);

		// call at the start of every frame to count
		void pushFrame();

		// the the current frame rate
		float getFPS();

	};

}

#endif