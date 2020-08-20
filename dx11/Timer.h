#pragma once
#include <string>
#include <iostream>
#include <chrono>

namespace GTimer
{
	using namespace std;
	using namespace chrono;

	enum class Duration
	{
		SECONDS,
		MILLISECONDS,
		MICROSECONDS
	};

	// Non-RAII Timer
	class Timer
	{
	public:
		Timer();
		~Timer();

		void Start();
		void Stop();

		// Print duration in milliseconds and seconds
		void Print(std::string headerMsg);

		// Get duration: MILLISECONDS, SECONDS
		long double GetTime(Duration duration);


	private:
		time_point<high_resolution_clock> m_start, m_end;
		microseconds m_duration;


	};

}


