/******************************************************************************/
/*!
\file   AudioComponent.cpp

\author ONG, Zhi Hao Jaryl (100%)
\par    email: zhihaojaryl@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the interface of AudioComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "AudioComponent.h"

bool CompareData(const AudioComponent & lhs, const AudioComponent & rhs)
{

	auto lhs_map = lhs.audioMap;
	auto rhs_map = rhs.audioMap;

	if (lhs_map.size() != rhs_map.size())
		return false;
	else
	{
		for (auto & lhs1 : lhs_map)
		{
			UNREFERENCED_PARAMETER(lhs1);
			for (auto & rhs1 : rhs_map)
			{
				//if the state cannot be found, return false
				if (lhs_map.find(rhs1.first) == lhs_map.end())
					return false;
				else
				{
					auto i = lhs_map.begin();
					auto j = rhs_map.begin();

					for (; i != lhs_map.end(); ++i, ++j)
					{
						if (i->second.m_loop != j->second.m_loop)
							return false;
						if (i->second.m_path != j->second.m_path)
							return false;
					}
				}
			}
		}
	}

	return true;
}


AudioComponent::AudioComponent()
:Component{ ComponentType::AudioComponent }, currState{ "IDLE" }, channelID{-1}
{
}

AudioComponent::~AudioComponent()
{
}

void AudioComponent::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<AudioComponent>("Component",
		"GetComponentType", &Component::GetComponentType);
	luaState->new_usertype<AudioComponent>("AudioComponent",
		"clone", &AudioComponent::Clone,
		"currstate", &AudioComponent::currState,
		"audiomap", &AudioComponent::audioMap,

		sol::base_classes, sol::bases<Component>()
		);
}

Component * AudioComponent::Clone() const
{
	return new AudioComponent{ *this };
}

void AudioComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("AudioComponent");
	if (audioMap.size() > 0)
	{
		writer.Key("audioMap");
		writer.StartArray();
		for (auto & elem : audioMap)
		{
			writer.StartObject();
			writer.Key("Path");
			writer.String(elem.second.m_path.c_str());
			writer.Key("Volume");
			writer.Double(double(elem.second.m_vol));
			writer.Key("Loop");
			writer.Bool(elem.second.m_loop);
			writer.Key("State");
			writer.String(elem.first.c_str());
			writer.EndObject();
		}
		writer.EndArray();
	}
}

std::map<std::string, Audio> * AudioComponent::GetAudioData()
{
	return &audioMap;
}
