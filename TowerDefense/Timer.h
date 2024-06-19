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

private:

	std::chrono::time_point<clockType> start_point, last_point;

};

#endif // !TIMER_H