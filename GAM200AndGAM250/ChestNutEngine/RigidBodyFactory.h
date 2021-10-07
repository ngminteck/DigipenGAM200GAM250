#pragma once
/******************************************************************************/
/*!
\file   RigidBodyFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the RigidBodyFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "RigidBodyComponent.h"
#include "ComponentFactory.h"
#include "rapidjson/document.h"

class RigidBodyFactory : public ComponentFactory
{
public:
	Component * CreateComponent(const rapidjson::Value& node ) const override 
	{
		Vector2D m_pos{ 0,0 }, m_vel{ 0,0 }, m_accel{ 0,0 }, m_direction{ 0,0 };
		float m_mass = 0.0f, m_gravity = 0.0f;
		bool isStaticObj = false;
		std::string m_objType = "DEFAULT";
		bool m_hasGravity = true;
		float m_startTime = 0.0f;
		float m_duration = 0.0f;
		float m_modA = 0.0f;
		float m_modB = 0.0f;
		float m_modC = 0.0f;

		if (node.HasMember("m_vel.x"))
			m_vel.x = node["m_vel.x"].GetFloat();

		if (node.HasMember("m_vel.y"))
			m_vel.y = node["m_vel.y"].GetFloat();

		if (node.HasMember("m_accel.x"))
			m_accel.x = node["m_accel.x"].GetFloat();

		if (node.HasMember("m_accel.y"))
			m_accel.y = node["m_accel.y"].GetFloat();

		if (node.HasMember("m_direction.x"))
			m_direction.x = node["m_direction.x"].GetFloat();

		if (node.HasMember("m_direction.y"))
			m_direction.y = node["m_direction.y"].GetFloat();

		if (node.HasMember("m_mass"))
			m_mass = node["m_mass"].GetFloat();

		if (node.HasMember("m_gravity"))
			m_gravity = node["m_gravity"].GetFloat();

		if (node.HasMember("m_isStaticObj"))
			isStaticObj = node["m_isStaticObj"].GetBool();

		if (node.HasMember("m_objType"))
			m_objType = node["m_objType"].GetString();

		if (node.HasMember("m_hasGravity"))
			m_hasGravity = node["m_hasGravity"].GetBool();

		if (node.HasMember("m_startTime"))
			m_startTime = node["m_startTime"].GetFloat();

		if (node.HasMember("m_duration"))
			m_duration = node["m_duration"].GetFloat();

		if (node.HasMember("m_modA"))
			m_modA = node["m_modA"].GetFloat();

		if (node.HasMember("m_modB"))
			m_modB = node["m_modB"].GetFloat();

		if (node.HasMember("m_modC"))
			m_modC = node["m_modC"].GetFloat();

		return new RigidBodyComponent(m_vel, m_accel, m_direction, m_mass, m_gravity, isStaticObj, m_startTime, m_duration, m_modA, m_modB, m_modC, m_objType, m_hasGravity);

	
	}

	Component * CreateComponent() const
	{
		return new RigidBodyComponent{ Vector2D{0.0f, 0.0f}, Vector2D{0.0f, 0.0f}, Vector2D{0.0f, 0.0f}, 1.0f, 0.0f, false, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, "DEFAULT", true};
	}


};