#pragma once
/******************************************************************************/
/*!
\file  System.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for System class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EventHandler.h"
#include "BaseHandler.h"
#include "Profiler.h"

class System
{
public:
	System() {} //Constructor
   virtual ~System() {} //Destructor
   //virtual void MessageSend(Event *) const = 0; //Receive message

};