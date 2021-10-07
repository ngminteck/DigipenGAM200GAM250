#pragma once
/******************************************************************************/
/*!
\file  AABBFactory.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for AABBFactory

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ComponentFactory.h"
#include "AABBComponent.h"

class AABBFactory : public ComponentFactory
{
public:
	Component * CreateComponent(const rapidjson::Value& node) const override 
	{ 
		float ScaleX = 0.5, ScaleY = 0.5;
		float offsetX = 0.0f, offsetY = 0.0f;
		
		if (node.HasMember("m_aabbScale.x"))
			ScaleX = node["m_aabbScale.x"].GetFloat();

		if (node.HasMember("m_aabbScale.y"))
			ScaleY = node["m_aabbScale.y"].GetFloat();

		if (node.HasMember("m_offset.x"))
			offsetX = node["m_offset.x"].GetFloat();

		if (node.HasMember("m_offset.y"))
			offsetY = node["m_offset.y"].GetFloat();

		return new AABBComponent{ ScaleX , ScaleY , offsetX, offsetY};


		//return new AABBComponent{node["ScaleX"].GetFloat(),node["ScaleY"].GetFloat()};
	}
	Component * CreateComponent() const override { return new AABBComponent{}; }
};