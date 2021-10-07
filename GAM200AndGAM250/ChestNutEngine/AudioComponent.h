/******************************************************************************/
/*!
\file   AudioComponent.h

\author ONG, Zhi Hao Jaryl (100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of AudioComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include <string>
#include <map>
#include "Audio.h"
struct AudioComponent : public Component
{
	AudioComponent();
	~AudioComponent();
	void AddToScript(sol::state * luaState);
	Component * Clone() const;
	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);
	std::map<std::string, Audio> * GetAudioData();

	std::string currState; //Current state of the audio component
	std::map<std::string, Audio> audioMap; //<State, Audio>

	int channelID;
	constexpr static Metadata <AudioComponent, std::string AudioComponent::*, std::map<std::string, Audio> AudioComponent::*> metadata
	{ "Audio Component", std::make_tuple(&AudioComponent::currState, &AudioComponent::audioMap), std::make_tuple("currState", "audioMap") };
};

bool CompareData(const AudioComponent & lhs, const AudioComponent & rhs);


