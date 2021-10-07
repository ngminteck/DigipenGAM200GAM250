/******************************************************************************/
/*!
\file   Camera.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains function and stucts implementation for Camera

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Camera.h"
#include "SceneActor.h"
#include "RigidBodyComponent.h"
Camera::Camera(glm::vec3 * pos, glm::vec3 * front, glm::vec3 * up, float* cam_offset_pos_x, float* cam_offset_pos_y, bool clamp = false, float clamp_minx = 0.0f, float clamp_maxx = 0.0f, float clamp_miny = 0.0f , float clamp_maxy = 0.0f)
:m_CamPos{pos},m_CamFront{front}, m_WorldUp{up},m_ViewMatrix{1.0f}, m_CamOffsetPosX{ cam_offset_pos_x },m_CamOffsetPosY{cam_offset_pos_y},cam_clamp{clamp}, map_min_x{clamp_minx} , map_max_x{ clamp_maxx }, map_min_y{ clamp_miny }, map_max_y{ clamp_maxy },
m_CamPos_Origin{ pos->x ,pos->y,pos->z }, m_CamPos_Dest{ 0.0f,0.0f,0.0f }, correct_position{ true }, correct_zoom{ true }, move_back_to_original_object{ false }, camera_shake_x{ 0.0f }, camera_shake_y{ 0.0f }, amplitude{ 0.0f }, frequency{ 0.0f }, oscillation_duration{ 0.0f },
camera_shake_move_value_x{ 0.0f }, camera_shake_move_value_y{ 0.0f }, view_location{ false }, view_object{ false }, auto_lerp_back_to_original_object{ false }, start_countdown{ false },view_stay_duration{ 0.0f }, view_actor{nullptr},
cam_lock{ false }, m_CamPos_lock{ 0.0f,0.0f,0.0f }, zooming{ false }, temp_zooming{ false }, lerp_speed_for_zoom{0.05f}, lerp_speed_for_pos{1.0f},
cutscene_mode{false}, cutscene_move_end{true}, cutscene_move_to_location{false}
{
	
  m_ViewMatrix = glm::lookAt(*m_CamPos, *m_CamPos + *m_CamFront,* m_WorldUp);
  UpdateCamAABB();
}

Camera::~Camera()
{
	view_actor = nullptr;
	m_CamPos = nullptr;
	m_CamFront = nullptr;
	m_WorldUp = nullptr;
	m_CamOffsetPosX = nullptr;
	m_CamOffsetPosY = nullptr;
}

void Camera::MakePosFrontUpNull()
{
	view_actor = nullptr;
	m_CamPos = nullptr;
	m_CamFront = nullptr;
	m_WorldUp = nullptr;
	m_CamOffsetPosX = nullptr;
	m_CamOffsetPosY = nullptr;
}

void Camera::LatchToObject(std::map<unsigned, SceneActor *> & map, std::string name)
{
	if (map.find(GetSceneActorKeyByName(&map, name)) != map.end())
		m_GameObj = map.find(GetSceneActorKeyByName(&map, name))->second;
}

void Camera::LatchToObject(SceneActor * obj)
{
	if(obj)
		m_GameObj = obj;
}


void Camera::UnlatchObject()
{
	m_GameObj = nullptr;
}


void Camera::ViewObject(SceneActor* obj)
{
	if (m_GameObj) {
		auto map = obj->GetPrefabActor()->GetComponentsPtr();
		if (map->find(ComponentType::TransformComponent) != map->end())
		{
			TransformComponent * trans = dynamic_cast<TransformComponent*>((*map)[ComponentType::TransformComponent]);
			m_CamPos->x = trans->GetPosition().x;
			m_CamPos->y = trans->GetPosition().y;
		}
	}
}

void Camera::Update(float dt)
{
	if (m_GameObj)
	{
		auto map = m_GameObj->GetPrefabActor()->GetComponentsPtr();
		if (!map)
			return;

		if (map->find(ComponentType::TransformComponent) != map->end())
		{
			TransformComponent * trans = dynamic_cast<TransformComponent*>((*map)[ComponentType::TransformComponent]);

			float half_object_scale_x = trans->GetScale().x * 0.5f / 1.6f;
			float half_object_scale_y = trans->GetScale().y * 0.5f;

			float new_cam_pos_x = 0.0f;
			float new_cam_pos_y = 0.0f;

			if (cutscene_mode)
			{
				if (cutscene_move_to_location)
				{
					if (!correct_zoom)
					{
						float z2 = m_CamPos_Dest.z;
						float z1 = m_CamPos->z;
						float zoom_different = z2 - z1;

						float check_different = abs(zoom_different);

						if (check_different <= 0)
						{
							m_CamPos->z = m_CamPos_Dest.z;
							correct_zoom = true;
						}
						else
						{
							z1 += zoom_different * lerp_speed_for_zoom;
							m_CamPos->z = z1;
						}
					}
					else
					{
						m_CamPos->z = m_CamPos_Dest.z;
					}

					float x2 = m_CamPos_Dest.x;
					float y2 = m_CamPos_Dest.y;

					if (cam_clamp)
					{
						float zoom = m_CamPos->z;
						float clamp_min_x = map_min_x + (zoom * 1.6f);
						float clamp_max_x = map_max_x - (zoom * 1.6f);
						float clamp_min_y = map_min_y + (zoom * 1.0f);

						if (x2 < clamp_min_x)
							x2 = clamp_min_x;

						if (x2 > clamp_max_x)
							x2 = clamp_max_x;

						if (y2 < clamp_min_y)
							y2 = clamp_min_y;
					}

					if (!correct_position)
					{
						float x1 = m_CamPos->x;
						float y1 = m_CamPos->y;

						float ang = atan2(y2 - y1, x2 - x1);
						float distance = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));

						if (distance <= lerp_speed_for_pos)
						{
							new_cam_pos_x = x2;
							new_cam_pos_y = y2;
							correct_position = true;
						}
						else
						{
							new_cam_pos_x = x1 + cos(ang) * lerp_speed_for_pos;
							new_cam_pos_y = y1 + sin(ang) * lerp_speed_for_pos;
						}
					}
					else
					{
						new_cam_pos_x = x2;
						new_cam_pos_y = y2;
					}

					if (correct_zoom && correct_position)
					{
						start_countdown = true;
					}
				}
				else
				{
					if (!correct_zoom)
					{
						float z2 = m_CamPos_Dest.z;
						float z1 = m_CamPos->z;
						float zoom_different = z2 - z1;

						float check_different = abs(zoom_different);

						if (check_different <= 0)
						{
							m_CamPos->z = m_CamPos_Dest.z;
							correct_zoom = true;
						}
						else
						{
							z1 += zoom_different * lerp_speed_for_zoom;
							m_CamPos->z = z1;
						}
					}
					else
					{
						m_CamPos->z = m_CamPos_Dest.z;
					}

					float x2 = trans->GetPosition().x + *m_CamOffsetPosX;
					float y2 = trans->GetPosition().y + *m_CamOffsetPosY;

					if (cam_clamp)
					{
						float zoom = m_CamPos->z;
						float clamp_min_x = map_min_x + (zoom * 1.6f);
						float clamp_max_x = map_max_x - (zoom * 1.6f);
						float clamp_min_y = map_min_y + (zoom * 1.0f);

						if (x2 < clamp_min_x)
							x2 = clamp_min_x;

						if (x2 > clamp_max_x)
							x2 = clamp_max_x;

						if (y2 < clamp_min_y)
							y2 = clamp_min_y;
					}

					if (!correct_position)
					{
						float x1 = m_CamPos->x;
						float y1 = m_CamPos->y;

						float ang = atan2(y2 - y1, x2 - x1);
						float distance = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));

						if (distance <= lerp_speed_for_pos)
						{
							new_cam_pos_x = x2;
							new_cam_pos_y = y2;
							correct_position = true;
						}
						else
						{
							new_cam_pos_x = x1 + cos(ang) * lerp_speed_for_pos;
							new_cam_pos_y = y1 + sin(ang) * lerp_speed_for_pos;
						}

					}
					else
					{
						new_cam_pos_x = x2;
						new_cam_pos_y = y2;
					}
					if (correct_zoom && correct_position)
					{
						cutscene_move_end = true;
						cutscene_mode = false;
					}
				}

				if (start_countdown)
				{
					if (view_stay_duration > 0.0f)
					{
						view_stay_duration -= dt;
					}

					if (view_stay_duration <= 0.0f)
					{
						view_stay_duration = 0.0f;
						start_countdown = false;
						cutscene_move_end = true;
					}
				}


			}
			else
			{
				if (cam_lock)
				{
					float x2 = m_CamPos_lock.x;
					float y2 = m_CamPos_lock.y;
					if (temp_zooming)
					{
						if (!correct_zoom)
						{
							float z2 = m_CamPos_Dest.z;
							float z1 = m_CamPos->z;
							float zoom_different = z2 - z1;

							float check_different = abs(zoom_different);

							if (check_different <= 0)
							{
								m_CamPos->z = m_CamPos_Dest.z;
								correct_zoom = true;
								temp_zooming = false;
								start_countdown = true;
							}
							else
							{
								z1 += zoom_different * lerp_speed_for_zoom;
								m_CamPos->z = z1;
							}
						}
					}
					else if (zooming)
					{
						if (!correct_zoom)
						{
							float z2 = m_CamPos_Dest.z;
							float z1 = m_CamPos->z;
							float zoom_different = z2 - z1;

							float check_different = abs(zoom_different);

							if (check_different <= 0)
							{
								m_CamPos->z = m_CamPos_Dest.z;
								correct_zoom = true;
								zooming = false;
							}
							else
							{
								z1 += zoom_different * lerp_speed_for_zoom;
								m_CamPos->z = z1;
							}
						}
					}

					if (cam_clamp)
					{
						float zoom = m_CamPos->z;
						float clamp_min_x = map_min_x + (zoom * 1.6f);
						float clamp_max_x = map_max_x - (zoom * 1.6f);
						float clamp_min_y = map_min_y + (zoom * 1.0f);

						if (x2 < clamp_min_x)
							x2 = clamp_min_x;

						if (x2 > clamp_max_x)
							x2 = clamp_max_x;

						if (y2 < clamp_min_y)
							y2 = clamp_min_y;
					}
					m_CamPos_lock.x = x2;
					m_CamPos_lock.y = y2;

					float screen_boundary_min_x = m_CamPos_lock.x + -(m_CamPos_lock.z * 1.6f) + half_object_scale_x;
					float screen_boundary_max_x = m_CamPos_lock.x + (m_CamPos_lock.z * 1.6f) - half_object_scale_x;

					float screen_boundary_max_y = m_CamPos_lock.y + m_CamPos_lock.z - half_object_scale_y;
					float screen_boundary_min_y = m_CamPos_lock.y - m_CamPos_lock.z + half_object_scale_y;

					m_CamPos->z = m_CamPos_lock.z;

					new_cam_pos_x = m_CamPos_lock.x;
					new_cam_pos_y = m_CamPos_lock.y;

					float current_x = trans->GetPosition().x;
					float current_y = trans->GetPosition().y;
					if (current_x < screen_boundary_min_x)
					{
						SetViewUnLock();
					}
					else if (current_x > screen_boundary_max_x)
					{
						SetViewUnLock();
					}
					else if (current_y < screen_boundary_min_y)
					{
						SetViewUnLock();
					}
					else if (current_y > screen_boundary_max_y)
					{
						SetViewUnLock();
					}
				}
				else
				{
					if (move_back_to_original_object)
					{
						if (!correct_zoom)
						{
							float z2 = m_CamPos_Dest.z;
							float z1 = m_CamPos->z;
							float zoom_different = z2 - z1;

							float check_different = abs(zoom_different);

							if (check_different <= 0)
							{
								m_CamPos->z = m_CamPos_Dest.z;
								correct_zoom = true;
							}
							else
							{
								z1 += zoom_different * lerp_speed_for_zoom;
								m_CamPos->z = z1;
							}
						}
						else
						{
							m_CamPos->z = m_CamPos_Dest.z;
						}

						float x2 = trans->GetPosition().x + *m_CamOffsetPosX;
						float y2 = trans->GetPosition().y + *m_CamOffsetPosY;

						if (cam_clamp)
						{
							float zoom = m_CamPos->z;
							float clamp_min_x = map_min_x + (zoom * 1.6f);
							float clamp_max_x = map_max_x - (zoom * 1.6f);
							float clamp_min_y = map_min_y + (zoom * 1.0f);

							if (x2 < clamp_min_x)
								x2 = clamp_min_x;

							if (x2 > clamp_max_x)
								x2 = clamp_max_x;

							if (y2 < clamp_min_y)
								y2 = clamp_min_y;
						}

						if (!correct_position)
						{
							float x1 = m_CamPos->x;
							float y1 = m_CamPos->y;

							float ang = atan2(y2 - y1, x2 - x1);
							float distance = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));

							if (distance <= lerp_speed_for_pos)
							{
								new_cam_pos_x = x2;
								new_cam_pos_y = y2;
								correct_position = true;
							}
							else
							{
								//new_cam_pos_x = x1 + ((x2 - x1) * 0.005f);
								//new_cam_pos_y = y1 + ((y2 - y1) *  0.005f);
								new_cam_pos_x = x1 + cos(ang) * lerp_speed_for_pos;
								new_cam_pos_y = y1 + sin(ang) * lerp_speed_for_pos;
							}

						}
						else
						{
							new_cam_pos_x = x2;
							new_cam_pos_y = y2;
						}
						if (correct_zoom && correct_position)
						{
							
							view_location = false;
							view_location = false;
							move_back_to_original_object = false;
						}
						//std::cout << "itself" << std::endl;
					}
					else if (view_location)
					{
						if (!correct_zoom)
						{
							float z2 = m_CamPos_Dest.z;
							float z1 = m_CamPos->z;
							float zoom_different = z2 - z1;

							float check_different = abs(zoom_different);

							if (check_different <= 0)
							{
								m_CamPos->z = m_CamPos_Dest.z;
								correct_zoom = true;
							}
							else
							{
								z1 += zoom_different * lerp_speed_for_zoom;
								m_CamPos->z = z1;
							}
						}
						else
						{
							m_CamPos->z = m_CamPos_Dest.z;
						}

						float x2 = m_CamPos_Dest.x;
						float y2 = m_CamPos_Dest.y;

						if (cam_clamp)
						{
							float zoom = m_CamPos->z;
							float clamp_min_x = map_min_x + (zoom * 1.6f);
							float clamp_max_x = map_max_x - (zoom * 1.6f);
							float clamp_min_y = map_min_y + (zoom * 1.0f);

							if (x2 < clamp_min_x)
								x2 = clamp_min_x;

							if (x2 > clamp_max_x)
								x2 = clamp_max_x;

							if (y2 < clamp_min_y)
								y2 = clamp_min_y;
						}

						if (!correct_position)
						{
							float x1 = m_CamPos->x;
							float y1 = m_CamPos->y;

							float ang = atan2(y2 - y1, x2 - x1);
							float distance = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));

							if (distance <= lerp_speed_for_pos)
							{
								new_cam_pos_x = x2;
								new_cam_pos_y = y2;
								correct_position = true;
							}
							else
							{
								new_cam_pos_x = x1 + cos(ang) * lerp_speed_for_pos;
								new_cam_pos_y = y1 + sin(ang) * lerp_speed_for_pos;
							}
						}
						else
						{
							new_cam_pos_x = x2;
							new_cam_pos_y = y2;
						}

						if (correct_zoom && correct_position)
						{
							start_countdown = true;
						}
						//std::cout << "view location" << std::endl;
					}
					else if (view_object)
					{
						if (view_actor != nullptr)
						{
							auto view_map = view_actor->GetPrefabActor()->GetComponentsPtr();
							if (view_map->find(ComponentType::TransformComponent) != view_map->end())
							{
								TransformComponent * view_trans = dynamic_cast<TransformComponent*>((*view_map)[ComponentType::TransformComponent]);

								if (!correct_zoom)
								{
									float z2 = m_CamPos_Dest.z;
									float z1 = m_CamPos->z;
									float zoom_different = z2 - z1;

									float check_different = abs(zoom_different);

									if (check_different <= 0)
									{
										m_CamPos->z = m_CamPos_Dest.z;
										correct_zoom = true;
									}
									else
									{
										m_CamPos->z = z1 + zoom_different * lerp_speed_for_zoom;
									}
								}
								else
								{
									m_CamPos->z = m_CamPos_Dest.z;
								}

								float x2 = view_trans->GetPosition().x;
								float y2 = view_trans->GetPosition().y;

								if (cam_clamp)
								{
									float zoom = m_CamPos->z;
									float clamp_min_x = map_min_x + (zoom * 1.6f);
									float clamp_max_x = map_max_x - (zoom * 1.6f);
									float clamp_min_y = map_min_y + (zoom * 1.0f);

									if (x2 < clamp_min_x)
										x2 = clamp_min_x;

									if (x2 > clamp_max_x)
										x2 = clamp_max_x;

									if (y2 < clamp_min_y)
										y2 = clamp_min_y;
								}
								if (!correct_position)
								{
									float x1 = m_CamPos->x;
									float y1 = m_CamPos->y;

									float ang = atan2(y2 - y1, x2 - x1);
									float distance = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));

									if (distance <= lerp_speed_for_pos)
									{
										new_cam_pos_x = x2;
										new_cam_pos_y = y2;
										correct_position = true;
									}
									else
									{
										new_cam_pos_x = x1 + cos(ang) * lerp_speed_for_pos;
										new_cam_pos_y = y1 + sin(ang) * lerp_speed_for_pos;
									}

								}
								else
								{
									new_cam_pos_x = x2;
									new_cam_pos_y = y2;
								}

								if (correct_zoom && correct_position)
								{
									start_countdown = true;
								}
							}
						}
					}
					else if (temp_zooming)
					{
						float x2 = trans->GetPosition().x + *m_CamOffsetPosX;
						float y2 = trans->GetPosition().y + *m_CamOffsetPosY;

						if (!correct_zoom)
						{
							float z2 = m_CamPos_Dest.z;
							float z1 = m_CamPos->z;

							float zoom_different = z2 - z1;

							float check_different = abs(zoom_different);

							if (check_different <= 0)
							{
								m_CamPos->z = m_CamPos_Dest.z;
								correct_zoom = true;
								start_countdown = true;
							}
							else
							{
								z1 += zoom_different * lerp_speed_for_zoom;
								m_CamPos->z = z1;
							}
						}


						//std::cout << "x2:" << x2 << " y2:" << y2 << std::endl;
						if (cam_clamp)
						{
							float zoom = m_CamPos->z;
							float clamp_min_x = map_min_x + (zoom * 1.6f);
							float clamp_max_x = map_max_x - (zoom * 1.6f);
							float clamp_min_y = map_min_y + (zoom * 1.0f);

							if (x2 < clamp_min_x)
								x2 = clamp_min_x;

							if (x2 > clamp_max_x)
								x2 = clamp_max_x;

							if (y2 < clamp_min_y)
								y2 = clamp_min_y;
						}
						new_cam_pos_x = x2;
						new_cam_pos_y = y2;

						//	std::cout << "new_cam_pos_x:" << new_cam_pos_x << " new_cam_pos_y:"<< new_cam_pos_y << std::endl;
							//std::cout << std::endl;
					}
					else if (zooming)
					{
						float x2 = trans->GetPosition().x + *m_CamOffsetPosX;
						float y2 = trans->GetPosition().y + *m_CamOffsetPosY;

						if (!correct_zoom)
						{
							float z2 = m_CamPos_Dest.z;
							float z1 = m_CamPos->z;
							float zoom_different = z2 - z1;

							float check_different = abs(zoom_different);

							if (check_different <= 0)
							{
								m_CamPos->z = m_CamPos_Dest.z;
								correct_zoom = true;
								zooming = false;
							}
							else
							{
								z1 += zoom_different * lerp_speed_for_zoom;
								m_CamPos->z = z1;
							}
						}

						if (cam_clamp)
						{
							float zoom = m_CamPos->z;
							float clamp_min_x = map_min_x + (zoom * 1.6f);
							float clamp_max_x = map_max_x - (zoom * 1.6f);
							float clamp_min_y = map_min_y + (zoom * 1.0f);

							if (x2 < clamp_min_x)
								x2 = clamp_min_x;

							if (x2 > clamp_max_x)
								x2 = clamp_max_x;

							if (y2 < clamp_min_y)
								y2 = clamp_min_y;
						}

						new_cam_pos_x = x2;
						new_cam_pos_y = y2;
					}
					else
					{

						new_cam_pos_x = trans->GetPosition().x + *m_CamOffsetPosX;
						new_cam_pos_y = trans->GetPosition().y + *m_CamOffsetPosY;

					}
				}

				if (start_countdown)
				{
					if (view_stay_duration > 0.0f)
					{
						view_stay_duration -= dt;
					}

					if (view_stay_duration <= 0.0f)
					{
						if (auto_lerp_back_to_original_object)
						{
							SetSelfView(new_cam_pos_x, new_cam_pos_y, m_CamPos->z);
							auto_lerp_back_to_original_object = false;
						}

						view_stay_duration = 0.0f;
						start_countdown = false;
					}
				}


				if (oscillation_duration > 0.0f)
				{
					if (camera_shake_x == 0.0f && camera_shake_y == 0.0f)
					{
						camera_shake_x = RandFloat(-amplitude, amplitude);
						camera_shake_y = RandFloat(-amplitude, amplitude);

						if (camera_shake_x != 0.0f)
						{
							// something wrong of this formula
							camera_shake_move_value_x = (camera_shake_x / frequency) * dt * 4;
						}

						if (camera_shake_y != 0.0f)
						{
							// something wrong of this formula
							camera_shake_move_value_y = (camera_shake_y / frequency) * dt * 4;
						}
					}
					oscillation_duration -= dt;

					if (oscillation_duration <= 0.0f)
						oscillation_duration = 0.0f;

				}

				if (camera_shake_x != 0.0f)
				{
					if (camera_shake_x > 0.0f)
					{
						camera_shake_x -= camera_shake_move_value_x;
						if (camera_shake_x < 0.0f)
							camera_shake_x = 0.0f;
					}
					else
					{
						camera_shake_x -= camera_shake_move_value_x;
						if (camera_shake_x > 0.0f)
							camera_shake_x = 0.0f;
					}
				}

				if (camera_shake_y != 0.0f)
				{
					if (camera_shake_y > 0.0f)
					{
						camera_shake_y -= camera_shake_move_value_y;
						if (camera_shake_y < 0.0f)
							camera_shake_y = 0.0f;
					}
					else
					{
						camera_shake_y -= camera_shake_move_value_y;
						if (camera_shake_y > 0.0f)
							camera_shake_y = 0.0f;
					}
				}

				// add in camera shake vector
				//std::cout << camera_shake_x << " " << camera_shake_y << std::endl;
				new_cam_pos_x = new_cam_pos_x + camera_shake_x;
				new_cam_pos_y = new_cam_pos_y + camera_shake_y;
				// if clamp
				if (cam_clamp)
				{
					float zoom = m_CamPos->z;
					float clamp_min_x = map_min_x + (zoom * 1.6f);
					float clamp_max_x = map_max_x - (zoom * 1.6f);
					float clamp_min_y = map_min_y + zoom;

					if (new_cam_pos_x < clamp_min_x)
						new_cam_pos_x = clamp_min_x;

					if (new_cam_pos_x > clamp_max_x)
						new_cam_pos_x = clamp_max_x;

					if (new_cam_pos_y < clamp_min_y)
						new_cam_pos_y = clamp_min_y;


				}
			}

			m_CamPos->x = new_cam_pos_x;
			m_CamPos->y = new_cam_pos_y;

			//m_CamPos->x += (new_cam_pos_x - m_CamPos->x) * 0.5f;
			//m_CamPos->y += (new_cam_pos_y - m_CamPos->y) * 0.5f;

			UpdateCamAABB();

		}

	}
}

void Camera::SetPosition(float x, float y, float z)
{
	m_CamPos->x = x;
	m_CamPos->y = y;
	m_CamPos->z = z;
    UpdateCamAABB();
}

void Camera::AddPosition(float x, float y, float z)
{
	m_CamPos->x += x;
	m_CamPos->y += y;
	m_CamPos->z += z;
  UpdateCamAABB();
}
void Camera::UpdateCamAABB()
{
  min.x = m_CamPos->x - m_CamPos->z*4.f;
  max.x = m_CamPos->x + m_CamPos->z*4.f;

  min.y = m_CamPos->y - m_CamPos->z*2.f;
  max.y = m_CamPos->y + m_CamPos->z*2.f;
}
glm::vec3 Camera::GetPosition()
{
	return *m_CamPos;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(*m_CamPos, *m_CamPos + *m_CamFront, *m_WorldUp);
}

SceneActor * Camera::GetGameObject() const
{
	return m_GameObj;
}


Vector2D Camera::GetMin()
{
  return min;
}
Vector2D Camera::GetMax()
{
  return max;
}

void Camera::SetClampLimit(float min_x, float max_x, float min_y, float max_y)
{
	map_min_x = min_x;
	map_max_x = max_x;
	map_min_y = min_y;
	map_max_y = max_y;
}

void Camera::ViewBackItself(float pos_lerp_speed, float zoom_lerp_speed)
{
	//std::cout << "pos: " << pos_lerp_speed << std::endl;
	//std::cout << "zoom: " << zoom_lerp_speed << std::endl;
	SetSelfView(m_CamPos->x, m_CamPos->y, m_CamPos->z, pos_lerp_speed, zoom_lerp_speed);
}

void Camera::CameraSwitch(float x, float y, float z, float pos_lerp_speed, float zoom_lerp_speed)
{
	cutscene_mode = false;
	SaveCameraOriginPos();
	SetSelfView(x, y, z, pos_lerp_speed, zoom_lerp_speed);
}


void Camera::SetSelfView(float x, float y, float z , float pos_lerp_speed, float zoom_lerp_speed)
{
	m_CamPos_Dest.z = m_CamPos_Origin.z;


	if (z <= 0)
	{
		m_CamPos_Dest.z = m_CamPos_Origin.z;
	}


	//m_CamPos->x += (x - m_CamPos->x) * 0.1f;
	//m_CamPos->y += (y - m_CamPos->y) * 0.1f;

	m_CamPos->x = x;
	m_CamPos->y = y;
	m_CamPos->z = z;

	/*
	std::cout << "SetSelfView" << std::endl;
	std::cout << "current:x" << m_CamPos->x << " y:" << m_CamPos->y << " z:" << m_CamPos->z << std::endl;
	std::cout << "dest:x" << m_CamPos_Dest.x << " y:" << m_CamPos_Dest.y << " z:" << m_CamPos_Dest.z << std::endl;
	std::cout << "origin:x" << m_CamPos_Origin.x << " y:" << m_CamPos_Origin.y << " z:" << m_CamPos_Origin.z << std::endl;
	std::cout << std::endl;*/
	

	if (pos_lerp_speed > 0.0f)
		lerp_speed_for_pos = pos_lerp_speed;

	if (zoom_lerp_speed > 0.0f)
		lerp_speed_for_zoom = zoom_lerp_speed;

	cam_lock = false;
	view_object = false;
	view_location = false;
	correct_position = false;
	correct_zoom = false;
	move_back_to_original_object = true;
	view_actor = nullptr; 
	zooming = false;
	temp_zooming = false;
}


