#pragma once
#include "Event.h"

struct PlayLevel : Event
{
	PlayLevel() :Event{ EventType::PLAY } {}
	~PlayLevel() {}
};