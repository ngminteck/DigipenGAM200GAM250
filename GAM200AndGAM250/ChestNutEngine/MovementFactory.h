#pragma once
/******************************************************************************/
/*!
\file   MovementFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the MovementFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ComponentFactory.h"
#include "MovementComponent.h"
#include "rapidjson/document.h"

class MovementFactory:public ComponentFactory
{
public:
	Component * CreateComponent(const rapidjson::Value& node) const override
	{
		std::string state;
		int dir = 0;

		if (node.HasMember("m_State"))
			state = node["m_State"].GetString();

		if (node.HasMember("m_Dir"))
			dir = node["m_Dir"].GetInt();

		return new MovementComponent{ state,dir };
	
		//return new MovementComponent{ node["State"].GetInt(),node["Dir"].GetInt() };
	}

	Component * CreateComponent() const override { return new MovementComponent{"IDLE",0};}
};