void Camera::SetCameraShake(float amp, float freq, float osc)
{
	amplitude = amp;
	frequency = freq;
	oscillation_duration = osc;

	if (frequency == 0.0f)
		frequency = 1.0f;

	camera_shake_x = 0.0f;
	camera_shake_y = 0.0f;
}



void Camera::SetViewLocation(float x, float y, float z, bool back_to_object, float timer, float pos_lerp_speed, float zoom_lerp_speed)
{
	if (!cutscene_mode)
	{
	
		m_CamPos_Dest.x = x;
		m_CamPos_Dest.y = y;

		if (z <= 0)
		{
			m_CamPos_Dest.z = m_CamPos->z;
		}
		else
		{
			m_CamPos_Dest.z = z;
		}

		std::cout << "SetViewLocation" << std::endl;
		std::cout << "current:x" << m_CamPos->x << " y:" << m_CamPos->y << " z:" << m_CamPos->z << std::endl;
		std::cout << "dest:x" << m_CamPos_Dest.x << " y:" << m_CamPos_Dest.y << " z:" << m_CamPos_Dest.z << std::endl;
		std::cout << "origin:x" << m_CamPos_Origin.x << " y:" << m_CamPos_Origin.y << " z:" << m_CamPos_Origin.z << std::endl;
		std::cout << std::endl;

		if (pos_lerp_speed > 0.0f)
			lerp_speed_for_pos = pos_lerp_speed;

		if (zoom_lerp_speed > 0.0f)
			lerp_speed_for_zoom = zoom_lerp_speed;


		view_location = true;
		view_object = false;
		correct_position = false;
		correct_zoom = false;
		auto_lerp_back_to_original_object = back_to_object;
		view_stay_duration = timer;
		zooming = false;
		temp_zooming = false;
		cam_lock = false;
		move_back_to_original_object = false;
	}
}

