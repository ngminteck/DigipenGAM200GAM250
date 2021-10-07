#include "CheckpointComponent.h"

CheckpointComponent::CheckpointComponent(std::string path)
	:Component{ComponentType::CheckpointComponent}, m_path{path}
{
}

CheckpointComponent::~CheckpointComponent()
{
}

Component * CheckpointComponent::Clone() const
{
	return new CheckpointComponent(*this);
}

void CheckpointComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.Key("ComponentType");
	writer.String("CheckpointComponent");

	writer.Key("File path");
	writer.String(m_path.c_str());
}
