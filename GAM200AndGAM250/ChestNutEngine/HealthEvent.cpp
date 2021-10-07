/******************************************************************************/
/*!
\file  HealthEvent.cpp
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for HealthEvent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "HealthEvent.h"

HealthEvent::HealthEvent(std::string id, int value, StatsComponent* hpComp)
	:m_ID{ id }, m_value{ value }, m_hpComp{ hpComp }, Event{ EventType::HEALTH } {}

EventType HealthEvent::GetEventType() const
{
	return m_EventType;
}

std::string HealthEvent::GetID() const
{
	return m_ID;
}

unsigned HealthEvent::GetValue() const
{
	return m_value;
}

StatsComponent * HealthEvent::GetHpComponent() const
{
	return m_hpComp;
}

HealthEvent::~HealthEvent() { }