void Camera::SetViewObject(SceneActor* object, float z, bool back_to_object, float timer , float pos_lerp_speed, float zoom_lerp_speed)
{
	if (!cutscene_mode)
	{
		view_actor = object;

		if (z <= 0)
		{
			m_CamPos_Dest.z = m_CamPos_Origin.z;
		}
		else
		{
			m_CamPos_Dest.z = z;
		}

		if (pos_lerp_speed > 0.0f)
			lerp_speed_for_pos = pos_lerp_speed;

		if (zoom_lerp_speed > 0.0f)
			lerp_speed_for_zoom = zoom_lerp_speed;


		view_location = false;
		view_object = true;
		correct_position = false;
		correct_zoom = false;
		auto_lerp_back_to_original_object = back_to_object;
		view_stay_duration = timer;
		zooming = false;
		temp_zooming = false;
		cam_lock = false;
		move_back_to_original_object = false;
	}
}

Vector3D Camera::GetPositionVector()
{
	return Vector3D{ m_CamPos->x,m_CamPos->y,m_CamPos->z };
}

void Camera::SaveCameraOriginPos()
{
	m_CamPos_Origin.x = m_CamPos->x;
	m_CamPos_Origin.y = m_CamPos->y;
	m_CamPos_Origin.z = m_CamPos->z;
}

