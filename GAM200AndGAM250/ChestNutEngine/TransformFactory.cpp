#pragma once
/******************************************************************************/
/*!
\file  TransformFactory.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for TransformFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "TransformFactory.h"
#include "TransformComponent.h"

Component * TransformFactory::CreateComponent(const rapidjson::Value& node) const
{
	Vector3D m_Translate, m_Scale;
    Vector2D m_Pos;
	float m_Rotate = 0.0f;

	// old
	if (node.HasMember("m_pos.x"))
		m_Pos.x = node["m_pos.x"].GetFloat();

	if (node.HasMember("m_pos.y"))
		m_Pos.y = node["m_pos.y"].GetFloat();

	// new
	if (node.HasMember("m_Pos.x"))
		m_Pos.x = node["m_Pos.x"].GetFloat();

	if (node.HasMember("m_Pos.y"))
		m_Pos.y = node["m_Pos.y"].GetFloat();

	if (node.HasMember("m_Translate.y"))
		m_Translate.y = node["m_Translate.y"].GetFloat();


	if (node.HasMember("m_Translate.x"))
		m_Translate.x = node["m_Translate.x"].GetFloat();

	if (node.HasMember("m_Translate.y"))
		m_Translate.y = node["m_Translate.y"].GetFloat();

	if (node.HasMember("m_Translate.z"))
		m_Translate.z = node["m_Translate.z"].GetFloat();

	if (node.HasMember("m_Rotate"))
		m_Rotate = node["m_Rotate"].GetFloat();

	if (node.HasMember("m_Scale.x"))
		m_Scale.x = node["m_Scale.x"].GetFloat();

	if (node.HasMember("m_Scale.y"))
		m_Scale.y = node["m_Scale.y"].GetFloat();

	if (node.HasMember("m_Scale.z"))
		m_Scale.z = node["m_Scale.z"].GetFloat();

	return new TransformComponent{ m_Pos,m_Translate ,m_Rotate ,m_Scale };

	/*float scale_x = node["m_SCALE_x"].GetFloat();
	float scale_y = node["m_SCALE_y"].GetFloat();
	float translate_x = node["m_TRANSLATE_x"].GetFloat();
	float translate_y = node["m_TRANSLATE_y"].GetFloat();
	float translate_z = node["m_TRANSLATE_z"].GetFloat();
	float rotate = node["m_ROTATE"].GetFloat();

	return new TransformComponent{ Vector3D{translate_x,translate_y,translate_z},
								   rotate,Vector3D{scale_x,scale_y,0.f} };*/
}

Component * TransformFactory::CreateComponent() const 
{
	return new TransformComponent{ Vector2D{}, Vector3D{}, 0.0f, Vector3D{} };
}
