#pragma once
/******************************************************************************/
/*!
\file ToggleGravity.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for System class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Event.h"
struct ToggleGravity : Event
{
	ToggleGravity(): Event{ EventType::TOGGLEGRAVITY } {}
	~ToggleGravity() {}
};