void Camera::SetViewLock(float pos_lerp_speed)
{
	if (!cutscene_mode)
	{
		SaveCameraOriginPos();

		m_CamPos_lock.x = m_CamPos->x;
		m_CamPos_lock.y = m_CamPos->y;
		m_CamPos_lock.z = m_CamPos->z;

		std::cout << "SetViewLock" << std::endl;
		std::cout << "current:x" << m_CamPos->x << " y:" << m_CamPos->y << " z:" << m_CamPos->z << std::endl;
		std::cout << "dest:x" << m_CamPos_Dest.x << " y:" << m_CamPos_Dest.y << " z:" << m_CamPos_Dest.z << std::endl;
		std::cout << "origin:x" << m_CamPos_Origin.x << " y:" << m_CamPos_Origin.y << " z:" << m_CamPos_Origin.z << std::endl;
		std::cout << std::endl;

		if (pos_lerp_speed > 0.0f)
			lerp_speed_for_pos = pos_lerp_speed;

		view_location = false;
		view_object = false;
		correct_position = true;
		correct_zoom = true;
		cam_lock = true;
		zooming = false;
		temp_zooming = false;
	}
}

void Camera::SetViewUnLock()
{
	if (!cutscene_mode)
	{
		cam_lock = false;
		
		SetSelfView(m_CamPos_lock.x, m_CamPos_lock.y, m_CamPos_lock.z, lerp_speed_for_pos, lerp_speed_for_zoom);
	}
}

