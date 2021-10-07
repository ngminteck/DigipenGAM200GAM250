/******************************************************************************/
/*!
\file   Timer.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Timer

All content � 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include <Windows.h>
#include <iostream>

const double FRAMES_PER_SECOND = 60;				// Frame Rate
const double FRAMES_CAP = 1 / FRAMES_PER_SECOND;	// Frame Rate Cap

class Timer
{
private:
	LARGE_INTEGER tStart;							// Stores performance counter at start of frame
	LARGE_INTEGER tEnd;								// Stores performance counter at end of frame
	int64_t perfCounter;							// Stores performance counter frequency
	double dt;										// Delta Time
	double totalTime;								// Total game time

public:
	Timer();										// Constructor

	void FrameStart();								// Start of updating/drawing frame
	void FrameEnd();								// End of updating/drawing frame

	double getTotalTime() const;					// in seconds
	double getDeltaTime() const;					// in seconds
	int	   getFPS() const;
};