#pragma once
/******************************************************************************/
/*!
\file PauseGame.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for PauseGame class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Event.h"
struct PauseGame: Event
{
	PauseGame() : Event{ EventType::PAUSE } {}
	~PauseGame() {}
};