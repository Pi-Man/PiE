#include "UpdateLimiter.h"

void UpdateLimiter::start() {
	_nextFrameTime = SDL_GetTicks();
}

double UpdateLimiter::push() {

	Uint32 start = SDL_GetTicks();

	counter.pushFrame();

	if (exact) {
		while (SDL_GetTicks() < (Uint32)_nextFrameTime);
	}
	else {
		long int time = _nextFrameTime - SDL_GetTicks();
		if (time > 0) {
			Sleep(time);
		}
	}

	_nextFrameTime += frameTime;

	_ups = counter.getFPS();

	lastFrameTime = SDL_GetTicks() - start;

	return _ups;
}

std::pair<double, bool> UpdateLimiter::poll() {
	std::pair<double, bool> pair{};
	pair.second = SDL_GetTicks() >= (Uint32)_nextFrameTime;
	pair.first = counter.getFPS();
	if (pair.second) {
		counter.pushFrame();
		lastFrameTime = frameTime;
		_ups = counter.getFPS();
		_nextFrameTime += frameTime;
	}
	return pair;
}

void UpdateLimiter::updateUPS(int ups) {
	this->ups = ups;
	frameTime = 1000.0 / ups;
}

double UpdateLimiter::getUPS() {
	return _ups;
}

long UpdateLimiter::getStaticUPS() {
	return ups;
}

double UpdateLimiter::getLastFrameTime() {
	return lastFrameTime;
}

std::pair<double, double> UpdateLimiter::getLastNextTimes() {
	return std::pair<double, double>{_nextFrameTime - lastFrameTime, _nextFrameTime};
}

bool UpdateLimiter::getExact() {
	return exact;
}

void UpdateLimiter::setExact(bool exact) {
	this->exact = exact;
}
