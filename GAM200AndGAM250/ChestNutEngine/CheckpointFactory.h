#pragma once
#include "ComponentFactory.h"
#include "CheckpointComponent.h"
struct CheckpointFactory : ComponentFactory
{

	CheckpointFactory();
	~CheckpointFactory();

	Component * CreateComponent(const rapidjson::Value& node) const;
	Component * CreateComponent() const;
};