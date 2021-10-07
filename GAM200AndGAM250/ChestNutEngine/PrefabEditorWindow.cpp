/******************************************************************************/
/*!
\file   PrefabEditorWindow.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	each prefab will have this imgui window editor to edit the prefab itself

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "PrefabEditorWindow.h"

//Limits for sliders for variables
namespace EOVLimits
{
	const static float SCALE = 10.0f;
	const static float ROTATE_MIN = 0.0f;
	const static float ROTATE_MAX = 360.0f;
	const static float TRANSLATE = 50.0f;
	const static float CAMERA_Z_MIN = 1.0f;
	const static float CAMERA_Z_MAX = 50.0f;
};

namespace EOVCamera //static variables to store original position
{
	static bool x_active = false;
	static bool y_active = false;
	static bool z_active = false;
	static glm::vec3  orig;
}

namespace EOVFloat
{
	static bool float_active = false;
	static float orig;
}
namespace EOVVector
{
	static bool x_active = false;
	static bool y_active = false;
	static bool z_active = false;
	static Vector2D orig2D;
	static Vector3D orig3D;
}

PrefabEditorWindow::PrefabEditorWindow(PrefabActor * prefabactor , ResourceFactoryManager * resourcemanager, CommandManager * command_manager)
	:prefab_actor{ prefabactor }, resource{ resourcemanager }, m_cmd{command_manager}, prefab_editor_window_active{ false }, 
	 prefab_editor_check_change{ false },moving{ false }, deleting{false}
{

}

PrefabEditorWindow::~PrefabEditorWindow()
{
}

void PrefabEditorWindow::SetPrefabActorToNullBeforeDestroyWindow()
{
	resource = nullptr;
	prefab_actor = nullptr;
	m_cmd = nullptr;
}


bool PrefabEditorWindow::GetPrefabEditorWindowActive() const
{
	return prefab_editor_window_active;
}

void PrefabEditorWindow::SetPrefabEditorWindowActive(bool value)
{
	prefab_editor_window_active = value;
}

PrefabActor * PrefabEditorWindow::GetPrefabActorInfor()
{
	return prefab_actor;
}

void PrefabEditorWindow::Draw()
{
	if (prefab_editor_window_active)
	{
		prefab_editor_check_change = true;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		float window_size_x = viewport->Size.x - viewport->Size.x * 0.4f;
		float window_size_y = viewport->Size.y - viewport->Size.y * 0.3f;

		float window_pos_x = viewport->Pos.x + viewport->Size.x * 0.2f;
		float window_pos_y = viewport->Pos.y;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar;
		ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar |  ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::SetNextWindowBgAlpha(1.0f);
		if (ImGui::Begin(prefab_actor->GetPrefabActorName().c_str(), &prefab_editor_window_active, window_flags))
		{
			ImGui::SetWindowSize(ImVec2{ window_size_x, window_size_y }, ImGuiCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2{ window_pos_x , window_pos_y }, ImGuiCond_FirstUseEver);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::MenuItem("Apply All and Update to all SceneActor"))
				{
					resource->GetSceneActorFactory()->UpdateAllSceneActor(prefab_actor);
				}

				if (ImGui::MenuItem("Apply All and Close"))
				{
					resource->GetSceneActorFactory()->UpdateAllSceneActor(prefab_actor);
					prefab_editor_window_active = false;
				}

				if (ImGui::MenuItem("Close without Apply"))
				{
					prefab_editor_window_active = false;
				}

				ImGui::EndMenuBar();
			}
			std::string world_outliner_window_id = "World Outliner##" + prefab_actor->GetPrefabActorName();
			ImGui::BeginChild(world_outliner_window_id.c_str(), { window_size_x *0.3f, window_size_y}, true, child_window_flags);
			PrefabWorldOutliner();
			ImGui::EndChild();

			ImGui::SameLine();

			std::string details_window_id = "Details##" + prefab_actor->GetPrefabActorName();
			ImGui::BeginChild(details_window_id.c_str(), { window_size_x *0.7f, window_size_y }, true, child_window_flags);
			PrefabDetails();
			ImGui::EndChild();

			ImGui::End();
		}

		if (deleting)
		{
			resource->GetPrefabActorFactory()->PrefabActorSelectionDelete(prefab_actor);
			deleting = false;
		}
	}
	else
	{
		if (prefab_editor_check_change)
		{
		
			prefab_actor->SetPrefabActorSelection(false);
			
			prefab_editor_check_change = false;
		}
	}
}

void PrefabEditorWindow::PrefabWorldOutliner()
{
	ImGuiTreeNodeFlags  node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | (prefab_actor->GetPrefabActorSelection() == true ? ImGuiTreeNodeFlags_Selected : 0);
	
	std::string  treenodeex_id = prefab_actor->GetPrefabActorName() + "##" + prefab_actor->GetPrefabActorName();
	bool node_open = ImGui::TreeNodeEx(treenodeex_id.c_str(), node_flags, prefab_actor->GetPrefabActorName().c_str());
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabToScene"))
		{
			unsigned sented_result = *(static_cast<unsigned *>(payload->Data));
			auto sented_prefab_actor =  resource->GetPrefabActorFactory()->GetPrefabActorContainer().find(sented_result)->second;

			if (prefab_actor == sented_prefab_actor)
			{
				std::cout << "you can't add the same prefab actor" << std::endl;
			}
			else
			{
				PrefabActor * new_prefab = new PrefabActor(*sented_prefab_actor);

				auto parent_comp = prefab_actor->GetComponents();
				auto child_comp = new_prefab->GetComponents();
				if (parent_comp.find(ComponentType::TransformComponent) != parent_comp.end() && child_comp.find(ComponentType::TransformComponent) != child_comp.end())
				{
					TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp.find(ComponentType::TransformComponent)->second);
					TransformComponent* trans = dynamic_cast<TransformComponent*>(child_comp.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(parent_trans);
				}
				new_prefab->SetParentPrefabActorPointer(prefab_actor);
				std::cout << new_prefab->GetPrefabActorName() << std::endl;
				prefab_actor->AddChildPrefabActor(new_prefab);
			}
		}
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabMoveActor"))
		{
			// = true;
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::GetIO().KeyShift && ImGui::IsMouseReleased(0))
		{
			
		}
		else if (ImGui::GetIO().KeyCtrl && ImGui::IsMouseReleased(0))
		{
			prefab_actor->SelectWithControlKey();
		}
		else if (ImGui::IsMouseClicked(0))
		{
			prefab_actor->SetPrefabActorSelection(false);
			
			if (prefab_actor->GetPrefabActorSelection())
				prefab_actor->SetPrefabActorSelection(false);
			else
				prefab_actor->SetPrefabActorSelection(true);
		}
	}
	std::string BeginPopupContextItem_label = "Options##" + prefab_actor->GetPrefabActorName();
	if (ImGui::BeginPopupContextItem(BeginPopupContextItem_label.c_str()))
	{
		ImGui::PushItemWidth(-1);
		std::string OpenPopup_label = "Rename##OpenPopup" + prefab_actor->GetPrefabActorName();
		ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
		if (ImGui::MenuItem("Rename"))
		{
			ImGui::OpenPopup(OpenPopup_label.c_str());
		}
		if (ImGui::BeginPopupModal(OpenPopup_label.c_str()))
		{
			std::string new_name = "";
			static char buf2[64] = "";
			ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
			ImGui::PushItemWidth(100);
			ImGui::InputText("##EditorContentBrowser", buf2, 64, text_flag1, NULL, NULL);
			ImGui::PopItemWidth();
			new_name = buf2;
			ImGui::Separator();

			if (ImGui::Button("Apply", ImVec2(120, 0)))
			{
				//resource->GetSceneActorFactory()->SceneActorSelectionRename(new_name);
				new_name = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				new_name = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemFlag();
		ImGui::PopItemWidth();
		ImGui::EndPopup();
	}
	if (node_open)
	{
		ChildPrefabWorldOutliner(prefab_actor->GetChildPrefabActors());
		ImGui::TreePop();
	}
}

void PrefabEditorWindow::ChildPrefabWorldOutliner(std::unordered_map<unsigned, PrefabActor *> child_object)
{
	ImGuiTreeNodeFlags  node_flags = ImGuiTreeNodeFlags_None;

	if (child_object.size() > 0)
	{
		for (auto & elem : child_object)
		{
			node_flags = (elem.second->GetPrefabActorSelection() == true ? ImGuiTreeNodeFlags_Selected : 0);

			if (elem.second->GetChildPrefabActors().size() == 0)
			{
				node_flags |= ImGuiTreeNodeFlags_Leaf;
			}
			else
			{
				node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			}
			bool node_open = ImGui::TreeNodeEx(elem.second->GetPrefabActorName().c_str(), node_flags, elem.second->GetPrefabActorName().c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabToScene"))
				{
					unsigned sented_result = *(static_cast<unsigned *>(payload->Data));
					auto sented_prefab_actor = resource->GetPrefabActorFactory()->GetPrefabActorContainer().find(sented_result)->second;

					if (prefab_actor == sented_prefab_actor)
					{
						std::cout << "you can't add the same prefab actor" << std::endl;
					}
					else
					{
						PrefabActor * new_prefab = new PrefabActor(*sented_prefab_actor);
						auto parent_comp = elem.second->GetComponents();
						auto child_comp = new_prefab->GetComponents();
						if (parent_comp.find(ComponentType::TransformComponent) != parent_comp.end() && child_comp.find(ComponentType::TransformComponent) != child_comp.end())
						{
							TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp.find(ComponentType::TransformComponent)->second);
							TransformComponent* trans = dynamic_cast<TransformComponent*>(child_comp.find(ComponentType::TransformComponent)->second);
							trans->SetParentTransformComponent(parent_trans);
						}
						new_prefab->SetParentPrefabActorPointer(elem.second);
						std::cout << new_prefab->GetPrefabActorName() << std::endl;
						elem.second->AddChildPrefabActor(new_prefab);
					}
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MoveActor"))
				{
					//dest = elem.second;
					//moving = true;
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("MoveActor", NULL, NULL);
				ImGui::Text("Move to...");
				ImGui::EndDragDropSource();
			}
			if (ImGui::IsItemHovered())
			{
				if (ImGui::GetIO().KeyShift && ImGui::IsMouseReleased(0))
				{
					resource->GetPrefabActorFactory()->PrefabActorShiftSelection(elem.second);
				}
				else if (ImGui::GetIO().KeyCtrl && ImGui::IsMouseReleased(0))
				{
					elem.second->SelectWithControlKey();
				}
				else if (ImGui::IsMouseReleased(0))
				{
					prefab_actor->SetPrefabActorSelection(false);

					if (elem.second->GetPrefabActorSelection())
						elem.second->SetPrefabActorSelection(false);
					else
						elem.second->SetPrefabActorSelection(true);
				}

			}
			std::string BeginPopupContextItem_label = "Options##" + elem.second->GetPrefabActorName();
			if (ImGui::BeginPopupContextItem(BeginPopupContextItem_label.c_str()))
			{
				ImGui::PushItemWidth(-1);
				if (ImGui::MenuItem("Duplicate All Selected"))
				{
					//->GetSceneActorFactory()->SceneActorSelectionDuplicate();

				}
				std::string OpenPopup_label = "Rename##OpenPopup" + elem.second->GetPrefabActorName();
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				if (ImGui::MenuItem("Rename All Selected"))
				{
					ImGui::OpenPopup(OpenPopup_label.c_str());
				}
				if (ImGui::BeginPopupModal(OpenPopup_label.c_str()))
				{
					std::string new_name = "";
					static char buf2[64] = "";
					ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
					ImGui::PushItemWidth(100);
					ImGui::InputText("##EditorContentBrowser", buf2, 64, text_flag1, NULL, NULL);
					ImGui::PopItemWidth();
					new_name = buf2;
					ImGui::Separator();

					if (ImGui::Button("Apply", ImVec2(120, 0)))
					{
						//resource->GetSceneActorFactory()->SceneActorSelectionRename(new_name);
						new_name = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0)))
					{
						new_name = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				ImGui::PopItemFlag();
				if (ImGui::MenuItem("Move Selected to this scene actor"))
				{
					//dest = elem.second;
					//moving = true;
				}
				if (ImGui::MenuItem("Delete All Selected"))
				{
					deleting = true;
				}
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			if (node_open)
			{
				ChildPrefabWorldOutliner(elem.second->GetChildPrefabActors());
				ImGui::TreePop();
			}
		}
	}
}

void PrefabEditorWindow::PrefabDetails()
{
	if (prefab_actor->GetPrefabActorSelection())
	{
		std::string CollapsingHeader_label = prefab_actor->GetPrefabActorName() + "##" + prefab_actor->GetPrefabActorName();
		if (ImGui::CollapsingHeader(CollapsingHeader_label.c_str()))
		{
			ImGui::Text("Name: ");
			ImGui::SameLine();
			ImGui::Text(prefab_actor->GetPrefabActorName().c_str());
			std::string change_name_id = "Change Name##" + prefab_actor->GetPrefabActorName();
			ImGui::SameLine();
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			if (ImGui::Button(change_name_id.c_str()))
			{
				ImGui::OpenPopup(change_name_id.c_str());
			}
			if (ImGui::BeginPopupModal(change_name_id.c_str()))
			{
				std::string new_name = "";
				static char buf2[64] = "";
				ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
				ImGui::Text("New Name:");
				ImGui::SameLine();
				ImGui::PushItemWidth(100);
				std::string change_name_input_id = "##changename" + prefab_actor->GetPrefabActorName();
				ImGui::InputText(change_name_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
				ImGui::PopItemWidth();
				new_name = buf2;
				ImGui::Separator();
				std::string change_name_apply_id = "Apply##changename" + prefab_actor->GetPrefabActorName();
				if (ImGui::Button(change_name_apply_id.c_str(), ImVec2(120, 0)))
				{
					std::string old_path = prefab_actor->GetPath();
					prefab_actor->SetPrefabActorName(&(resource->GetPrefabActorFactory()->GetPrefabActorContainer()), new_name);
					resource->GetPrefabActorFactory()->SavePrefabActor(prefab_actor);
					resource->GetSceneActorFactory()->UpdateSceneActorCriticalInfo(old_path, prefab_actor);
					resource->GetPrefabActorFactory()->DeletePrefabFile(old_path);
					new_name = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				std::string change_name_cancel_id = "Cancel##changename" + prefab_actor->GetPrefabActorName();
				if (ImGui::Button(change_name_cancel_id.c_str(), ImVec2(120, 0)))
				{
					new_name = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
	
			ImGui::Text("Object Type: ");
			ImGui::SameLine();
			std::string object_type_id = "##objecttype" + prefab_actor->GetPath();
			if (ImGui::BeginCombo(object_type_id.c_str(),prefab_actor->GetObjectType().c_str()))
			{
				auto object_type_list = resource->GetPrefabActorFactory()->GetObjectTypeContainer();

				for (auto & elem : object_type_list)
				{
					if (ImGui::MenuItem(elem.second.c_str()))
					{
						prefab_actor->SetObjectType(elem.second.c_str());
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Text("Active: ");
			ImGui::SameLine();
			std::string active_id = "##active" + prefab_actor->GetPrefabActorName();
			ImGui::Checkbox(active_id.c_str(), prefab_actor->GetActivePtr());
			ImGui::Text("Tagging");
			//bool delete_tagging = false;
			//std::string delete_key = "";
			for (auto it = prefab_actor->GetPrefabActorTagging().begin() ; it != prefab_actor->GetPrefabActorTagging().end(); )
			{
				bool erase_it = false;
				if (it->first.compare("object_type_key") == 0 || (it->first.compare("prefab_name_key") == 0))
				{
					++it;
				}
				else
				{
					ImGui::Text("Key:");
					ImGui::SameLine();
					ImGui::Text(it->first.c_str());
					ImGui::SameLine();
					std::string change_key_id = "Change Key##" + it->first + prefab_actor->GetPrefabActorName();
					if (ImGui::Button(change_key_id.c_str(), ImVec2{ 120,20 }))
					{
						ImGui::OpenPopup(change_key_id.c_str());
					}
					if (ImGui::BeginPopupModal(change_key_id.c_str()))
					{
						std::string new_key = "";
						static char buf2[64] = "";
						ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
						ImGui::Text("New Key:");

						ImGui::PushItemWidth(100);
						std::string change_key_input_id = "##changekey" + it->first + prefab_actor->GetPrefabActorName();
						ImGui::InputText(change_key_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
						ImGui::PopItemWidth();
						new_key = buf2;
						ImGui::Separator();
						std::string change_key_apply_id = "Apply##" + it->first + prefab_actor->GetPrefabActorName();
						if (ImGui::Button(change_key_apply_id.c_str(), ImVec2(120, 0)))
						{
							bool result = prefab_actor->ChangeExistedPrefabTaggingKey(it->first, new_key);
							if (result)
							{
								erase_it = true;
								new_key = "";
								ImGui::CloseCurrentPopup();
							}
							else
							{
								new_key = "Key already existed.";
							}
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						std::string change_key_cancel_id = "Cancel##c" + it->first + prefab_actor->GetPrefabActorName();
						if (ImGui::Button(change_key_cancel_id.c_str(), ImVec2(120, 0)))
						{
							new_key = "";
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::Text("Value:");
					ImGui::SameLine();
					ImGui::Text(it->second.c_str());
					ImGui::SameLine();
					std::string change_value_id = "Change value##" + it->second + prefab_actor->GetPrefabActorName();
					if (ImGui::Button(change_value_id.c_str(), ImVec2{ 120,20 }))
					{
						ImGui::OpenPopup(change_value_id.c_str());
					}
					if (ImGui::BeginPopupModal(change_value_id.c_str()))
					{
						std::string new_value = "";
						static char buf2[64] = "";
						ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
						ImGui::Text("New Value:");
						ImGui::SameLine();
						ImGui::PushItemWidth(100);
						std::string change_value_input_id = "##changevalue" + it->second + prefab_actor->GetPrefabActorName();
						ImGui::InputText(change_value_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
						ImGui::PopItemWidth();
						new_value = buf2;
						ImGui::Separator();
						std::string change_value_apply_id = "Apply##" + it->second + prefab_actor->GetPrefabActorName();
						if (ImGui::Button(change_value_apply_id.c_str(), ImVec2(120, 0)))
						{
							bool result = prefab_actor->ChangeExistedPrefabTaggingValue(it->first, new_value);
							if (result)
							{
								new_value = "";
								ImGui::CloseCurrentPopup();
							}
							else
							{
								new_value = "Key already existed.";
							}
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						std::string change_key_cancel_id = "Cancel##c" + it->second + prefab_actor->GetPrefabActorName();
						if (ImGui::Button(change_key_cancel_id.c_str(), ImVec2(120, 0)))
						{
							new_value = "";
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					std::string remove_tagging_id = "Remove##" + prefab_actor->GetPrefabActorName();
					if (ImGui::Button(remove_tagging_id.c_str(), ImVec2{ 120,20 }))
					{
						erase_it = true;
					}

					if (erase_it)
					{
						it = prefab_actor->GetPrefabActorTagging().erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			std::string add_tagging_id = "Add New Tag##" + prefab_actor->GetPrefabActorName();
			if (ImGui::Button(add_tagging_id.c_str(), ImVec2{ 120,20 }))
			{
				ImGui::OpenPopup(add_tagging_id.c_str());
			}
			if (ImGui::BeginPopupModal(add_tagging_id.c_str()))
			{
				std::string new_key = "";
				std::string new_value = "";
				static char buf1[64] = "";
				static char buf2[64] = "";
				ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
				ImGui::Text("Key:");
				ImGui::SameLine();
				ImGui::PushItemWidth(100);
				std::string add_key_input_id = "##addkey" + prefab_actor->GetPrefabActorName();
				ImGui::InputText(add_key_input_id.c_str(), buf1, 64, text_flag1, NULL, NULL);
				ImGui::PopItemWidth();
				new_key = buf1;
				ImGui::SameLine();
				ImGui::Text("Value:");
				ImGui::SameLine();
				ImGui::PushItemWidth(100);
				std::string add_value_input_id = "##addvalue" + prefab_actor->GetPrefabActorName();
				ImGui::InputText(add_value_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
				ImGui::PopItemWidth();
				new_value = buf2;

				ImGui::Separator();
				std::string add_tagging_apply_id = "Add New Tagging##" + prefab_actor->GetPrefabActorName();
				if (ImGui::Button(add_tagging_apply_id.c_str(), ImVec2(120, 0)))
				{
					prefab_actor->SetNewPrefabActorTagging(new_key, new_value);
					new_key = "";
					new_value = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				std::string add_tagging_cancel_id = "Cancel##c" + prefab_actor->GetPrefabActorName();
				if (ImGui::Button(add_tagging_cancel_id.c_str(), ImVec2(120, 0)))
				{
					new_key = "";
					new_value = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopItemFlag();
			
			ImGui::Text("Component List: ");
			ImGui::SameLine();
			auto & map = prefab_actor->GetComponents();
			auto & component_list = resource->GetPrefabActorFactory()->GetComponentTypeContainer();
			std::string addcomp_id = "##addcomp" + prefab_actor->GetPath();
			if (ImGui::BeginCombo(addcomp_id.c_str(), "Select Component To Add"))
			{
				for (auto & elem : component_list)
				{
					if (map.find(elem.first) == map.end())
					{
						if (ImGui::MenuItem(elem.second.c_str()))
						{
							auto factories = resource->GetPrefabActorFactory()->GetFactories();
							ComponentFactory * compfac = factories[elem.second.c_str()];
							Component * comp = compfac->CreateComponent();
							prefab_actor->AddComponent(comp);
						}
					}
				}
				ImGui::EndCombo();
			}

			for (auto it = map.begin(); it != map.end();)
			{
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
				std::string  treenodeex_id  = prefab_actor->GetPrefabActorName() + component_list.find(it->first)->second;
				bool node_open = ImGui::TreeNodeEx(treenodeex_id.c_str(), node_flags, component_list.find(it->first)->second.c_str());
				if (node_open)
				{
					PrintComponent(prefab_actor->GetPrefabActorName(), it->second);
					if (component_list.find(it->first)->second.compare("TransformComponent") != 0)
					{
						std::string delete_button_id = "Delete component##" + prefab_actor->GetPrefabActorName() + component_list.find(it->first)->second;
						if (ImGui::Button(delete_button_id.c_str()))
						{
							delete it->second;
							it->second = nullptr;
							it = map.erase(it);
						}
						else
						{
							++it;
						}
					}
					else
					{
						++it;
					}
					ImGui::TreePop();
				}
				else
				{
					++it;
				}
			}

		}
	}

	ChildPrefabDetails(prefab_actor->GetChildPrefabActors());


}

void PrefabEditorWindow::ChildPrefabDetails(std::unordered_map<unsigned, PrefabActor *> & child_object)
{
	for (auto it2 = child_object.begin(); it2 != child_object.end();)
	{
		if (it2->second->GetPrefabActorSelection())
		{
			std::string CollapsingHeader_label = it2->second->GetPrefabActorName() + "##" + it2->second->GetPrefabActorName();
			if (ImGui::CollapsingHeader(CollapsingHeader_label.c_str()))
			{
				ImGui::Text("Name: ");
				ImGui::SameLine();
				ImGui::Text(it2->second->GetPrefabActorName().c_str());
				std::string change_name_id = "Change Name##" + it2->second->GetPrefabActorName();
				ImGui::SameLine();
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				if (ImGui::Button(change_name_id.c_str()))
				{
					ImGui::OpenPopup(change_name_id.c_str());
				}
				if (ImGui::BeginPopupModal(change_name_id.c_str()))
				{
					std::string new_name = "";
					static char buf2[64] = "";
					ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
					ImGui::Text("New Name:");
					ImGui::SameLine();
					ImGui::PushItemWidth(100);
					std::string change_name_input_id = "##changename" + it2->second->GetPrefabActorName();
					ImGui::InputText(change_name_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
					ImGui::PopItemWidth();
					new_name = buf2;
					ImGui::Separator();
					std::string change_name_apply_id = "Apply##changename" + it2->second->GetPrefabActorName();
					if (ImGui::Button(change_name_apply_id.c_str(), ImVec2(120, 0)))
					{
						std::string old_path = it2->second->GetPath();
						it2->second->SetPrefabActorName(&(resource->GetPrefabActorFactory()->GetPrefabActorContainer()), new_name);
						resource->GetPrefabActorFactory()->SavePrefabActor(it2->second);
						resource->GetSceneActorFactory()->UpdateSceneActorCriticalInfo(old_path, it2->second);
						resource->GetPrefabActorFactory()->DeletePrefabFile(old_path);
						new_name = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					std::string change_name_cancel_id = "Cancel##changename" + it2->second->GetPrefabActorName();
					if (ImGui::Button(change_name_cancel_id.c_str(), ImVec2(120, 0)))
					{
						new_name = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				ImGui::Text("Object Type: ");
				ImGui::SameLine();
				std::string object_type_id = "##objecttype" + it2->second->GetPath();
				if (ImGui::BeginCombo(object_type_id.c_str(), it2->second->GetObjectType().c_str()))
				{
					auto object_type_list = resource->GetPrefabActorFactory()->GetObjectTypeContainer();

					for (auto & elem : object_type_list)
					{
						if (ImGui::MenuItem(elem.second.c_str()))
						{
							it2->second->SetObjectType(elem.second.c_str());
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Text("Active: ");
				ImGui::SameLine();
				std::string active_id = "##active" + it2->second->GetPrefabActorName();
				ImGui::Checkbox(active_id.c_str(), it2->second->GetActivePtr());
				ImGui::Text("Tagging");
				//bool delete_tagging = false;
				//std::string delete_key = "";
				for (auto it = it2->second->GetPrefabActorTagging().begin(); it != it2->second->GetPrefabActorTagging().end(); )
				{
					bool erase_it = false;
					if (it->first.compare("object_type_key") == 0 || (it->first.compare("prefab_name_key") == 0))
					{
						++it;
					}
					else
					{
						ImGui::Text("Key:");
						ImGui::SameLine();
						ImGui::Text(it->first.c_str());
						ImGui::SameLine();
						std::string change_key_id = "Change Key##" + it->first + it2->second->GetPrefabActorName();
						if (ImGui::Button(change_key_id.c_str(), ImVec2{ 120,20 }))
						{
							ImGui::OpenPopup(change_key_id.c_str());
						}
						if (ImGui::BeginPopupModal(change_key_id.c_str()))
						{
							std::string new_key = "";
							static char buf2[64] = "";
							ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
							ImGui::Text("New Key:");

							ImGui::PushItemWidth(100);
							std::string change_key_input_id = "##changekey" + it->first + it2->second->GetPrefabActorName();
							ImGui::InputText(change_key_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
							ImGui::PopItemWidth();
							new_key = buf2;
							ImGui::Separator();
							std::string change_key_apply_id = "Apply##" + it->first + it2->second->GetPrefabActorName();
							if (ImGui::Button(change_key_apply_id.c_str(), ImVec2(120, 0)))
							{
								bool result = it2->second->ChangeExistedPrefabTaggingKey(it->first, new_key);
								if (result)
								{
									erase_it = true;
									new_key = "";
									ImGui::CloseCurrentPopup();
								}
								else
								{
									new_key = "Key already existed.";
								}
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							std::string change_key_cancel_id = "Cancel##c" + it->first + it2->second->GetPrefabActorName();
							if (ImGui::Button(change_key_cancel_id.c_str(), ImVec2(120, 0)))
							{
								new_key = "";
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text("Value:");
						ImGui::SameLine();
						ImGui::Text(it->second.c_str());
						ImGui::SameLine();
						std::string change_value_id = "Change value##" + it->second + it2->second->GetPrefabActorName();
						if (ImGui::Button(change_value_id.c_str(), ImVec2{ 120,20 }))
						{
							ImGui::OpenPopup(change_value_id.c_str());
						}
						if (ImGui::BeginPopupModal(change_value_id.c_str()))
						{
							std::string new_value = "";
							static char buf2[64] = "";
							ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
							ImGui::Text("New Value:");
							ImGui::SameLine();
							ImGui::PushItemWidth(100);
							std::string change_value_input_id = "##changevalue" + it->second + it2->second->GetPrefabActorName();
							ImGui::InputText(change_value_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
							ImGui::PopItemWidth();
							new_value = buf2;
							ImGui::Separator();
							std::string change_value_apply_id = "Apply##" + it->second + it2->second->GetPrefabActorName();
							if (ImGui::Button(change_value_apply_id.c_str(), ImVec2(120, 0)))
							{
								bool result = it2->second->ChangeExistedPrefabTaggingValue(it->first, new_value);
								if (result)
								{
									new_value = "";
									ImGui::CloseCurrentPopup();
								}
								else
								{
									new_value = "Key already existed.";
								}
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							std::string change_key_cancel_id = "Cancel##c" + it->second + it2->second->GetPrefabActorName();
							if (ImGui::Button(change_key_cancel_id.c_str(), ImVec2(120, 0)))
							{
								new_value = "";
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						std::string remove_tagging_id = "Remove##" + it2->second->GetPrefabActorName();
						if (ImGui::Button(remove_tagging_id.c_str(), ImVec2{ 120,20 }))
						{
							erase_it = true;
						}

						if (erase_it)
						{
							it = it2->second->GetPrefabActorTagging().erase(it);
						}
						else
						{
							++it;
						}
					}
				}

				std::string add_tagging_id = "Add New Tag##" + it2->second->GetPrefabActorName();
				if (ImGui::Button(add_tagging_id.c_str(), ImVec2{ 120,20 }))
				{
					ImGui::OpenPopup(add_tagging_id.c_str());
				}
				if (ImGui::BeginPopupModal(add_tagging_id.c_str()))
				{
					std::string new_key = "";
					std::string new_value = "";
					static char buf1[64] = "";
					static char buf2[64] = "";
					ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
					ImGui::Text("Key:");
					ImGui::SameLine();
					ImGui::PushItemWidth(100);
					std::string add_key_input_id = "##addkey" + it2->second->GetPrefabActorName();
					ImGui::InputText(add_key_input_id.c_str(), buf1, 64, text_flag1, NULL, NULL);
					ImGui::PopItemWidth();
					new_key = buf1;
					ImGui::SameLine();
					ImGui::Text("Value:");
					ImGui::SameLine();
					ImGui::PushItemWidth(100);
					std::string add_value_input_id = "##addvalue" + it2->second->GetPrefabActorName();
					ImGui::InputText(add_value_input_id.c_str(), buf2, 64, text_flag1, NULL, NULL);
					ImGui::PopItemWidth();
					new_value = buf2;

					ImGui::Separator();
					std::string add_tagging_apply_id = "Add New Tagging##" + it2->second->GetPrefabActorName();
					if (ImGui::Button(add_tagging_apply_id.c_str(), ImVec2(120, 0)))
					{
						it2->second->SetNewPrefabActorTagging(new_key, new_value);
						new_key = "";
						new_value = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					std::string add_tagging_cancel_id = "Cancel##c" + it2->second->GetPrefabActorName();
					if (ImGui::Button(add_tagging_cancel_id.c_str(), ImVec2(120, 0)))
					{
						new_key = "";
						new_value = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				ImGui::PopItemFlag();

				ImGui::Text("Component List: ");
				ImGui::SameLine();
				auto & map = it2->second->GetComponents();
				auto & component_list = resource->GetPrefabActorFactory()->GetComponentTypeContainer();
				std::string addcomp_id = "##addcomp" + it2->second->GetPath();
				if (ImGui::BeginCombo(addcomp_id.c_str(), "Select Component To Add"))
				{
					for (auto & elem : component_list)
					{
						if (map.find(elem.first) == map.end())
						{
							if (ImGui::MenuItem(elem.second.c_str()))
							{
								auto factories = resource->GetPrefabActorFactory()->GetFactories();
								ComponentFactory * compfac = factories[elem.second.c_str()];
								Component * comp = compfac->CreateComponent();
								it2->second->AddComponent(comp);
							}
						}
					}
					ImGui::EndCombo();
				}

				for (auto it = map.begin(); it != map.end();)
				{
					ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
					std::string  treenodeex_id = it2->second->GetPrefabActorName() + component_list.find(it->first)->second;
					bool node_open = ImGui::TreeNodeEx(treenodeex_id.c_str(), node_flags, component_list.find(it->first)->second.c_str());
					if (node_open)
					{
						PrintComponent(it2->second->GetPrefabActorName(), it->second);
						if (component_list.find(it->first)->second.compare("TransformComponent") != 0)
						{
							std::string delete_button_id = "Delete component##" + it2->second->GetPrefabActorName() + component_list.find(it->first)->second;
							if (ImGui::Button(delete_button_id.c_str()))
							{
								delete it->second;
								it->second = nullptr;
								it = map.erase(it);
							}
							else
							{
								++it;
							}
						}
						else
						{
							++it;
						}
						ImGui::TreePop();
					}
					else
					{
						++it;
					}
				}
			}
			ChildPrefabDetails(it2->second->GetChildPrefabActors());
			++it2;
		}
		else
		{
			ChildPrefabDetails(it2->second->GetChildPrefabActors());
			++it2;
		}
	}
}

void PrefabEditorWindow::PrintComponent(const std::string & label, Component * comp)
{
	switch (comp->GetComponentType())
	{
		case(ComponentType::StatsComponent):	  PrintComponentData<StatsComponent>(label, comp); break;
		case(ComponentType::CollisionComponent):  PrintComponentData<CollisionComponent>(label, comp); break;
		case(ComponentType::AABBComponent):		  PrintComponentData<AABBComponent>(label, comp); break;
		case(ComponentType::RigidBodyComponent):  PrintComponentData<RigidBodyComponent>(label, comp); break;
		case(ComponentType::MovementComponent):   PrintComponentData<MovementComponent>(label, comp); break;
		case(ComponentType::TransformComponent):  PrintComponentData<TransformComponent>(label, comp); break;
		case(ComponentType::SpriteComponent):	  PrintSpriteComponentData(label, comp); break;
		case(ComponentType::ScriptComponent):	  PrintScriptComponentData(label, comp); break;
		case(ComponentType::AudioComponent):	  PrintComponentData<AudioComponent>(label, comp); break;
		case(ComponentType::ButtonComponent):	  PrintButtonComponentData(label, comp); break;
		case(ComponentType::CameraComponent):	  PrintCameraComponentData(label, comp); break;
	default: break;
	}
}


template<typename T>
void PrefabEditorWindow::PrintComponentData(const std::string & label, Component * comp)
{
	T *  component = static_cast<T*>(comp);
	auto map = component->metadata.reflect<TypeErasedObj>(*component);

	for (auto itor = map.begin(); itor != map.end(); ++itor)
	{
		PrintComponentDataInterface(label, itor->first, itor->second);
	}
}

void PrefabEditorWindow::PrintButtonComponentData(const std::string & label, Component * comp)
{
	ButtonComponent * button = dynamic_cast<ButtonComponent *>(comp);

	ImGui::TextColored({ 0,10,255, 1 }, "Active Texture:"); ImGui::SameLine();

	if (ImGui::BeginCombo(("##" + label + "active").c_str(), button->GetActiveTexName().c_str()))
	{
		auto spritesheet = resource->GetSpriteSheetFactory()->GetContainer();
		for (auto itor = spritesheet->begin(); itor != spritesheet->end(); itor++)
		{
			std::string spriteName = itor->second->GetTextureName();

			if (ImGui::MenuItem(spriteName.c_str()))
			{
				button->SetActiveTexName(spriteName);
				button->SetActiveTex(itor->second);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::TextColored({ 0,10,255, 1 }, "Inactive Texture: "); ImGui::SameLine();
	if (ImGui::BeginCombo(("##" + label + "inactive").c_str(), button->GetInactiveTexName().c_str()))
	{
		auto spritesheet = resource->GetSpriteSheetFactory()->GetContainer();
		for (auto itor = spritesheet->begin(); itor != spritesheet->end(); itor++)
		{
			std::string spriteName = itor->second->GetTextureName();

			if (ImGui::MenuItem(spriteName.c_str()))
			{
				button->SetInactiveTexName(spriteName);
				button->SetInactiveTex(itor->second);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::TextColored({ 0,10,255, 1 }, "Gamestate Path: "); ImGui::SameLine();
	ImGui::Text("Path:  "); ImGui::SameLine();
	if (ImGui::BeginCombo(("##" + label + "path").c_str(), button->GetGameStatePath().c_str()))
	{
		
		for (auto & p : std::filesystem::directory_iterator{ "Resource/Json/Level" })
		{
			if (p.path().extension() == ".json")
			{
				if (ImGui::MenuItem(p.path().string().c_str()))
					button->SetGameStatePath(p.path().string());
			}
		}

		ImGui::EndCombo();
	}


	ImGui::TextColored({ 0,10,255, 1 }, "Trigger type: "); ImGui::SameLine();
	ImGui::InputInt(("##" + label + "trigger type").c_str(), reinterpret_cast<int *>(&button->m_type));

	ImGui::TextColored({ 0,10,255, 1 }, "Section ID: "); ImGui::SameLine();
	ImGui::InputInt(("##" + label + "sectionID").c_str(), reinterpret_cast<int *>(&button->m_sectionID));

	ImGui::TextColored({ 0,10,255, 1 }, "Button ID: "); ImGui::SameLine();
	ImGui::InputInt(("##" + label + "buttonID").c_str(), reinterpret_cast<int *>(&button->m_buttonID));

	ImGui::TextColored({ 0,10,255, 1 }, "Menu ID: "); ImGui::SameLine();
	ImGui::InputInt(("##" + label + "Menu ID").c_str(), reinterpret_cast<int *>(&button->m_menuID));

	ImGui::TextColored({ 0,10,255, 1 }, "Next Menu: "); ImGui::SameLine();
	ImGui::InputInt(("##" + label + "Next Menu").c_str(), reinterpret_cast<int *>(&button->m_next));

	ImGui::TextColored({ 0,10,255, 1 }, "is Menu: "); ImGui::SameLine();
	ImGui::Checkbox(("##" + label + "is Menu").c_str(), reinterpret_cast<bool *>(&button->m_isMenu));

	ImGui::TextColored({ 0,10,255, 1 }, "Icon type: "); ImGui::SameLine();
	ImGui::InputInt(("##" + label + "Icon type").c_str(), reinterpret_cast<int *>(&button->m_icon));

}

void PrefabEditorWindow::PrintSpriteComponentData(const std::string & label, Component * comp)
{
	SpriteComponent *  sprite = static_cast<SpriteComponent*>(comp);
	ImGui::Text("Is Animated");
	ImGui::SameLine();
	ImGui::Checkbox("##sca", sprite->GetAnimatedTexturePtr());

	sprite->CheckBoolAnimatedTextureChange();
	std::string spritename_label_id = "";
	spritename_label_id = "##spritename" + label;

	if (*sprite->GetAnimatedTexturePtr() == true)
	{
		ImGui::Text("Sprite");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ApplySprite"))
			{
				SpriteSheetstring spritesheetdata = *(static_cast<SpriteSheetstring *>(payload->Data));
				sprite->SetSpriteName(spritesheetdata.sprite_name);
				sprite->SetTextureName(spritesheetdata.texture_name);
				unsigned key = GetSpriteKeyBySpriteName(resource->GetSpriteSheetFactory()->GetContainer(), spritesheetdata.sprite_name);
				auto tex = resource->GetSpriteSheetFactory()->GetContainer()->find(key)->second;
				sprite->SetTexture(tex);
				sprite->SetSprite(spritesheetdata.texture_name);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo(spritename_label_id.c_str(), sprite->GetSpriteName().c_str()))
		{
			for (auto it = resource->GetSpriteSheetFactory()->GetContainer()->begin(); it != resource->GetSpriteSheetFactory()->GetContainer()->end(); ++it)
			{
				if (*sprite->GetAnimatedTexturePtr() == it->second->GetIsAnimatedTexture())
				{
					if (ImGui::BeginMenu(it->second->GetTextureName().c_str()))
					{
						std::string spritesheet_name = it->second->GetTextureName();
						auto tex = it->second->GetAnimationListContainer();
						for (auto it_text = tex.begin(); it_text != tex.end(); ++it_text)
						{
							if (ImGui::MenuItem(it_text->first.c_str()))
							{
								std::string texture_name = it_text->first;
								sprite->SetSpriteName(spritesheet_name);
								sprite->SetTextureName(texture_name);
								sprite->SetTexture(it->second);
								sprite->SetSprite(texture_name);
								//sprite->SetMesh(texture_name);
							}
						}
						ImGui::EndMenu();
					}
				}
			}
			ImGui::EndCombo();
		}
	}
	else
	{
		ImGui::Text("Texture");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ApplyTexture"))
			{
				SpriteSheetstring spritesheetdata = *(static_cast<SpriteSheetstring *>(payload->Data));
				sprite->SetSpriteName(spritesheetdata.sprite_name);
				sprite->SetTextureName(spritesheetdata.texture_name);
				unsigned key = GetSpriteKeyBySpriteName(resource->GetSpriteSheetFactory()->GetContainer(), spritesheetdata.sprite_name);
				auto tex = resource->GetSpriteSheetFactory()->GetContainer()->find(key)->second;
				sprite->SetTexture(tex);
				sprite->SetSprite(spritesheetdata.texture_name);
				//sprite->SetMesh(spritesheetdata.texture_name);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo(spritename_label_id.c_str(), sprite->GetSpriteName().c_str()))
		{
			for (auto it = resource->GetSpriteSheetFactory()->GetContainer()->begin(); it != resource->GetSpriteSheetFactory()->GetContainer()->end(); ++it)
			{
				if (*sprite->GetAnimatedTexturePtr() == it->second->GetIsAnimatedTexture())
				{
					std::string spritesheet_name = it->second->GetTextureName();
					auto tex = it->second->GetAnimationListContainer();

					for (auto it_text = tex.begin(); it_text != tex.end(); ++it_text)
					{
						if (ImGui::MenuItem(it_text->first.c_str()))
						{
							std::string texture_name = it_text->first;
							sprite->SetSpriteName(spritesheet_name);
							sprite->SetTextureName(texture_name);
							sprite->SetTexture(it->second);
							sprite->SetSprite(texture_name);
							//sprite->SetMesh(texture_name);
						}
					}
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::TextColored({ 0,10,255, 1 }, "Layer: "); ImGui::SameLine();
	ImGui::InputInt(("##" + spritename_label_id + "layer").c_str(), (int*)sprite->GetLayerPtr());

	ImGui::TextColored({ 0,10,255, 1 }, "Orthogonal: "); ImGui::SameLine();
	ImGui::Checkbox(("##" + spritename_label_id + "orthogonal").c_str(), &sprite->m_OrthogonalCamera);
}


void PrefabEditorWindow::PrintScriptComponentData(const std::string & label, Component * comp)
{
	ScriptComponent * script = dynamic_cast<ScriptComponent *>(comp);
	std::string inputLabel = "##" + label + "ScriptComponent";
	static std::string stateString = "";
	static std::string scriptPath = "";

	ImGui::TextColored({ 0,10,255, 1 }, "Script file: ");
	ImGui::Text("State"); ImGui::SameLine(); ImGui::Text("Script Path");

	for (auto & elem : *(script->GetScriptList()))
	{
		ImGui::Text(elem.first.c_str());  ImGui::SameLine(); ImGui::Text(elem.second->GetPath().c_str());
	}

	ImGui::Text("State"); ImGui::SameLine(); ImGui::InputText("##State", &stateString);
	if (ImGui::BeginCombo((inputLabel).c_str(), scriptPath.c_str()))
	{
		
		for (auto & p : std::filesystem::directory_iterator{ "Resource/Scripts" })
		{
			if (p.path().extension() == ".lua")
			{
				if (ImGui::MenuItem(p.path().string().c_str()))
					scriptPath = p.path().string();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Add Script"))
	{
		script->AddScript(stateString, scriptPath);
	}
}

void PrefabEditorWindow::PrintCameraComponentData(const std::string & label, Component * comp)
{
	std::string do_nothing = label;
	do_nothing = "";
	CameraComponent * cam_comp = dynamic_cast<CameraComponent *>(comp);

	ImGui::Text("cam_pos");
	ImGui::Text("x:");
	ImGui::SameLine();
	PrintFloat(("##cam_pos_x"), &cam_comp->GetCamPos()->x, INPUTBOX);
	ImGui::Text("y:");
	ImGui::SameLine();
	PrintFloat(("##cam_pos_y"), &cam_comp->GetCamPos()->y, INPUTBOX);
	ImGui::Text("z:");
	ImGui::SameLine();
	PrintFloat(("##cam_pos_z"), &cam_comp->GetCamPos()->z, INPUTBOX);

	ImGui::Text("cam_front");
	ImGui::Text("x:");
	ImGui::SameLine();
	PrintFloat(("##cam_front_x"), &cam_comp->GetCamFront()->x, INPUTBOX);
	ImGui::Text("y:");
	ImGui::SameLine();
	PrintFloat(("##cam_front_y"), &cam_comp->GetCamFront()->y, INPUTBOX);
	ImGui::Text("z:");
	ImGui::SameLine();
	PrintFloat(("##cam_front_z"), &cam_comp->GetCamFront()->z, INPUTBOX);

	ImGui::Text("cam_up");
	ImGui::Text("x:");
	ImGui::SameLine();
	PrintFloat(("##cam_up_x"), &cam_comp->GetCamUp()->x, INPUTBOX);
	ImGui::Text("y:");
	ImGui::SameLine();
	PrintFloat(("##cam_up_y"), &cam_comp->GetCamUp()->y, INPUTBOX);
	ImGui::Text("z:");
	ImGui::SameLine();
	PrintFloat(("##cam_up_z"), &cam_comp->GetCamUp()->z, INPUTBOX);

	ImGui::Text("cam_pos_offset");
	ImGui::Text("x:");
	ImGui::SameLine();
	PrintFloat(("##cam_pos_offset_x"), cam_comp->GetPosOffsetX(), INPUTBOX);
	ImGui::Text("y:");
	ImGui::SameLine();
	PrintFloat(("##cam_pos_offset_y"), cam_comp->GetPosOffsetY(), INPUTBOX);
}

void PrefabEditorWindow::PrintComponentDataInterface(const std::string & label, const std::string & variable, TypeErasedObj & variable_type)
{
	std::any obj = variable_type.GetValue();
	const type_info& objtype = obj.type();
	std::string inputLabel = "##" + label + variable;

	if (objtype == typeid(int *))
	{
		int * value = std::any_cast<int *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, (variable + ": ").c_str());
		PrintInt(inputLabel, value, INPUTBOX);
	}
	else if (objtype == typeid(Direction*))
	{
		int * value = (int*)(std::any_cast<Direction*>(obj));
		ImGui::TextColored({ 0,10,255, 1 }, (variable + ": ").c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Text(inputLabel.c_str(), value);
		ImGui::PopItemWidth();
	}


	else if (objtype == typeid(unsigned int *))
	{
		int * value = (int*)std::any_cast<unsigned int *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, (variable + ": ").c_str());
		ImGui::PushItemWidth(100);
		ImGui::InputInt(inputLabel.c_str(), value);
		ImGui::PopItemWidth();
	}

	else if (objtype == typeid(float *))
	{
		float * value = std::any_cast<float *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, variable.c_str()); ImGui::SameLine();
		PrintFloat(inputLabel, value, INPUTBOX);
		if (ImGui::IsItemActive() && !EOVFloat::float_active)
		{
			EOVFloat::float_active = true;
			EOVFloat::orig = *value;
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			std::cout << "Registered" << std::endl;
			m_cmd->PushCommand(new ValueChange<float>{ value, EOVFloat::orig, *value });
			EOVVector::x_active = false;
		}
	}

	else if (objtype == typeid(bool *))
	{
		bool * value = std::any_cast<bool*>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, (variable + ": ").c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Text(*value ? "True" : "False");
		ImGui::PopItemWidth();
	}

	else if (objtype == typeid(Vector2D *))
	{
		ImGui::TextColored({ 0,10,255, 1 }, (variable + ": ").c_str());
		Vector2D * vec = std::any_cast<Vector2D*>(obj);

		ImGui::Text("x"); ImGui::SameLine();
		PrintFloat((inputLabel + "x"), &vec->x, INPUTBOX);
		if (ImGui::IsItemActive() && !EOVVector::x_active)
		{
			EOVVector::x_active = true;
			EOVVector::orig2D = *vec;
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			std::cout << "Registered" << std::endl;
			m_cmd->PushCommand(new ValueChange<Vector2D>{ vec, EOVVector::orig2D, *vec });
			EOVVector::x_active = false;
		}


		ImGui::Text("y"); ImGui::SameLine();
		PrintFloat((inputLabel + "y"), &vec->y, INPUTBOX);
		if (ImGui::IsItemActive() && !EOVVector::y_active)
		{
			EOVVector::y_active = true;
			EOVVector::orig2D = *vec;
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			std::cout << "Registered" << std::endl;
			m_cmd->PushCommand(new ValueChange<Vector2D>{ vec, EOVVector::orig2D, *vec });
			EOVVector::y_active = false;
		}

	}

	else if (objtype == typeid(Vector3D *))
	{
		ImGui::TextColored({ 0,10,255, 1 }, variable.c_str());
		Vector3D * vec = std::any_cast<Vector3D *>(obj);

		ImGui::Text("x"); ImGui::SameLine();
		PrintFloat((inputLabel + "x"), &vec->x, INPUTBOX);
		if (ImGui::IsItemActive() && !EOVVector::x_active)
		{
			EOVVector::x_active = true;
			EOVVector::orig3D = *vec;
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			std::cout << "Registered" << std::endl;
			m_cmd->PushCommand(new ValueChange<Vector3D>{ vec, EOVVector::orig3D, *vec });
			EOVVector::x_active = false;
		}

		ImGui::Text("y"); ImGui::SameLine();
		PrintFloat((inputLabel + "y"), &vec->y, INPUTBOX);
		if (ImGui::IsItemActive() && !EOVVector::y_active)
		{
			EOVVector::y_active = true;
			EOVVector::orig3D = *vec;
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			std::cout << "Registered" << std::endl;
			m_cmd->PushCommand(new ValueChange<Vector3D>{ vec, EOVVector::orig3D, *vec });
			EOVVector::y_active = false;
		}


		ImGui::Text("z"); ImGui::SameLine();
		PrintFloat((inputLabel + "z"), &vec->z, SLIDER);
		if (ImGui::IsItemActive() && !EOVVector::z_active)
		{
			EOVVector::z_active = true;
			EOVVector::orig3D = *vec;
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			std::cout << "Registered" << std::endl;
			m_cmd->PushCommand(new ValueChange<Vector3D>{ vec, EOVVector::orig3D, *vec });
			EOVVector::z_active = false;
		}
	}

	else if (objtype == typeid(std::string *))
	{
		std::string * str = std::any_cast<std::string*>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, variable.c_str()); ImGui::SameLine();
		PrintString(inputLabel, variable, str);
	}

	else if (objtype == typeid(Rect *))
	{
		Rect * rec = std::any_cast<Rect *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, variable.c_str());

		for (int i = 0; i < 4; ++i)
		{
			std::stringstream  vertexName;
			vertexName << "Vertices " << i;
			Vector2D vec = rec->m_vertices[i];
			if (ImGui::TreeNode(vertexName.str().c_str()))
			{

				ImGui::Text("x"); ImGui::SameLine();
				PrintFloat((inputLabel + "x").c_str(), &vec.x, INPUTBOX);

				ImGui::Text("y"); ImGui::SameLine();
				PrintFloat((inputLabel + "y").c_str(), &vec.y, INPUTBOX);

				ImGui::TreePop();
			}

		}
	}

	else if (objtype == typeid(Texture2D **))
	{
		Texture2D ** tex = std::any_cast<Texture2D **>(obj);
		if (!(*tex))
			return;
		ImGui::TextColored({ 0,10,255, 1 }, "Texture name: "); ImGui::SameLine();
		ImGui::Text((*tex)->GetTextureName().c_str());
	}

	else if (objtype == typeid(std::map<std::string, std::string> *))
	{
		std::map<std::string, std::string> * audioMap = std::any_cast<std::map<std::string, std::string> *>(obj);

		ImGui::TextColored({ 0,10,255, 1 }, "Audio Data: ");
		ImGui::Text("State"); ImGui::SameLine(); ImGui::Text("File path");

		for (auto & data : *audioMap)
		{
			ImGui::Text(data.first.c_str()); ImGui::SameLine();
			if (ImGui::BeginCombo((inputLabel + data.first).c_str(), data.second.c_str()))
			{
				
				for (auto & p : std::filesystem::directory_iterator{ "Resource/Audio" })
				{
					if (p.path().extension() == ".wav" || p.path().extension() == ".mp3"
						|| p.path().extension() == ".ogg")
					{
						if (ImGui::MenuItem(p.path().string().c_str()))
							data.second = p.path().string();
					}
				}
				ImGui::EndCombo();
			}
		}
	}

	else
	{

	}
}

void PrefabEditorWindow::PrintFloat(const std::string & inputLabel, float * value, BoxType box)
{
	float min = *value - 20;
	float max = *value + 20;
	ImGui::PushItemWidth(100);

	switch (box)
	{
	case(INPUTBOX): ImGui::InputFloat(inputLabel.c_str(), value); break;
	case(SLIDER): ImGui::SliderFloat(inputLabel.c_str(), value, min, max); break;
	}

	ImGui::PopItemWidth();
}

void PrefabEditorWindow::PrintString(const std::string & inputLabel, const std::string & name, std::string * str)
{
	UNREFERENCED_PARAMETER(name);
	ImGui::PushItemWidth(100);
	ImGui::InputText(inputLabel.c_str(), str);
	ImGui::PopItemWidth();
}

void PrefabEditorWindow::PrintInt(const std::string & inputLabel, int * value, BoxType box)
{
	int min = *value - 20;
	int max = *value + 20;
	ImGui::PushItemWidth(100);
	switch (box)
	{
	case(INPUTBOX): ImGui::InputInt(inputLabel.c_str(), value); break;
	case(SLIDER): ImGui::SliderInt(inputLabel.c_str(), value, min, max); break;
	}
	ImGui::PopItemWidth();
}

