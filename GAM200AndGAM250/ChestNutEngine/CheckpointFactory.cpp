#include "CheckpointFactory.h"

CheckpointFactory::CheckpointFactory()
{
}

CheckpointFactory::~CheckpointFactory()
{
}

Component * CheckpointFactory::CreateComponent(const rapidjson::Value & node) const
{
	CheckpointComponent * checkpoint = new CheckpointComponent{};

	if (node.HasMember("File path"))
	{
		//Set texture 
		checkpoint->m_path = node["File path"].GetString();
	}

	return checkpoint;
}

Component * CheckpointFactory::CreateComponent() const
{
	return new CheckpointComponent{""};
}