void Camera::SetObjectAnchorPoint(unsigned pos, float padding_x, float padding_y)
{
	if (m_GameObj != nullptr)
	{
		auto map = m_GameObj->GetPrefabActor()->GetComponentsPtr();
		if (map->find(ComponentType::TransformComponent) != map->end())
		{
			TransformComponent * trans = dynamic_cast<TransformComponent*>((*map)[ComponentType::TransformComponent]);
			float half_object_scale_x = trans->GetScale().x * 0.5f / 1.6f;
			float half_object_scale_y = trans->GetScale().y * 0.5f;
			switch (pos)
			{
			
				case 1:
					*m_CamOffsetPosX = (m_CamPos->z * 1.6f) - half_object_scale_x - padding_x;
					*m_CamOffsetPosY = -(m_CamPos->z * 1.0f) + half_object_scale_y + padding_y;
					break;

				case 2:
					*m_CamOffsetPosX = 0.0f;
					*m_CamOffsetPosY = -(m_CamPos->z * 1.0f) + half_object_scale_y + padding_y;
					break;

				case 3:
					*m_CamOffsetPosX = -(m_CamPos->z * 1.6f) + half_object_scale_x + padding_x;
					*m_CamOffsetPosY = -(m_CamPos->z * 1.0f) + half_object_scale_y + padding_y;
					break;

				case 4:
					*m_CamOffsetPosX = (m_CamPos->z * 1.6f)  - half_object_scale_x - padding_x;
					*m_CamOffsetPosY = 0.0f;
					break;

				case 6:
					*m_CamOffsetPosX = -(m_CamPos->z * 1.6f)  + half_object_scale_x + padding_x;
					*m_CamOffsetPosY = 0.0f ;
					break;

				case 7:
					*m_CamOffsetPosX = (m_CamPos->z * 1.6f) - half_object_scale_x - padding_x;
					*m_CamOffsetPosY = (m_CamPos->z * 1.0f) - half_object_scale_y - padding_y;
					break;

				case 8:
					*m_CamOffsetPosX = 0.0f ;
					*m_CamOffsetPosY = (m_CamPos->z * 1.0f) - half_object_scale_y - padding_y;
					break;

				case 9:
					*m_CamOffsetPosX = -(m_CamPos->z * 1.6f) + half_object_scale_x + padding_x;
					*m_CamOffsetPosY = (m_CamPos->z * 1.0f) - half_object_scale_y - padding_y;
					break;

				default:
					*m_CamOffsetPosX = 0.0f ;
					*m_CamOffsetPosY = 0.0f ;
					
				break;
			}
		}
		
	}
}

