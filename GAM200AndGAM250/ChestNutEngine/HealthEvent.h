#pragma once
/******************************************************************************/
/*!
\file  HealthEvent.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for HealthEvent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Event.h"
#include "StatsComponent.h"

class HealthEvent : public Event
{

public:
	HealthEvent(std::string, int, StatsComponent* hpComp);
	~HealthEvent();
	EventType GetEventType() const;
	std::string GetID() const;
	unsigned GetValue() const;
	StatsComponent* GetHpComponent() const;

private:
	std::string m_ID; //GameObject ID to modify
	int m_value; //Value to update;
	EventType m_EventType;
	StatsComponent * m_hpComp;
};