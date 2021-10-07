/******************************************************************************/
/*!
\file   WinLoseEvent.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of WinLoseEvent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Event.h"
struct WinLoseEvent : Event
{
	WinLoseEvent(bool val) 
		:Event{ EventType::WINLOSE },
		m_winlose{ val }
	{ }
	bool m_winlose;
};