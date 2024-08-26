#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

using clockType = std::chrono::high_resolution_clock;

class Timer
{
public:

	Timer() : start_point(clockType::now()), last_point(start_point) {}

	void ResetLastPoint();

	int SecondsFromStart() const;
	int SecondsFromLast() const;

	int MinutesFromStart() const;
	int MinutesFromLast() const;

	void StartTimer();
	void StopTimer();
	void ResetTimer();
	int GetActualTime();

private:

	std::chrono::time_point<clockType> start_point, last_point;
	int activeSeconds = 0, lastSeconds = 0;
	bool active = false;

};

#endif // !TIMER_H