#pragma once

#include "Component.h"

struct CheckpointComponent : Component
{
	CheckpointComponent(std::string = "");
	~CheckpointComponent();

	Component * Clone() const;
	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	std::string m_path;

	constexpr static Metadata <CheckpointComponent, std::string CheckpointComponent::*> metadata
	{ "Button Component", std::make_tuple(&CheckpointComponent::m_path), std::make_tuple("File path")};
};