#pragma once
#include "ComponentFactory.h"
#include "DialogueComponent.h"
struct DialogueFactory : public ComponentFactory
{
	DialogueFactory();
	~DialogueFactory();

	Component * CreateComponent() const override;
	Component * CreateComponent(const rapidjson::Value& node) const;
};