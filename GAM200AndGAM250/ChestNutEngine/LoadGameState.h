#pragma once
#include <string>
#include "Event.h"
struct LoadGameState : Event
{
	LoadGameState(std::string path) :Event{ EventType::CHANGEGAMESTATE }, m_path{ path }{}
	~LoadGameState() {}

	std::string m_path;
};