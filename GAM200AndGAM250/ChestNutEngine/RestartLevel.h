#pragma once

#include "Event.h"

struct RestartLevel : Event
{
	RestartLevel() :Event{ EventType::MISC_EVENT } {}
	~RestartLevel() {}
};