void Camera::OffObjectAnchorPoint()
{
	*m_CamOffsetPosX = 0.0f;
	*m_CamOffsetPosY = 0.0f;
}

void Camera::SetZoom(float z, float zoom_lerp_speed)
{
	if (!cutscene_mode)
	{
		m_CamPos_Dest.z = z;


		if (zoom_lerp_speed > 0.0f)
			lerp_speed_for_zoom = zoom_lerp_speed;

		view_location = false;
		view_object = false;
		correct_position = true;
		zooming = true;
		temp_zooming = false;
		correct_zoom = false;
	}
}

void Camera::SetTempZoom(float z, bool back_to_object, float timer, float zoom_lerp_speed)
{
	if (!cutscene_mode)
	{
		m_CamPos_Origin.z = m_CamPos->z;

		auto_lerp_back_to_original_object = back_to_object;
		view_stay_duration = timer;
		m_CamPos_Dest.z = z;


		if (zoom_lerp_speed > 0.0f)
			lerp_speed_for_zoom = zoom_lerp_speed;

		view_location = false;
		view_object = false;
		correct_position = true;
		zooming = false;
		temp_zooming = true;
		correct_zoom = false;
	}
}

float Camera::GetZoom()const
{
	return m_CamPos->z;
}

void Camera::SetZoomLerpSpeed(float zoom_lerp_speed)
{
	if (zoom_lerp_speed > 0.0f)
		lerp_speed_for_zoom = zoom_lerp_speed;
}

