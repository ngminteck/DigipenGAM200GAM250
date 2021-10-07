/******************************************************************************/
/*!
\file  EditorObjectSelector.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EditorObjectSelector class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EditorObjectSelector.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

void EditorObjectSelector::Update()
{
	m_profiler->UpdateStart();
	static Point2D o_pos;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 vec = viewport->Size;

	m_mouse = Vector2D{ (ImGui::GetMousePos().x - vec.x / 2) / (vec.y / 2 / m_cam->GetPosition().z) + m_cam->GetPosition().x,
		(vec.y / 2 - ImGui::GetMousePos().y) / (vec.y / 2 / m_cam->GetPosition().z) + m_cam->GetPosition().y };

	auto & gameObjs = m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer();

	if (objselectedTransComp != nullptr)
	{
		if (m_input->InputCheckReleased(VK_LBUTTON))
		{
			m_cmd->PushCommand(new UndoMove{ o_pos, objselectedTransComp->GetPosition(), objselectedTransComp });
			objselectedTransComp = nullptr;
			m_temp = Vector2D{ 0.0f, 0.0f };
		}

		else
		{
			
			if (m_input->InputCheckCurr(VK_LBUTTON) && m_input->InputCheckCurr(VK_CONTROL))
			{
				Vector2D offset = m_mouse - m_temp;
				objselectedTransComp->SetPosition(Vector2D{ (int)(objselectedTransComp->GetPosition().x + offset.x) + 0.5f , (int)(objselectedTransComp->GetPosition().y + offset.y) + 0.5f });
				objselectedAABBComp->SetInit(true);
				m_temp = objselectedTransComp->GetPosition();
				
			}

		
			else if (m_input->InputCheckCurr(VK_LBUTTON))
			{
				Vector2D offset = m_mouse - m_temp;
				objselectedTransComp->SetPosition(Vector2D{ objselectedTransComp->GetPosition().x + offset.x , objselectedTransComp->GetPosition().y + offset.y });
				objselectedAABBComp->SetInit(true);
				m_temp = m_mouse;
			}

		}
	}

	else
	{
		for (auto & elem : gameObjs)
		{
			auto map = elem.second->GetPrefabActor()->GetComponents();
			if (map.find(ComponentType::AABBComponent) != map.end())
			{
				AABBComponent * aabb = nullptr;
				TransformComponent * trans = nullptr;

				if (map.find(ComponentType::AABBComponent) != map.end())
					aabb = dynamic_cast<AABBComponent*>(map[ComponentType::AABBComponent]);

				if (map.find(ComponentType::TransformComponent) != map.end())
					trans = dynamic_cast<TransformComponent*>(map[ComponentType::TransformComponent]);

				if (aabb && trans && CheckCollisionWithMouse(m_mouse, aabb->GetMin(), aabb->GetMax()) && ImGui::GetIO().WantCaptureMouse == false)
				{
					 if (m_input->InputCheckCurr(VK_LBUTTON) && trans != nullptr)
					{
						//Store the current mouse position
						m_temp = m_mouse;

						objselectedTransComp = trans;
						objselectedAABBComp = aabb;
						o_pos = trans->GetPosition();
						auto & cgameObjs = m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer();
						auto  cobjectItr = cgameObjs.begin();
						while (cobjectItr != cgameObjs.end())
						{
							cobjectItr->second->ClearSceneActorSelection();
							++cobjectItr;
						}
						elem.second->SetSceneActorSelection(true);
						break;
					}
				}
			}
		}
	}

	m_profiler->UpdateEnd();
}

EditorObjectSelector::~EditorObjectSelector()
{
	delete m_profiler;
}

bool EditorObjectSelector::CheckCollisionWithMouse(const Vector2D & mouse, const Vector2D & min, const Vector2D & max) const
{
	float mouse_x = mouse.x;
	float mouse_y = mouse.y;

	float min_x = min.x;
	float min_y = min.y;

	float max_x = max.x;
	float max_y = max.y;

	if (mouse_x > min_x && mouse_x < max_x)
	{
		if (mouse_y > min_y && mouse_y < max_y)
		{
			return true;
		}
	}

	return false;
}