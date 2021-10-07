#include "ButtonFactory.h"

ButtonFactory::ButtonFactory(SpriteSheetFactory * sprite)
: m_sprite{sprite}
{
}

ButtonFactory::~ButtonFactory()
{
}

Component * ButtonFactory::CreateComponent(const rapidjson::Value & node) const
{
	ButtonComponent * button = new ButtonComponent{};

	if (node.HasMember("Active Texture"))
	{
		//Set texture 
		button->SetActiveTexName(node["Active Texture"].GetString());

		//Set texture name
		auto map = m_sprite->GetContainer();
		unsigned key = GetSpriteKeyBySpriteName(map, button->GetActiveTexName());
		button->SetActiveTex((*map)[key]);
	}

	if (node.HasMember("isHovered"))
		button->m_hovered = node["isHovered"].GetBool();


	if (node.HasMember("Inactive Texture"))
	{
		//Set texture name
		button->SetInactiveTexName(node["Inactive Texture"].GetString());

		//Set texture
		auto map = m_sprite->GetContainer();
		unsigned key = GetSpriteKeyBySpriteName(map, button->GetInactiveTexName());
		button->SetInactiveTex((*map)[key]);
	}

	if (node.HasMember("Button ID"))
		button->m_buttonID = node["Button ID"].GetUint();

	if (node.HasMember("Icon"))
		button->m_icon = static_cast<Icon>(node["Icon"].GetInt());

	if (node.HasMember("Trigger Type"))
		button->m_type = static_cast<TriggerType>(node["Trigger Type"].GetInt());

	if (node.HasMember("Section ID"))
		button->m_sectionID = node["Section ID"].GetInt();

	//Set gamestate
	if (node.HasMember("Game State"))
		button->SetGameStatePath(node["Game State"].GetString());

	if (node.HasMember("isHovered"))
		button->m_hovered = node["isHovered"].GetBool();

	if (node.HasMember("Menu ID"))
		button->m_menuID = node["Menu ID"].GetInt();

	if (node.HasMember("isMenu"))
		button->m_isMenu = node["isMenu"].GetBool();

	if (node.HasMember("Next Menu"))
		button->m_next = node["Next Menu"].GetInt();


	if (node.HasMember("Navigate"))
		button->m_navigate = node["Navigate"].GetString();

	if (node.HasMember("Enter"))
		button->m_enter = node["Enter"].GetString();

	return button;
}


Component * ButtonFactory::CreateComponent() const
{
	return new ButtonComponent{};
}
