/*----------------------------------------------------------------------------------------*
/*	FI_Timer.cpp
/*----------------------------------------------------------------------------------------*
/*	Forrest Ireland
/*	April 13, 2015
/*----------------------------------------------------------------------------------------*
/*	Source file for Timer class. Provides basic time keeping for windows machines
/*----------------------------------------------------------------------------------------*/

#include "Timer.h"

namespace fli{
	Timer::Timer() {
		QueryPerformanceFrequency(&frequency);
		startCount.QuadPart = 0;
		endCount.QuadPart = 0;

		stopped = false;
		startTime = 0;
		endTime = 0;
	}

	Timer::~Timer() {

	}

	void Timer::Start() {
		stopped = false;
		QueryPerformanceCounter(&startCount);
	}

	void Timer::Stop() {
		stopped = true;
		QueryPerformanceCounter(&endCount);
	}

	double Timer::GetElapsedTimeMicroSec() {
		if (!stopped) {
			QueryPerformanceCounter(&endCount);
		}

		startTime = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
		endTime = endCount.QuadPart * (1000000.0 / frequency.QuadPart);

		return endTime - startTime;
	}

	double Timer::GetElapsedTimeMilliSec() {
		return this->GetElapsedTimeMicroSec() * 0.001;
	}

	double Timer::GetElapsedTimeSec() {
		return this->GetElapsedTimeMicroSec() * 0.000001;
	}
}