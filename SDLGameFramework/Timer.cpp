#include <typeinfo>
#include <iostream>
#include "Timer.h"

#include <SDL_timer.h>

Timer::Timer() : prevTicks(0), currentTicks(0) {}

Timer::~Timer() {}

void Timer::Start() {
	prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
}

void Timer::UpdateFrameTicks() {
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const {
	float deltaTime = static_cast<float>(currentTicks - prevTicks) / 1000.0f;
	const float maxDeltaTime = 0.05f; // Maximum delta time of 50 milliseconds (1/20th of a second)
	return (deltaTime > maxDeltaTime) ? maxDeltaTime : deltaTime;
}

float Timer::GetCurrentFPS() const
{
	float deltaTime = GetDeltaTime(); // Get the delta time in seconds
	if (deltaTime > 0) {
		return 1.0f / deltaTime; // FPS is the reciprocal of the delta time
	}
	return 0.0f; // To handle the case when deltaTime is zero to avoid division by zero
}

unsigned int Timer::GetSleepTime(const unsigned int fps_) const {
	const unsigned int milliSecsPerFrame = 1000 / fps_;
	const unsigned int frameTime = SDL_GetTicks() - currentTicks;
	if (frameTime >= milliSecsPerFrame) {
		return 0;
	}
	return milliSecsPerFrame - frameTime;
}

float Timer::GetCurrentTicks() const {
	return static_cast<float>(currentTicks) / 1000.0f;
}




