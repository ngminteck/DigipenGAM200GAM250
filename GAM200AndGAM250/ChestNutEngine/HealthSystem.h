#pragma once
/******************************************************************************/
/*!
\file  HealthSystem.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for HealthSystem class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "System.h"
#include "StatsComponent.h"
#include "HealthEvent.h"
#include <vector>
#include "GameObject.h"

class HealthSystem : System
{
	public:
	HealthSystem(EventHandler *);
	~HealthSystem();
	void UpdateHealthComponent(HealthEvent *);
	Profiler profiler;

private:
	EventHandler * m_ev;
};
