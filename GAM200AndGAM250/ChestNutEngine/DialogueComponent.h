#pragma once
#include <vector>
#include "Texture2D.h"
#include "Component.h"
struct DialogueComponent : Component
{
	DialogueComponent();
	~DialogueComponent();

	Component * Clone() const;

	std::vector<std::string> m_dialogues;

	int m_id;

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

};