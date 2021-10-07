/******************************************************************************/
/*!
\file   Timer.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts definition for Timer

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Timer.h"

Timer::Timer()
	:tEnd{ 0 }, tStart{ 0 }, perfCounter{ 0 },
	dt{ 0 }, totalTime{ 0 }
{
	LARGE_INTEGER perfCounterFrequency;							// For storing Performance Counter Frequency
	QueryPerformanceFrequency(&perfCounterFrequency);			// Retrieve Performance Counter Frequency
	perfCounter = perfCounterFrequency.QuadPart;				// Store as an int_64(long long)
	QueryPerformanceCounter(&tStart);							// Store the start of frame performance counter
}

void Timer::FrameStart()
{
	QueryPerformanceCounter(&tEnd);								// Store the end of frame performance counter
	tStart.QuadPart = tEnd.QuadPart;							// Set starting performance counter to new performance counter
}
void Timer::FrameEnd()
{
	do															// Frame Rate Controller (Currently at 60FPS cap)
	{
		QueryPerformanceCounter(&tEnd);
		dt = (double)((tEnd.QuadPart - tStart.QuadPart)  / (double)perfCounter);
	} while (dt  < FRAMES_CAP);
	//Debug  printouts
//	std::cout << dt <<"ms" << std::endl;
//	std::cout << (int)(dt * 60 * 60 / 1000) << "FPS" << std::endl;
//	std::cout << totalTime/1000 << std::endl; 
	totalTime += dt;											// Increment total game time
}

double Timer::getTotalTime() const								// For retrieving total game time
{
	return totalTime;
}

double Timer::getDeltaTime() const								// For retrieving delta time
{
	return dt;
}

int Timer::getFPS() const
{
	return static_cast<int>(dt * FRAMES_PER_SECOND*60.f);
}