#include "FPSCounter.h"

namespace PiE {

	bool FPSCounter::hasExpired(const std::pair<long, long> & entry) {
		return entry.first < lastpush_ - lifetime_;
	}

	FPSCounter::FPSCounter(long lifeTime) : lifetime_(lifeTime) {}

	void FPSCounter::pushFrame() {

		std::lock_guard<std::mutex> lock(mutex);

		long thispush_ = SDL_GetTicks();

		times_.push_front(std::pair<long, long>(thispush_, thispush_ - lastpush_));

		lastpush_ = thispush_;

		times_.remove_if([this](const std::pair<long, long> & entry) { return this->hasExpired(entry); });
	}

	float FPSCounter::getFPS() {

		std::lock_guard<std::mutex> lock(mutex);

		long times = 0;
		int count = 0;

		for (std::pair<long, long> entry : times_) {

			times += entry.second;
			count += 1;

		}

		return 1000.0f * (float)count / (float)times;
	}

}