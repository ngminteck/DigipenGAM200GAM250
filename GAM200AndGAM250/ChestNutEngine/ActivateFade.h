#pragma once
#include "Event.h"
struct ActivateFade : Event
{
	ActivateFade(bool in = true) : Event{EventType::FADEIN}, m_in{ in }  {}
	~ActivateFade() {}

	bool m_in;
};