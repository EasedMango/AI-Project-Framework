#pragma once
#include <SDL.h>

class Timer
{
private:
	unsigned int prevTicks;
	unsigned int currentTicks;
	static unsigned int singleEventID;
	static Uint32 callBackFuncion(Uint32 interval, void* singleEventParam);

public:
	Timer();
	~Timer();

	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int fps_) const;
	float GetCurrentTicks() const;
};
