/******************************************************************************/
/*!
\file   EditorPrefabEditor.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	prefab editor

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EditorPrefabEditor.h"



void EditorPrefabEditor::DrawComponentInfo(GameObject * prefab_obj, Factory * factory)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();


	float bottom_dock_size_y = viewport->Size.y * 0.3f;
	float left_dock_size_x = viewport->Size.x * 0.2f;
	float right_dock_size_x = viewport->Size.x * 0.2f;
	float center_dock_size_x = viewport->Size.x - left_dock_size_x - right_dock_size_x;
	float center_dock_size_y = viewport->Size.y - bottom_dock_size_y;
	UNREFERENCED_PARAMETER(center_dock_size_x);
	UNREFERENCED_PARAMETER(center_dock_size_y);
	float center_dock_pos_x = viewport->Pos.x + left_dock_size_x;
	float center_dock_pos_y = viewport->Pos.y;
	UNREFERENCED_PARAMETER(center_dock_pos_x);
	UNREFERENCED_PARAMETER(center_dock_pos_y);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	ImGui::Begin("PrefabEditor" , NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ center_dock_size_x ,center_dock_size_y }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ center_dock_pos_x , center_dock_pos_y }, ImGuiCond_FirstUseEver);
	ImGuiID dockspace_id = ImGui::GetID("Center Dockspace");
	ImGui::SetNextWindowDockId(dockspace_id);

	
	if (ImGui::CollapsingHeader(prefab_obj->GetIDName().c_str()))
	{
		auto & map = prefab_obj->GetComponents();

		std::unordered_map<ComponentType, Component*>::iterator it = map.begin();
		while (it != map.end())
		{
			std::string componentName = GetComponentName(it->first);
			
			if (ImGui::CollapsingHeader(componentName.c_str()))
			{
				PrintComponent(prefab_obj->GetIDName(), it->second);
				if (ImGui::Button("Delete component"))
				{
					delete it->second;
					it->second = nullptr;
					it = map.erase(it);

					m_console->WriteToConsole(prefab_obj->GetIDName() + "'s " + componentName + " deleted!");

					std::string prefab_archetype = prefab_obj->GetArcheType();

					auto & instmap = factory->GetGameObjectsInstance();
					std::unordered_map<unsigned, GameObject *>::iterator instit = instmap.begin();
					while (instit != instmap.end())
					{
						std::string inst_archetype = instit->second->GetArcheType();
						if (inst_archetype.compare(prefab_archetype) == 0)
						{
							std::string prefab_id_search = prefab_obj->GetIDName();
							unsigned result_id = GetMapKeyByName(factory->GetPrefabs(), prefab_id_search);
							delete instit->second;
							instit->second = new GameObject(*(factory->GetPrefabs().at(result_id)));
						}
						
						++instit;
						
					}
					break;
				}
			}
			++it;	
		}
	}
	
	
	ImGui::Separator();

	ImGui::Text("Add Components");

	const char* items[] = { "TransformComponent", "SpriteComponent", "MovementComponent", "CollisionComponent",
							"RigidBodyComponent", "HpComponent", "AABBComponent", "ControlComponent", "GravityComponent",
							"TriangleComponent", "HotspotComponent" };

	static int item_current = 0;
	ImGui::Combo("Components", &item_current, items, IM_ARRAYSIZE(items));

	ComponentType compType;
	GameObject * gameObj = nullptr;

	if (ImGui::Button("Create Component"))
	{

		std::string compType_s = items[item_current];

		switch (item_current)
		{
		case(0):  compType = ComponentType::TransformComponent; break;
		case(1):  compType = ComponentType::SpriteComponent;	break;
		case(2):  compType = ComponentType::MovementComponent;  break;
		case(3):  compType = ComponentType::CollisionComponent; break;
		case(4):  compType = ComponentType::RigidBodyComponent; break;
		case(5):  compType = ComponentType::HPComponent;		break;
		case(6):  compType = ComponentType::AABBComponent;		break;
		case(7):  compType = ComponentType::ControlComponent;	break;
		case(8):  compType = ComponentType::GravityComponent;	break;
		case(9):  compType = ComponentType::TriangleComponent;	break;
		case(10): compType = ComponentType::HotspotComponent;	break;
		}


		for (auto & obj : factory->GetGameObjectsInstance())
		{
			if (obj.second->GetSelection())
				gameObj = obj.second;
		}

		if (gameObj != nullptr)
		{
			auto components = gameObj->GetComponents();
			if (components.find(compType) == components.end()) //If component exist
			{
				auto factories = factory->GetFactories();
				ComponentFactory * compfac = factories[compType_s];
				Component * comp = compfac->CreateComponent();
				gameObj->AddComponent(comp);
				m_console->WriteToConsole(compType_s + " added to " + gameObj->GetIDName());
			}

			else
				m_console->WriteToConsole(gameObj->GetIDName() + " already has that component!");
		}

	}
	ImGui::End();

}

