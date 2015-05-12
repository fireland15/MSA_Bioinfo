/*----------------------------------------------------------------------------------------*
/*	FI_Timer.h
/*----------------------------------------------------------------------------------------*
/*	Forrest Ireland
/*	April 13, 2015
/*----------------------------------------------------------------------------------------*
/*	Header file for Timer class. Provides basic time keeping for windows machines
/*----------------------------------------------------------------------------------------*/

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace fli {
	class Timer {
	public:
		Timer();
		~Timer();

		void Start();
		void Stop();
		double GetElapsedTimeMicroSec();
		double GetElapsedTimeMilliSec();
		double GetElapsedTimeSec();

	private:
		double startTime;
		double endTime;
		bool stopped;

		LARGE_INTEGER frequency;
		LARGE_INTEGER startCount;
		LARGE_INTEGER endCount;
	};
}