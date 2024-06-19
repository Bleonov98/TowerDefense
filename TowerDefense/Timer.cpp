#include "Timer.h"

void Timer::ResetLastPoint()
{
	last_point = clockType::now();
}

int Timer::SecondsFromStart() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(clockType::now() - start_point).count();
}

int Timer::SecondsFromLast() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(clockType::now() - last_point).count();
}

int Timer::MinutesFromStart() const
{
	return std::chrono::duration_cast<std::chrono::minutes>(clockType::now() - start_point).count();
}

int Timer::MinutesFromLast() const
{
	return std::chrono::duration_cast<std::chrono::minutes>(clockType::now() - last_point).count();
}
