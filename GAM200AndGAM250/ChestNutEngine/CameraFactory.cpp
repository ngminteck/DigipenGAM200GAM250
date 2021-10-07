/******************************************************************************/
/*!
\file   CameraFactory.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file camera factory function

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "CameraFactory.h"
#include "CameraComponent.h"

Component * CameraFactory::CreateComponent(const rapidjson::Value& node) const
{
	float cam_pos_x = 0.0f;
	float cam_pos_y = 0.0f;
	float cam_pos_z = 3.3f;

	float cam_front_x = 0.0f;
	float cam_front_y = 0.0f;
	float cam_front_z = -1.0f;

	float cam_up_x = 0.0f;
	float cam_up_y = 1.0f;
	float cam_up_z = 0.0f;

	float cam_offset_x = 0.0f;
	float cam_offset_y = 0.0f;

	bool cam_clamp = false;

	float cam_clamp_min_x = 0.0f;
	float cam_clamp_max_x = 0.0f;
	float cam_clamp_min_y = 0.0f;
	float cam_clamp_max_y = 0.0f;

	if (node.HasMember("cam_pos_x"))
		cam_pos_x = node["cam_pos_x"].GetFloat();

	if (node.HasMember("cam_pos_y"))
		cam_pos_y = node["cam_pos_y"].GetFloat();

	if (node.HasMember("cam_pos_z"))
		cam_pos_z = node["cam_pos_z"].GetFloat();

	if (node.HasMember("cam_front_x"))
		cam_front_x = node["cam_front_x"].GetFloat();

	if (node.HasMember("cam_front_y"))
		cam_front_y = node["cam_front_y"].GetFloat();

	if (node.HasMember("cam_front_z"))
		cam_front_z = node["cam_front_z"].GetFloat();

	if (node.HasMember("cam_up_x"))
		cam_up_x = node["cam_up_x"].GetFloat();

	if (node.HasMember("cam_up_y"))
		cam_up_y = node["cam_up_y"].GetFloat();

	if (node.HasMember("cam_up_z"))
		cam_up_z = node["cam_up_z"].GetFloat();

	if (node.HasMember("cam_offset_x"))
		cam_offset_x = node["cam_offset_x"].GetFloat();

	if (node.HasMember("cam_offset_y"))
		cam_offset_y = node["cam_offset_y"].GetFloat();

	if (node.HasMember("cam_clamp"))
		cam_clamp = node["cam_clamp"].GetBool();


	if (node.HasMember("cam_clamp_min_x"))
		cam_clamp_min_x = node["cam_clamp_min_x"].GetFloat();

	if (node.HasMember("cam_clamp_max_x"))
		cam_clamp_max_x = node["cam_clamp_max_x"].GetFloat();

	if (node.HasMember("cam_clamp_min_y"))
		cam_clamp_min_y = node["cam_clamp_min_y"].GetFloat();

	if (node.HasMember("cam_clamp_max_y"))
		cam_clamp_max_y = node["cam_clamp_max_y"].GetFloat();

	return new CameraComponent{ cam_pos_x ,cam_pos_y ,cam_pos_z ,cam_front_x ,cam_front_y ,cam_front_z ,cam_up_x ,cam_up_y , cam_up_z, cam_offset_x, cam_offset_y , cam_clamp, cam_clamp_min_x,cam_clamp_max_x,cam_clamp_min_y,cam_clamp_max_y };
}

Component * CameraFactory::CreateComponent() const
{
	return new CameraComponent{};
}
