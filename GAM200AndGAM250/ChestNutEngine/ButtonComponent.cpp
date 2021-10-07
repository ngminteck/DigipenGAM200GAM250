/******************************************************************************/
/*!
\file  ButtonComponent.cpp

\author ONG, Zhi Hao Jaryl (100%)
\par    email: zhihaojaryl@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the interface of ButtonComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent()
	:Component{ ComponentType::ButtonComponent }, m_buttonID{ 0 }, m_hovered{ false }, m_menuID{0}, m_isMenu{ false }, m_sectionID{0}, m_icon{-1}
{
}

ButtonComponent::~ButtonComponent()
{
}

ButtonComponent::ButtonComponent(const ButtonComponent & rhs)
	:Component{ComponentType::ButtonComponent}, m_activeTexName{rhs.m_activeTexName}, m_activeTex{ rhs.m_activeTex},
	m_inactiveTex{rhs.m_inactiveTex}, m_inactiveTexName{m_inactiveTexName}, m_currentTex{rhs.m_currentTex}, m_gamestate{rhs.m_gamestate}
	, m_buttonID{rhs.m_buttonID}, m_type{rhs.m_type}, m_hovered{false}, m_next{-1}, m_icon{rhs.m_icon}
{
}

Component * ButtonComponent::Clone() const
{
	return new ButtonComponent{ *this };
}

void ButtonComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.Key("ComponentType");
	writer.String("ButtonComponent");

	writer.Key("isHovered");
	writer.Bool(m_hovered);

	writer.Key("Active Texture");
	writer.String(m_activeTexName.c_str());

	writer.Key("Inactive Texture");
	writer.String(m_inactiveTexName.c_str());

	writer.Key("Button ID");
	writer.Int(m_buttonID);

	writer.Key("Trigger Type");
	writer.Int(m_type);

	writer.Key("Game State");
	writer.String(m_gamestate.c_str());

	writer.Key("Menu ID");
	writer.Int(m_menuID);

	writer.Key("isMenu");
	writer.Bool(m_isMenu);

	writer.Key("Next Menu");
	writer.Int(m_next);

	writer.Key("Section ID");
	writer.Int(m_sectionID);

	writer.Key("Icon");
	writer.Int(m_icon);

	writer.Key("Navigate");
	writer.String(m_navigate.c_str());

	writer.Key("Enter");
	writer.String(m_enter.c_str());

}

void ButtonComponent::SetActiveTexName(const std::string & tex)
{
	m_activeTexName = tex;
}

void ButtonComponent::SetInactiveTexName(const std::string & tex)
{
	m_inactiveTexName = tex;
}

void ButtonComponent::SetGameStatePath(const std::string & path)
{
	m_gamestate = path;
}

void ButtonComponent::SetActiveTex(Texture2D * tex)
{
	m_activeTex = tex;
}

void ButtonComponent::SetInactiveTex(Texture2D *tex )
{
	m_inactiveTex = tex;
}

void ButtonComponent::SetCurrTex(Texture2D * tex)
{
	m_currentTex = tex;
}

std::string ButtonComponent::GetActiveTexName() const
{
	return m_activeTexName;
}

std::string ButtonComponent::GetInactiveTexName() const
{
	return m_inactiveTexName;
}

std::string ButtonComponent::GetGameStatePath() const
{
	return m_gamestate;
}

Texture2D * ButtonComponent::GetActiveTex() const
{
	return m_activeTex;
}

Texture2D * ButtonComponent::GetInactiveTex() const
{
	return m_inactiveTex;
}

Texture2D * ButtonComponent::GetCurrentTex() const
{
	return m_currentTex;
}


bool CompareData(const ButtonComponent & lhs, const ButtonComponent & rhs)
{
	if (lhs.GetActiveTexName() != rhs.GetActiveTexName()) 
		return false;

	if (lhs.GetInactiveTexName() != rhs.GetInactiveTexName()) 
		return false;

	if (lhs.GetGameStatePath() != rhs.GetGameStatePath()) 
		return false;

	if (lhs.m_buttonID != rhs.m_buttonID) 
		return false;
	
	if (lhs.m_type != rhs.m_type) 
		return false;

	if (lhs.m_isMenu != rhs.m_isMenu)
		return false;

	if (lhs.m_menuID != rhs.m_menuID)
		return false;

	if (lhs.m_next != rhs.m_next)
		return false;

	if (lhs.m_icon != rhs.m_icon)
		return false;
	
	if (lhs.m_sectionID != rhs.m_sectionID)
		return false;

	return true;
}
