/******************************************************************************/
/*!
\file   CameraComponent.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file camera component function

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "TransformComponent.h"

#pragma warning(push)
#pragma warning(disable:4201)   // suppress even more warnings about nameless structs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning (pop)
#include "Camera.h"

class CameraComponent : public Component
{

private:
	int m_CompId;

	glm::vec3 * cam_pos;
	glm::vec3 * cam_front;
	glm::vec3 * cam_up;
	

	float *cam_offset_pos_x;
	float *cam_offset_pos_y;

	bool cam_clamp;
	float cam_clamp_min_x;
	float cam_clamp_max_x;
	float cam_clamp_min_y;
	float cam_clamp_max_y;
	
	Camera * cam;

public:
	CameraComponent();
	CameraComponent(float pos_x, float pos_y, float pos_z, float front_x, float front_y, float front_z, float up_x, float up_y, float up_z, float offset_pos_x, float offset_pos_y , bool cam_clap, float clamp_min_x, float clamp_max_x, float clamp_min_y, float clamp_max_y );
	CameraComponent(const CameraComponent & rhs);
	~CameraComponent();
	Component * Clone() const override;

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	Camera * GetCamera();

	glm::vec3 * GetCamPos();
	glm::vec3 * GetCamFront();
	glm::vec3 * GetCamUp();

	float * GetPosOffsetX();
	float * GetPosOffsetY();

	
	constexpr static Metadata <CameraComponent, glm::vec3 * CameraComponent::*, glm::vec3 * CameraComponent::*, glm::vec3 * CameraComponent::*,
		                                        float * CameraComponent::*, float * CameraComponent::*> metadata
	{ "Camera Component", std::make_tuple(&CameraComponent::cam_pos, &CameraComponent::cam_front, &CameraComponent::cam_up,
										  &CameraComponent::cam_offset_pos_x, &CameraComponent::cam_offset_pos_y),
		std::make_tuple("cam_pos", "cam_front", "cam_up",
						"cam_offset_pos_x", "cam_offset_pos_y") };
	static void AddToScript(sol::state * luaState);
	
};