void Camera::SetPosLerpSpeed(float pos_lerp_speed)
{
	if (pos_lerp_speed > 0.0f)
		lerp_speed_for_pos = pos_lerp_speed;
}

void Camera::CutSceneModeStart()
{
	if (!view_location && !view_object && !cam_lock && !temp_zooming && zooming)
	{
		m_CamPos_Origin.x = m_CamPos->x;
		m_CamPos_Origin.y = m_CamPos->y;
		m_CamPos_Origin.z = m_CamPos->z;
		cutscene_move_end = true;
		cutscene_mode = true;
	}
}

void Camera::CutSceneModeMoveLocation(float x, float y, float z, float timer, float pos_lerp_speed , float zoom_lerp_speed )
{
	if (cutscene_mode && cutscene_move_end)
	{
		cutscene_move_end = false;
		m_CamPos_Dest.x = x;
		m_CamPos_Dest.y = y;
		m_CamPos_Dest.z = z;

		if (pos_lerp_speed > 0.0f)
			lerp_speed_for_pos = pos_lerp_speed;

		if (zoom_lerp_speed > 0.0f)
			lerp_speed_for_zoom = zoom_lerp_speed;

		view_stay_duration = timer;

		correct_zoom = false;
		correct_position = false;
		cutscene_move_to_location = true;
	}
}
void Camera::CutSceneModeEnd()
{
	if (cutscene_mode)
	{
		m_CamPos_Dest.x = m_CamPos_Origin.x;
		m_CamPos_Dest.y = m_CamPos_Origin.y;
		m_CamPos_Dest.z = m_CamPos_Origin.z;

		view_stay_duration = 0.0f;

		correct_zoom = false;
		correct_position = false;
		cutscene_move_end = false;
	}
}

