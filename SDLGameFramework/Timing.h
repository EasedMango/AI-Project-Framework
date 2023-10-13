#pragma once
#include <chrono>
#include <iostream>
#include <queue>

class Timing
{
private:
	const char* blockName;
	std::chrono::high_resolution_clock::time_point start;
	bool ending;
	inline static long long totalDuration = 0;
	inline static long long count = 0;
	void PrintNearestTimeScale(const long long time, const char* blockName) const
	{
		if (time < 1000)
		{
			std::cout << blockName << " Timing " << time << " microseconds" << std::endl;
		}
		else if (time < 1000000)
		{
			std::cout << blockName << " Timing " << time / 1000.f << " milliseconds" << std::endl;
		}
		else if (time < 60000000)
		{
			std::cout << blockName << " Timing " << time / 1000000.f << " seconds" << std::endl;
		}
		else if (time < 3600000000)
		{
			std::cout << blockName << " Timing " << time / 60000000.f << " minutes" << std::endl;
		}
		else
		{
			std::cout << blockName << " Timing " << time / 3600000000.f << " hours" << std::endl;
		}
	};


public:
	static void AddDuration(long long duration) {
		totalDuration += duration;
		++count;
	}

	static long long GetAverage() {
		if (count == 0) {
			return 0; // To avoid division by zero
		}
		return totalDuration / count;
	}

	explicit Timing(const char* block_name)
		: blockName(block_name), ending(false)
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~Timing()
	{
		if (!ending)
		{
			EndTimer();
		}
	}

	void EndTimer()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

		const long long duration = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
		PrintNearestTimeScale(duration, blockName);
		AddDuration(duration);
		ending = true;
	}




};
#define TIMING(name) Timing timing(name)