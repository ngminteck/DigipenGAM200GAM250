#pragma once
/******************************************************************************/
/*!
\file  Event.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for BaseHandler

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include <iostream>
#include <string>

enum EventType : int
{
	HEALTH = 1,
	COLLIDE = 2,
	MOVE = 3,
	SWITCHLEVEL = 4,
	PLAYSCRIPT = 5,
	AUDIO = 6,
	DELETEOBJECT = 7,
	TOGGLEGRAVITY = 8,
	DAMAGE = 9,
	PAUSE = 10,
	RESUME = 11,
	LOADLEVEL = 12,
	PLAY = 13,
	WINLOSE = 14,
	EDITAUDIO = 15,
	PLAYERDIES = 16,
	ENDGAME       = 17,
	CHANGEGAMESTATE = 18,
	FADEIN          = 19,
	MISC_EVENT     = -1
};
struct Event
{
	Event(EventType type = MISC_EVENT) :m_type{ type } {}
	virtual ~Event() = 0;
	EventType m_type;
};