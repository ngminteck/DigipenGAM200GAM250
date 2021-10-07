#pragma once
/******************************************************************************/
/*!
\file PlayAudio.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for PlayAudio class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Event.h"
#include "Audio.h"
struct PlayAudio : Event
{
	PlayAudio(Audio audio) : m_audio{ audio }, Event { EventType::AUDIO } {}
	~PlayAudio() {}

	Audio m_audio;
};