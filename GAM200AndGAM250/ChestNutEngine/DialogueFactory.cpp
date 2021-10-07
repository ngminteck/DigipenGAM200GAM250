#include "DialogueFactory.h"

DialogueFactory::DialogueFactory()
{
}

DialogueFactory::~DialogueFactory()
{
}


Component * DialogueFactory::CreateComponent(const rapidjson::Value& node) const
{
	UNREFERENCED_PARAMETER(node);
	return new DialogueComponent{};
}

Component * DialogueFactory::CreateComponent() const
{
	return new DialogueComponent{};
}