#pragma once
#include "Event.h"
#include "InputDefines.h"

class MoveEvent : Event
{
public:
	MoveEvent(std::string, int, EventType);
	~MoveEvent();

private:
	std::string m_ID; //GameObject ID to modify
	unsigned m_value; //Value to update;
	EventType m_EventType;
};