bool Camera::GetIsCutSceneStillOnLerp()const
{
	return cutscene_move_end;
}

void Camera::SetOriginZoom(float z)
{
	m_CamPos_Origin.z = z;
}

void Camera::SetViewBackItselfNoLerp()
{
	correct_zoom = true;
	correct_position = true;
	view_location = false;
	view_object = false;
	auto_lerp_back_to_original_object = false;
	zooming = false;
	temp_zooming = false;
	start_countdown = false;
	cam_lock = false;
	view_stay_duration = 0.0f;
}

void Camera::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<Camera>("Camera",
		// Member functions
		"GetPositionVector", &Camera::GetPositionVector,
		"GetMin", &Camera::GetMin,
		"GetMax", &Camera::GetMax,

		"GetSceneActor", &Camera::GetGameObject,
		"SetSelfView", &Camera::SetSelfView,
		"SetCamShake", &Camera::SetCameraShake,
		"SetViewLocation", &Camera::SetViewLocation,
		"SetViewObject", &Camera::SetViewObject,
		"SetSelfView", &Camera::SetSelfView,
		"ViewBackItself", &Camera::ViewBackItself,
		"SetViewLock", &Camera::SetViewLock,
		"SetViewUnLock", &Camera::SetViewUnLock,
		"SetObjectAnchorPoint", &Camera::SetObjectAnchorPoint,
		"OffObjectAnchorPoint", &Camera::OffObjectAnchorPoint,
		"SetTempZoom", &Camera::SetTempZoom,
		"SetZoom", &Camera::SetZoom,
		"GetZoom", &Camera::GetZoom,
		"SetZoomLerpSpeed", &Camera::SetZoomLerpSpeed,
		"lerp_speed_for_pos", &Camera::lerp_speed_for_pos,
		"CutSceneModeStart", &Camera::CutSceneModeStart,
		"CutSceneModeMoveLocation", &Camera::CutSceneModeMoveLocation,
		"CutSceneModeEnd", &Camera::CutSceneModeEnd,
		"GetIsCutSceneStillOnLerp", &Camera::GetIsCutSceneStillOnLerp,
		"SaveCameraOriginPos", &Camera::SaveCameraOriginPos,
		"SetOriginZoom", &Camera::SetOriginZoom,
		"SetViewBackItselfNoLerp", &Camera::SetViewBackItselfNoLerp
		);
}