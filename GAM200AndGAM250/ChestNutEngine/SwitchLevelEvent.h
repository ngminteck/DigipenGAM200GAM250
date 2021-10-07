/******************************************************************************/
/*!
\file   SwitchLevelEvent.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of SwitchLevelEvent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Event.h"
struct SwitchLevelEvent :  Event
{
	SwitchLevelEvent() :Event{ EventType::SWITCHLEVEL } {}
};