#pragma once
/******************************************************************************/
/*!
\file  AudioFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the MovementFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ComponentFactory.h"
#include "AudioComponent.h"
#include "rapidjson/document.h"

class AudioFactory :public ComponentFactory
{
public:
	Component * CreateComponent(const rapidjson::Value& node) const
	{

		AudioComponent * comp = new AudioComponent{};
		if (node.HasMember("audioMap"))
		{
			const rapidjson::Value& scriptNode = node["audioMap"];
			for (rapidjson::SizeType i = 0; i < scriptNode.Size(); ++i)
			{
		
				if (scriptNode[i].HasMember("State") == false) continue;
				if (scriptNode[i].HasMember("Path") == false) continue;

				std::string state{ scriptNode[i]["State"].GetString() };
				std::string path{ scriptNode[i]["Path"].GetString() };
				
				bool loop = false;
				float vol = 0.0f;

				if (scriptNode[i].HasMember("Volume"))
					vol = (float)scriptNode[i]["Volume"].GetDouble();
				if (scriptNode[i].HasMember("Loop"))
					loop = scriptNode[i]["Loop"].GetBool();

				comp->audioMap.emplace(state, Audio{path, loop, vol});

			}
		}
		return comp;
	}

	Component * CreateComponent() const 
	{ 
		return new AudioComponent{};
	}
};
