#ifndef _UPDATE_LIMITER_H
#define _UPDATE_LIMITER_H

#include "FPSCounter.h"

namespace PiE {

// limits the update rate of an update loop
	class UpdateLimiter {

		double _nextFrameTime = 0;
		double _ups = 0;
		long ups = 0;

		bool exact = false;

		FPSCounter counter;

		double frameTime = 0;
		double lastFrameTime = 0;

	public:

		// creates an update limiter with the specified update rate and whether or not to use a wait loop or sleep
		UpdateLimiter(int ups, bool exactTime) : ups(ups), frameTime(1000.0 / ups), exact(exactTime) {}
		// creates an update limiter with the specified update rate and whether or not to use a wait loop or sleep
		UpdateLimiter(int ups, bool exactTime, long averagingTime) : ups(ups), frameTime(1000.0 / ups), exact(exactTime), counter(averagingTime) {}

		// start the update limiter, call this right before starting the loop
		void start();

		// call this at the start of the loop, will block to keep the update rate at the specified value
		double push();
		std::pair<double, bool> poll();

		// change the specified update rate
		void updateUPS(int ups);

		// get the current measured update rate
		double getUPS();
		// get the specified update rate
		long getStaticUPS();

		// get the time spent on the last update
		double getLastFrameTime();
		// get the start time from the last update and the anticipated start for the next frame
		std::pair<double, double> getLastNextTimes();

		bool getExact();
		void setExact(bool exact);

	};

}

#endif