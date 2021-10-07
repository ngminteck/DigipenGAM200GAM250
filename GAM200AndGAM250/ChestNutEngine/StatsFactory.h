#pragma once
/******************************************************************************/
/*!
\file   HPFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the HPFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ComponentFactory.h"
#include "StatsComponent.h"
#include "rapidjson/document.h"

class StatsFactory : public ComponentFactory
{
public:
	Component * CreateComponent(const rapidjson::Value& node) const override 
	{ 
		float m_HP = 10.0f;
		float m_MP = 2.0f;
		std::string m_char = "SURTR";
		bool m_isDead = false;
		bool m_invul = false;
		float m_invulTimer = 0.0f;

		if (node.HasMember("m_HP"))
			m_HP = node["m_HP"].GetFloat();
		if (node.HasMember("m_MP"))
			m_MP = node["m_MP"].GetFloat();
		if (node.HasMember("m_char"))
			m_char = node["m_char"].GetString();
		if (node.HasMember("m_isDead"))
			m_isDead = node["m_isDead"].GetBool();
		if (node.HasMember("m_invul"))
			m_invul = node["m_invul"].GetBool();
		if (node.HasMember("m_invulTimer"))
			m_invulTimer = node["m_invulTimer"].GetFloat();

		return new StatsComponent{ m_HP, m_MP, m_char, m_isDead, m_invul, m_invulTimer };

		//return new HPComponent(node["m_HP"].GetUint());
	}
	Component * CreateComponent() const override { return new StatsComponent{ 10.0f, 2.0f, "SURTR", false, false, 0.0f }; }
};