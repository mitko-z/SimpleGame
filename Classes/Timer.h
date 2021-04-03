#pragma once
#include <ctime>
#include <chrono>

class Timer
{
public:
	void start();

	bool isStarted();

	void stop();

	void restart();

	double elapsedMilliseconds();

	double elapsedSeconds();

private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	bool m_bRunning = false;
};