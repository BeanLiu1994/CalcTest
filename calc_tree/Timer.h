#pragma once
#include <iostream>
#include <chrono>

template<typename Display_ty = std::chrono::microseconds>
class Timer
{
public:
	Timer(bool startNow = true)
	{
		if (startNow) StartTimer();
	}
	inline void StartTimer()
	{
		start = std::chrono::steady_clock::now();
	}
	inline long long EndTimer(const char _printThis[])
	{
		Display_ty duration = std::chrono::duration_cast<Display_ty>(std::chrono::steady_clock::now() - start);
		if (_printThis)
			std::cout << _printThis << " elapsed time:  " << duration.count() << std::endl;
		return duration.count();
	}
private:
	std::chrono::steady_clock::time_point start;
};
