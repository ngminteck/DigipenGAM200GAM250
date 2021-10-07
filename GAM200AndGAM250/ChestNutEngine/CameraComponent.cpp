/******************************************************************************/
/*!
\file   CameraComponent.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file camera component function

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "CameraComponent.h"

CameraComponent::CameraComponent()
	:Component{ ComponentType::CameraComponent }, cam_pos{ new glm::vec3(0.0f,0.0f,3.3f) }, cam_front{ new glm::vec3(0.0f,0.0f,-1.0f) }, cam_up{ new glm::vec3(0.0f,1.0f,0.0f) },
	cam_offset_pos_x{new float(0.0f)}, cam_offset_pos_y{ new float(0.0f)}, cam_clamp{ false }, cam_clamp_min_x{ 0.0f }, cam_clamp_max_x{ 0.0f }, cam_clamp_min_y{ 0.0f }, cam_clamp_max_y{ 0.0f },
	cam{ new Camera {cam_pos,cam_front ,cam_up ,cam_offset_pos_x,cam_offset_pos_y,cam_clamp,cam_clamp_min_x,cam_clamp_max_x,cam_clamp_min_y,cam_clamp_max_y} }
{

}

CameraComponent::CameraComponent(float pos_x, float pos_y, float pos_z, float front_x, float front_y, float front_z, float up_x, float up_y, float up_z, float offset_pos_x, float offset_pos_y, bool cam_clamp= false, float clamp_min_x =0.0f, float clamp_max_x =0.0f, float clamp_min_y=0.0f, float clamp_max_y =0.0f)
	:Component{ ComponentType::CameraComponent }, cam_pos{ new glm::vec3(pos_x,pos_y,pos_z) }, cam_front{ new glm::vec3(front_x,front_y,front_z) }, cam_up{ new glm::vec3(up_x,up_y,up_z) },
	cam_offset_pos_x{ new float(offset_pos_x) }, cam_offset_pos_y{ new float(offset_pos_y) },cam_clamp{ cam_clamp }, cam_clamp_min_x{ clamp_min_x }, cam_clamp_max_x{ clamp_max_x }, cam_clamp_min_y{ clamp_min_y }, cam_clamp_max_y{ clamp_max_y },
	cam{ new Camera {cam_pos,cam_front ,cam_up ,cam_offset_pos_x,cam_offset_pos_y,cam_clamp,cam_clamp_min_x,cam_clamp_max_x,cam_clamp_min_y,cam_clamp_max_y} }
{
	
}

CameraComponent::CameraComponent(const CameraComponent & rhs)
	:Component{ ComponentType::CameraComponent }, cam_pos{ new glm::vec3(rhs.cam_pos->x,rhs.cam_pos->y,rhs.cam_pos->z) },
	cam_front{ new glm::vec3(rhs.cam_front->x,rhs.cam_front->y,rhs.cam_front->z) }, cam_up{ new glm::vec3(rhs.cam_up->x,rhs.cam_up->y,rhs.cam_up->z) },
	cam_offset_pos_x{ new float(*rhs.cam_offset_pos_x) }, cam_offset_pos_y{ new float( *rhs.cam_offset_pos_y) },cam_clamp{ rhs.cam_clamp }, 
	cam_clamp_min_x{ rhs.cam_clamp_min_x }, cam_clamp_max_x{ rhs.cam_clamp_max_x }, cam_clamp_min_y{ rhs.cam_clamp_min_y }, cam_clamp_max_y{ rhs.cam_clamp_max_y },
	cam{ new Camera {cam_pos,cam_front ,cam_up ,cam_offset_pos_x,cam_offset_pos_y,cam_clamp,cam_clamp_min_x,cam_clamp_max_x,cam_clamp_min_y,cam_clamp_max_y} }
{

}

CameraComponent::~CameraComponent()
{
	cam->UnlatchObject();
	delete cam;
	delete cam_pos;
	delete cam_front;
	delete cam_up;
	delete cam_offset_pos_x;
	delete cam_offset_pos_y;
}

Component * CameraComponent::Clone() const
{
	return new CameraComponent(*this);
}

void CameraComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("CameraComponent");

	writer.Key("cam_pos_x");
	writer.Double(cam_pos->x);

	writer.Key("cam_pos_y");
	writer.Double(cam_pos->y);

	writer.Key("cam_pos_z");
	writer.Double(cam_pos->z);

	writer.Key("cam_front_x");
	writer.Double(cam_front->x);

	writer.Key("cam_front_y");
	writer.Double(cam_front->y);

	writer.Key("cam_front_z");
	writer.Double(cam_front->z);

	writer.Key("cam_up_x");
	writer.Double(cam_up->x);

	writer.Key("cam_up_y");
	writer.Double(cam_up->y);

	writer.Key("cam_up_z");
	writer.Double(cam_up->z);

	writer.Key("cam_offset_pos_x");
	writer.Double(*cam_offset_pos_x);

	writer.Key("cam_offset_pos_y");
	writer.Double(*cam_offset_pos_y);

	writer.Key("cam_clamp");
	writer.Bool(cam_clamp);

	writer.Key("cam_clamp_min_x");
	writer.Double(cam_clamp_min_x);
	writer.Key("cam_clamp_max_x");
	writer.Double(cam_clamp_max_x);
	writer.Key("cam_clamp_min_y");
	writer.Double(cam_clamp_min_y);
	writer.Key("cam_clamp_max_y");
	writer.Double(cam_clamp_max_y);

}

Camera * CameraComponent::GetCamera()
{
	return cam;
}

glm::vec3 * CameraComponent::GetCamPos()
{
	return cam_pos;
}

glm::vec3 * CameraComponent::GetCamFront()
{
	return cam_front;
}

glm::vec3 * CameraComponent::GetCamUp()
{
	return cam_up;
}

float * CameraComponent::GetPosOffsetX()
{
	return cam_offset_pos_x;
}

float * CameraComponent::GetPosOffsetY()
{
	return cam_offset_pos_y;
}

void CameraComponent::AddToScript(sol::state * luaState)
{

	luaState->new_usertype<CameraComponent>("CameraComponent",
		// Member functions
		"GetCamera", &CameraComponent::GetCamera,
		// Variables
		sol::base_classes, sol::bases<Component>()
		);
}
