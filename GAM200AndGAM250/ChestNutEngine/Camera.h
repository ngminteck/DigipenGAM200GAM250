/******************************************************************************/
/*!
\file   Camera.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains function and stucts declaration for Camera

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include "GraphicsSystem.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Metadata.h"
#include <math.h>
#include "Random.h"

// x 1.6
// y 1
const float screen_width = 1.6f;

class Camera
{
private:
	glm::vec3 * m_CamPos;
	glm::vec3 * m_CamFront;
	glm::vec3 *m_WorldUp;
	glm::mat4 m_ViewMatrix;
	SceneActor* m_GameObj;
    Vector2D min;
    Vector2D max;

	float *m_CamOffsetPosX;
	float *m_CamOffsetPosY;

	bool cam_clamp;
	float map_min_x;
	float map_max_x;
	float map_min_y;
	float map_max_y;

	glm::vec3 m_CamPos_Origin;
	glm::vec3 m_CamPos_Dest;

	bool correct_position;
	bool correct_zoom;
	bool move_back_to_original_object;

	float camera_shake_x;
	float camera_shake_y;
	float amplitude;
	float frequency;
	float oscillation_duration;
	float camera_shake_move_value_x;
	float camera_shake_move_value_y;

	bool view_location;
	bool view_object;
	bool auto_lerp_back_to_original_object;
	bool start_countdown;
	float view_stay_duration;
	SceneActor * view_actor;
	bool cam_lock;
	glm::vec3 m_CamPos_lock;
	bool zooming;
	bool temp_zooming;

	float lerp_speed_for_zoom;
	float lerp_speed_for_pos;

	bool cutscene_mode;
	bool cutscene_move_end;
	bool cutscene_move_to_location;
	
public:
	Camera(glm::vec3 * pos, glm::vec3 * front, glm::vec3 * up , float* cam_offset_pos_x, float* cam_offset_pos_y , bool clamp, float clamp_minx, float clamp_maxx , float clamp_miny , float clamp_maxy );
	~Camera();

	void MakePosFrontUpNull();

	void Update(float dt);

	void LatchToObject(std::map<unsigned, SceneActor *> & obj,std::string);
	void LatchToObject(SceneActor *);
	void UnlatchObject();

	void SetPosition(float x, float y, float z= 1.f);

	void ViewObject(SceneActor*);
	void AddPosition(float x, float y, float z= 0.0f);
    void UpdateCamAABB();

	void SetClampLimit(float min_x, float max_x, float min_y, float max_y);



	//Getters
	
	glm::vec3 GetPosition();
	Vector3D GetPositionVector();
	glm::mat4 GetViewMatrix();
    Vector2D GetMin();
    Vector2D GetMax();

	SceneActor * GetGameObject() const;


	void ViewBackItself(float pos_lerp_speed = 0.0f, float zoom_lerp_speed = 0.0f);
	void SetSelfView(float x, float y, float z, float pos_lerp_speed = 0.0f, float zoom_lerp_speed = 0.0f);
	void CameraSwitch(float x, float y, float z, float pos_lerp_speed = 0.0f, float zoom_lerp_speed = 0.0f);
	
	void SetCameraShake(float amp, float freq, float osc);

	void SaveCameraOriginPos();
	void SetViewLocation(float x, float y, float z, bool back_to_object, float timer , float pos_lerp_speed = 0.0f, float zoom_lerp_speed = 0.0f);
	void SetViewObject(SceneActor* object, float z, bool back_to_object, float timer, float pos_lerp_speed = 0.0f, float zoom_lerp_speed = 0.0f);

	void SetViewLock(float pos_lerp_speed = 0.0f);
	void SetViewUnLock();

	void SetObjectAnchorPoint(unsigned pos = 5, float padding_x =0.0f, float padding_y=0.0f);
	void OffObjectAnchorPoint();

	void SetZoom(float z, float zoom_lerp_speed = 0.0f);
	float GetZoom()const;

	void SetTempZoom(float z, bool back_to_object, float timer , float zoom_lerp_speed = 0.0f);

	void SetZoomLerpSpeed(float zoom_lerp_speed);

	void SetPosLerpSpeed(float pos_lerp_speed);

	void CutSceneModeStart();
	void CutSceneModeMoveLocation(float x, float y, float z, float timer, float pos_lerp_speed = 0.0f, float zoom_lerp_speed = 0.0f);
	void CutSceneModeEnd();
	bool GetIsCutSceneStillOnLerp()const;

	void SetOriginZoom(float z);

	void SetViewBackItselfNoLerp();

	

	constexpr static Metadata <Camera, glm::vec3 * Camera::*> metadata
	{ "Camera", std::make_tuple(&Camera::m_CamPos), std::make_tuple("m_CamPos") };

	static void AddToScript(sol::state * luaState);
};