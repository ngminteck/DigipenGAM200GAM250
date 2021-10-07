#pragma once
#include "Event.h"

struct ChangeAudio : Event
{
	ChangeAudio() :Event{ EventType::EDITAUDIO } {}
	~ChangeAudio() {}
};