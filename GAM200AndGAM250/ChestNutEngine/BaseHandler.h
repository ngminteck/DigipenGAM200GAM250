#pragma once
/******************************************************************************/
/*!
\file  BaseHandler.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for BaseHandler

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Event.h"
class BaseHandler
{

public:
	//Interface for executing the event
	void ExecuteEvent(Event * evnt) {
		Call(evnt);
	}

private:
	virtual void Call(Event * evnt) const = 0;
};

