/******************************************************************************/
/*!
\file  HealthSystem.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for HealthSystem class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "HealthSystem.h"
#include <iostream>

HealthSystem::HealthSystem(EventHandler * ev)
:m_ev{ev}, profiler{"Health System"}
{
	ev->SubscribeEvent(this, &HealthSystem::UpdateHealthComponent);
}

HealthSystem::~HealthSystem()
{

}

void HealthSystem::UpdateHealthComponent(HealthEvent *health)
{
	UNREFERENCED_PARAMETER(health);
	//extract data from health event.
	//health->GetHpComponent()->UpdateHP(health->GetValue());
	//health system find the object based on id
	//health system updates the health component of that id.
	//health->PrintMessage("Health System");
}
