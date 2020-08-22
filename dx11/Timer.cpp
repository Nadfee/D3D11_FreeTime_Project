#include "Timer.h"

using namespace GTimer;

Timer::Timer() :
	m_start(high_resolution_clock::now()),
	m_end(high_resolution_clock::now()),
	m_duration(0)
{
	
}

Timer::~Timer()
{

}

void Timer::Start()
{
	m_start = high_resolution_clock::now();
}

void Timer::Stop()
{
	m_end = high_resolution_clock::now();
	m_duration = duration_cast<microseconds>(m_end - m_start);
}

void GTimer::Timer::Print(std::string headerMsg)
{
	// Printing ms with IE-03 to keep the decimals from microseconds
	// Printing s with a duration cast first to remove three decimals (only purpose for s is to have a reference point to a second)
	auto durationInMs = m_duration.count() * 1E-03;
	auto durationInS = duration_cast<milliseconds>(m_duration).count() * 1E-03;

	cout << "\n" << "--- " << headerMsg << " ---" << endl
		 << "Elapsed: " << durationInMs << "ms " 
		 << "(" << durationInS * 1E-03 << "s)." << endl;
}

long double GTimer::Timer::GetDeltaTime(Duration duration)
{
	switch (duration)
	{
	case Duration::MILLISECONDS:
		return m_duration.count() * 1E-03;	// Milliseconds with decimal
		break;
	case Duration::SECONDS:
		return m_duration.count() * 1E-06;	// Seconds with decimal
		break;
	case Duration::MICROSECONDS:
		return m_duration.count();
		break;

	// Microseconds default
	default:
		return m_duration.count();
	}
}
