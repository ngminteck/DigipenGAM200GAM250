#include "DialogueComponent.h"

DialogueComponent::DialogueComponent()
	:Component{ComponentType::DialogueComponent}, m_id{0}
{
}

DialogueComponent::~DialogueComponent()
{
}

Component * DialogueComponent::Clone() const
{
	return new DialogueComponent{ *this };
}

void DialogueComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.Key("ComponentType");
	writer.String("DialogueComponent");

	writer.Key("DialogueID");
	writer.Uint64(m_id);

	if (m_dialogues.size() > 0)
	{
		writer.Key("Dialogues");
		writer.StartArray();
		for (auto & elem : m_dialogues)
		{
			writer.StartObject();
			writer.Key("Dialogue Text");
			writer.String(elem.c_str());
			writer.EndObject();
		}
		writer.EndArray();
	}
}
