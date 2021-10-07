/******************************************************************************/
/*!
\file  ButtonComponent.h

\author ONG, Zhi Hao Jaryl (100%)
\par    email: zhihaojaryl@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the interface of ButtonComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include <string>
#include "SpriteComponent.h"

enum Icon : int
{
	NOTANINDICATOR = -1,
	VOLUME1 = 1,
	VOLUME2 = 2,
	VOLUME3 = 3,
	VOLUME4 = 4,
	VOLUME5 = 5,
};


enum TriggerType : unsigned
{
	SCRIPT     = 0,
	GAMESTATE  = 1,
	CHANGEMENU = 2,
	EXIT       = 3,
	COLLAPSEMENU = 4,
	PAUSEMENU  = 10,
	RESUMEMENU  = 11,
	INCREASEVOL = 12,
	DECREASEVOL = 13,
	PLAYERLOSE  = 20,
	PLAYERCONTINUE = 21
};

class ButtonComponent : public Component
{
public:
	ButtonComponent();
	~ButtonComponent();
	ButtonComponent(const ButtonComponent &);

	Component * Clone() const;
	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	//Setter
	void SetActiveTexName(const std::string &);
	void SetInactiveTexName(const std::string &);
	void SetGameStatePath(const std::string &);
	void SetActiveTex(Texture2D *);
	void SetInactiveTex(Texture2D *);
	void SetCurrTex(Texture2D *);

	//Getter
	std::string GetActiveTexName() const;
	std::string GetInactiveTexName() const;
	std::string GetGameStatePath() const ;
	Texture2D * GetActiveTex() const;
	Texture2D * GetInactiveTex() const;
	Texture2D * GetCurrentTex() const;
	
	unsigned m_buttonID; //button id
	unsigned m_sectionID; //belongs to which section

	TriggerType m_type;  //The type of function it does
	bool m_hovered; //check if button is hovered

	SpriteComponent * m_sprite;

	int m_menuID; //belongs to which menu
	bool m_isMenu; //if it is a menu background
	int m_next; //the menuid of the next menu

	//Sound paths when navigated and entered
	std::string m_navigate = "NO AUDIO";
	std::string m_enter = "NO AUDIO";

	Icon m_icon;

private:
	//Texture when button is hovered
	std::string m_activeTexName;
	Texture2D * m_activeTex;

	//Texture when button is NOT hovered
	std::string m_inactiveTexName;
	Texture2D * m_inactiveTex;

	Texture2D * m_currentTex; //The texture that will be drawn

	std::string m_gamestate; //Stores the json path to change game state

	constexpr static Metadata <ButtonComponent, bool ButtonComponent::*, Texture2D * ButtonComponent::*, Texture2D * ButtonComponent::*, std::string ButtonComponent::*,
		unsigned ButtonComponent::*, TriggerType ButtonComponent::*, int ButtonComponent::*, bool ButtonComponent::*, int ButtonComponent::*, unsigned ButtonComponent::*> metadata
	{ "Button Component", std::make_tuple(&ButtonComponent::m_hovered, &ButtonComponent::m_activeTex, &ButtonComponent::m_inactiveTex, &ButtonComponent::m_gamestate,
										  &ButtonComponent::m_buttonID, &ButtonComponent::m_type, &ButtonComponent::m_menuID, &ButtonComponent::m_isMenu, &ButtonComponent::m_next, &ButtonComponent::m_sectionID),
	std::make_tuple("isHovered", "Active Texture", "Inactive Texture", "Game State", "Button ID", "Trigger Type", "Menu ID", "isMenu" , "Next Menu", "Section ID")
	};
};

bool CompareData(const ButtonComponent & lhs, const ButtonComponent & rhs);


