/******************************************************************************/
/*!
\file   DamageEvent.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of DamageEvent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Event.h"
struct DamageEvent : Event
{
	DamageEvent(StatsComponent* statComp, unsigned damage) 
		:Event{ EventType::DAMAGE },
		m_statComp(statComp),
		m_damage(damage)
	{ }
	StatsComponent* m_statComp;
	unsigned m_damage;
};