#pragma once
#include "Event.h"

struct LoadLevel : Event
{
	LoadLevel() :Event{ EventType::LOADLEVEL } {}
	~LoadLevel() {}
};