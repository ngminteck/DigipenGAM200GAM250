/******************************************************************************/
/*!
\file   FrameController.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts definition for FrameController

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "FrameController.h"

FrameController::FrameController()
	:tEnd{ 0 }, tStart{ 0 }, perfCounter{ 0 },
	dt{ 0 }, totalTime{ 0 }, fixedDeltaTIme{1.0/60.0},numberOfSteps {0},accumTime{0.0}
{
	LARGE_INTEGER perfCounterFrequency;							// For storing Performance Counter Frequency
	QueryPerformanceFrequency(&perfCounterFrequency);			// Retrieve Performance Counter Frequency
	perfCounter = perfCounterFrequency.QuadPart;				// Store as an int_64(long long)
	QueryPerformanceCounter(&tStart);							// Store the start of frame performance counter
}

void FrameController::FrameStart()
{
	numberOfSteps = 0;
	QueryPerformanceCounter(&tStart);								// Store the end of frame performance counter
	while (accumTime >= fixedDeltaTIme)
	{
		accumTime -= fixedDeltaTIme;
		++numberOfSteps;
	}
}


void FrameController::FrameEnd()
{
	QueryPerformanceCounter(&tEnd);
	dt = (double)((tEnd.QuadPart - tStart.QuadPart) / (double)perfCounter);
	accumTime += dt;

	//do															// Frame Rate Controller (Currently at 60FPS cap)
	//{
	//	QueryPerformanceCounter(&tEnd);
	//	dt = (double)((tEnd.QuadPart - tStart.QuadPart)  / (double)perfCounter);
	//} while (dt  < FRAMES_CAP);
	//Debug  printouts
//	std::cout << dt <<"ms" << std::endl;
//	std::cout << (int)(dt * 60 * 60 / 1000) << "FPS" << std::endl;
//	std::cout << totalTime/1000 << std::endl; 
	totalTime += dt;											// Increment total game time
}

double FrameController::getTotalTime() const								// For retrieving total game time
{
	return totalTime;
}

double FrameController::getDeltaTime() const								// For retrieving delta time
{
	return dt;
}
int FrameController::GetNOS()const
{
	return numberOfSteps;
}

int FrameController::getFPS() const
{
	return static_cast<int>(dt * FRAMES_PER_SECOND*60.f);
}