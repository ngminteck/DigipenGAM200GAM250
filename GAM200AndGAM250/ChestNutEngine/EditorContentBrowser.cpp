/******************************************************************************/
/*!
\file   EditorContentBrowser.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief container the ImGui of File directory and  the asset panel


All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EditorContentBrowser.h"

EditorContentBrowser::EditorContentBrowser( EditorConsole * console, ResourceFactoryManager* resource_fac , CommandManager * cmd)
	: thumbnail_size_x{ 100.0f }, thumbnail_size_y{100.0f}, m_console{console}, m_ResourceFactoryManager{ resource_fac }, m_CommandManager{cmd}, m_profiler{new Profiler{"Content Browser"}}
{
	search_asset = "";
	new_name = "";
	current_selected_directory = "Prefab";
	resource = "Resource";
	spritesheetdata.sprite_name ="";
	spritesheetdata.texture_name = "";

	memory_asset_directory_name.push_back("Prefab");
	memory_asset_directory_name.push_back("Sprite");
	memory_asset_directory_name.push_back("Texture");

}

EditorContentBrowser::~EditorContentBrowser()
{
	for (auto it = prefab_editor_window.begin(); it != prefab_editor_window.end();)
	{
		it->second->SetPrefabEditorWindowActive(false);
		it->second->SetPrefabActorToNullBeforeDestroyWindow();
		delete it->second;
		it->second = nullptr;
		it = prefab_editor_window.erase(it);
	}

	delete m_profiler;
}

void EditorContentBrowser::Init()
{
	auto prefab_container = m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer();
	auto default_prefab_container = m_ResourceFactoryManager->GetPrefabActorFactory()->GetDefaultPrefabActorContainer();

	for (auto & elem : prefab_container)
	{
		if (default_prefab_container.find(elem.second->GetPrefabActorName()) == default_prefab_container.end())
		{
			PrefabEditorWindow* prefab_editor_win = new PrefabEditorWindow(elem.second, m_ResourceFactoryManager, m_CommandManager);
			prefab_editor_window.emplace(elem.first, prefab_editor_win);
		}
	}
}

void EditorContentBrowser::Draw()
{
	m_profiler->UpdateStart();
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	float directory_window_size_x = viewport->Size.x * 0.2f;
	float directory_window_size_y = viewport->Size.y * 0.3f;

	float asset_window_size_x = viewport->Size.x - directory_window_size_x - viewport->Size.x * 0.2f;
	float asset_window_size_y = viewport->Size.y * 0.3f;

	float directory_window_pos_x = viewport->Pos.x;
	float directory_window_pos_y = viewport->Size.y - directory_window_size_x;

	float asset_window_pos_x = viewport->Pos.x + asset_window_size_x;
	float asset_window_pos_y = viewport->Pos.y + viewport->Size.y - asset_window_size_y;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	ImGui::Begin("Directory" , NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ directory_window_size_x, directory_window_size_y }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ directory_window_pos_x , directory_window_pos_y}, ImGuiCond_FirstUseEver);
	Directory();
	ImGui::End();

	ImGui::Begin("Asset", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ asset_window_size_x, asset_window_size_y}, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ asset_window_pos_x , asset_window_pos_y }, ImGuiCond_FirstUseEver);
	int columns_count = static_cast<int>(asset_window_size_x / thumbnail_size_x);
	if (columns_count < 0)
		columns_count = 1;
	Asset(columns_count);
	ImGui::End();

	if (prefab_editor_window.size() > 0)
	{
		for (auto & elem : prefab_editor_window)
		{
			elem.second->Draw();
		}
	}
	m_profiler->UpdateEnd();
}

void EditorContentBrowser::Directory()
{

	if (ImGui::CollapsingHeader("Memory Asset"))
	{
		for (auto & elem : memory_asset_directory_name)
		{
			ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | (elem.compare(current_selected_directory) == 0 ? ImGuiTreeNodeFlags_Selected : 0);
			ImGui::TreeNodeEx(elem.c_str(), TreeNodeEx_flags, elem.c_str());
			if (ImGui::IsItemClicked())
			{
				current_selected_directory = elem;
			}
			ImGui::TreePop();
		}
	
	}
	if(ImGui::CollapsingHeader("File Asset"))
	{
		ImGuiTreeNodeFlags Resource_TreeNodeEx_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (resource.compare(current_selected_directory) == 0 ? ImGuiTreeNodeFlags_Selected : 0);
		bool node_open_resource = ImGui::TreeNodeEx(resource.c_str(), Resource_TreeNodeEx_flags, resource.c_str());
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
		{
			current_selected_directory = resource;
		}
		if (node_open_resource)
		{
			for (auto & p : std::filesystem::directory_iterator{ resource.c_str() })
			{
				if (!p.path().has_extension())
				{
					ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (p.path().string().compare(current_selected_directory) == 0 ? ImGuiTreeNodeFlags_Selected : 0);
					bool node_open = ImGui::TreeNodeEx(p.path().string().c_str(), TreeNodeEx_flags, p.path().filename().string().c_str());
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
					{
						current_selected_directory = p.path().string();
					}
					if (node_open)
					{
						RecursiveDirectory(p.path().string());
						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
		
	}
}

void EditorContentBrowser::RecursiveDirectory(std::string path)
{
	for (auto & p : std::filesystem::directory_iterator{ path.c_str() })
	{
		if (!p.path().has_extension())
		{
			ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (p.path().string().compare(current_selected_directory) == 0 ? ImGuiTreeNodeFlags_Selected : 0);
			bool node_open = ImGui::TreeNodeEx(p.path().string().c_str(), TreeNodeEx_flags, p.path().filename().string().c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
			{
				current_selected_directory = p.path().string();
			}
			if (node_open)
			{
				RecursiveDirectory(p.path().string());
				ImGui::TreePop();
			}
		}
	}
}
#pragma warning( push )
#pragma warning( disable : 4312)

void EditorContentBrowser::Asset(int col_max)
{ 
	if (ImGui::Button("New Asset##EditorContentBrowser", ImVec2(100.0f, 20.0f)))
	{
		ImGui::OpenPopup("New Asset##Popup");
	}
	if (ImGui::BeginPopup("New Asset##Popup"))
	{
		if (ImGui::BeginMenu("New Prefab##EditorContentBrowser"))
		{
			auto & prefab = m_ResourceFactoryManager->GetPrefabActorFactory()->GetDefaultPrefabActorContainer();
			for (auto & elem : prefab)
			{
				if (ImGui::MenuItem(elem.second.c_str()))
				{
					unsigned prefab_key = GetPrefabKeyByName(&m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer(), elem.first);
					unsigned key = m_ResourceFactoryManager->GetPrefabActorFactory()->NewPrefab(prefab_key);

					auto prefab_actor = m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer().find(key)->second;
					PrefabEditorWindow* prefab_editor_win = new PrefabEditorWindow(prefab_actor, m_ResourceFactoryManager, m_CommandManager);
					prefab_editor_window.emplace(key, prefab_editor_win);


				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("New Texture##EditorContentBrowser"))
		{
			ImGui::MenuItem("Ask Justin and Jaryl do this feature.##EditorContentBrowser");
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save All Asset##EditorContentBrowser", ImVec2(120.0f, 20.0f)))
	{
		m_ResourceFactoryManager->GetPrefabActorFactory()->SaveAllPrefabActor();
	}
	ImGui::SameLine();
	if (ImGui::Button("Close All Prefab Editor##EditorContentBrowser", ImVec2(180.0f, 20.0f)))
	{
		if (prefab_editor_window.size() > 0)
		{
			for (auto & elem : prefab_editor_window)
			{
				prefab_editor_window.find(elem.first)->second->SetPrefabEditorWindowActive(false);
			}
		}
		
	}
	ImGui::SameLine();
	ImGui::Text("Search Asset:");
	ImGui::SameLine();
	static char buf1[64] = "";
	ImGuiInputTextFlags text_flag = ImGuiInputTextFlags_None;
	ImGui::PushItemWidth(100);
	ImGui::InputText("##EditorContentBrowser", buf1, 64, text_flag, NULL, NULL);
	ImGui::PopItemWidth();
	search_asset = buf1;
		ImGui::SameLine();
		ImGui::Text("Asset Directory:");
		ImGui::SameLine();
		std::string directory_button = current_selected_directory;
		while (directory_button.size() > 0)
		{
			std::string directory_button_label_display = directory_button.substr(0, directory_button.find("\\"));
			directory_button.erase(0, directory_button_label_display.size());
			std::string directory_path = current_selected_directory.substr(0, current_selected_directory.find(directory_button));
			// idk why the last whole path will don't have, some hard code here
			if (directory_path.size() <= 0)
			{
				directory_path = current_selected_directory;
			}
			std::string directory_button_label_id = directory_button_label_display + "##" + directory_path;
			if (ImGui::Button(directory_button_label_id.c_str(), ImVec2(100.0f, 20.0f)))
			{
				current_selected_directory = directory_path;
			}
			if (directory_button.size() > 0)
			{
				directory_button.erase(0, 1);
				ImGui::SameLine();
				ImGui::Text(">");
				ImGui::SameLine();
			}
		}
		ImGui::Separator();
		int count = 1;
		ImTextureID tex_id = 0;
		ImVec2 min, max;
		std::string search{ buf1 };
		if (current_selected_directory.compare("Prefab") == 0)
		{
			auto & prefab = m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer();
			//auto & default_prefab = m_ResourceFactoryManager->GetPrefabActorFactory()->GetDefaultPrefabActorContainer();
			bool deleting = false;
			unsigned deleting_index = 0;
			for (auto & elem : prefab)
			{
				if (search.size() == 0 || (search.size() > 0 && elem.second->GetPrefabActorName().find(search) != std::string::npos))
				{
					if (elem.second->GetComponents().find(ComponentType::SpriteComponent) != elem.second->GetComponents().end())
					{
						SpriteComponent * sprite_comp = dynamic_cast<SpriteComponent*>(elem.second->GetComponents().find(ComponentType::SpriteComponent)->second);
						if (sprite_comp->GetTexture() != nullptr)
						{
							tex_id = reinterpret_cast<void*>(sprite_comp->GetTexture()->GetTextureID());
							std::string tex_name = sprite_comp->GetTextureName();
							auto animation_container = sprite_comp->GetTexture()->GetAnimationListContainer();
							if (animation_container.find(tex_name) != animation_container.end())
							{
								auto tex_info = animation_container.find(tex_name)->second;
								min = { tex_info->m_max.x , tex_info->m_min.y };
								max = { tex_info->m_min.x , tex_info->m_max.y };
							}
						}
					}
					std::string label = elem.second->GetPrefabActorName();

					if (ImGui::ThumbNail(tex_id, ImVec2(thumbnail_size_x, thumbnail_size_y), &label, true, min, max))
					{
						if (prefab_editor_window.find(elem.first) != prefab_editor_window.end())
						{
							prefab_editor_window.find(elem.first)->second->SetPrefabEditorWindowActive(true);
						}
						else
						{
							PrefabEditorWindow* prefab_editor_win = new PrefabEditorWindow(elem.second, m_ResourceFactoryManager, m_CommandManager);
							prefab_editor_window.emplace(elem.first, prefab_editor_win);
							prefab_editor_window.find(elem.first)->second->SetPrefabEditorWindowActive(true);
						}
					}
					++count;
					std::string BeginPopupContextItem_label = "Options##EditorContentBrowser" + label;
					if (ImGui::BeginPopupContextItem(BeginPopupContextItem_label.c_str()))
					{
						ImGui::PushItemWidth(-1);
						if (ImGui::MenuItem("Duplicate"))
						{
							unsigned key = m_ResourceFactoryManager->GetPrefabActorFactory()->DuplicatePrefab(elem.second);
							auto prefab_actor = m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer().find(key)->second;
							PrefabEditorWindow* prefab_editor_win = new PrefabEditorWindow(prefab_actor, m_ResourceFactoryManager, m_CommandManager);
							prefab_editor_window.emplace(key, prefab_editor_win);
						}
						std::string OpenPopup_label = "Rename##OpenPopup" + elem.second->GetPrefabActorName();
						ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
						if (ImGui::MenuItem("Rename"))
						{
							ImGui::OpenPopup(OpenPopup_label.c_str());
						}
						if (ImGui::BeginPopupModal(OpenPopup_label.c_str()))
						{
							static char buf2[64] = "";
							ImGuiInputTextFlags text_flag1 = ImGuiInputTextFlags_None;
							ImGui::PushItemWidth(100);
							ImGui::InputText("##EditorContentBrowser", buf2, 64, text_flag1, NULL, NULL);
							ImGui::PopItemWidth();
							new_name = buf2;
							ImGui::Separator();

							if (ImGui::Button("Apply", ImVec2(120, 0)))
							{
								std::string old_path = elem.second->GetPath();
								elem.second->SetPrefabActorName(&(m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer()), new_name);
								m_ResourceFactoryManager->GetPrefabActorFactory()->SavePrefabActor(elem.second);
								m_ResourceFactoryManager->GetSceneActorFactory()->UpdateSceneActorCriticalInfo(old_path, elem.second);
								m_ResourceFactoryManager->GetPrefabActorFactory()->DeletePrefabFile(old_path);
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
						if (ImGui::MenuItem("Delete"))
						{
							deleting_index = elem.first;
							deleting = true;
						}

						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("PrefabToScene", &elem.first, sizeof(unsigned));
						std::string drag_msg = "Add " + elem.second->GetPrefabActorName() + " to scene.";
						ImGui::Text(drag_msg.c_str());
						ImGui::EndDragDropSource();
					}
					if (count < col_max)
					{
						ImGui::SameLine();
					}
					else
					{
						count = 1;
					}
				}
			}
			if (deleting)
			{
				DestroyPrefabEditorWindow(deleting_index);
				m_ResourceFactoryManager->GetPrefabActorFactory()->DeletePrefab(deleting_index);
			}
		}
		else if (current_selected_directory.compare("Sprite") ==0)
		{
			auto sprite_sheet = m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer();
			for (auto it  = sprite_sheet->begin() ; it != sprite_sheet->end() ; ++ it)
			{
				if (it->second->GetIsAnimatedTexture() == true)
				{
					std::string path_name = it->second->GetPath();
					auto & tex = it->second->GetAnimationListContainer();
					std::string spritesheet_name = it->second->GetTextureName();
					for (auto it2 = tex.begin(); it2 != tex.end(); ++it2)
					{
						std::string label = it2->first + "##" + path_name;
						std::string texture_name = it2->first;
						tex_id = reinterpret_cast<void*>(it->second->GetTextureID());
						
						min = { it2->second->m_min.x, it2->second->m_min.y };
						max = { it2->second->m_max.x, it2->second->m_max.y };
						if (ImGui::ThumbNail(tex_id, ImVec2(thumbnail_size_x, thumbnail_size_y), &label,true, min, max),false)
						{

						}
						++count;
						if (count < col_max)
						{
							ImGui::SameLine();
						}
						else
						{
							count = 1;
						}
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
						{

							spritesheetdata.sprite_name = spritesheet_name;
							spritesheetdata.texture_name = texture_name;

							ImGui::SetDragDropPayload("ApplySprite", &spritesheetdata, sizeof(SpriteSheetstring));
							std::string drag_msg = "Add " + texture_name + " to scene.";
							ImGui::Text(drag_msg.c_str());
							ImGui::EndDragDropSource();
						}
					}
				}
			}
		}
		else if (current_selected_directory.compare("Texture") == 0)
		{
		auto sprite_sheet = m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer();
		for (auto it = sprite_sheet->begin(); it != sprite_sheet->end(); ++it)
		{
			if (it->second->GetIsAnimatedTexture() == false)
			{
				std::string path_name = it->second->GetPath();
				auto & tex = it->second->GetAnimationListContainer();
				std::string spritesheet_name = it->second->GetTextureName();
				for (auto it2 = tex.begin(); it2 != tex.end(); ++it2)
				{
					std::string label = it2->first + "##" + path_name;
					std::string texture_name = it2->first;

					tex_id = reinterpret_cast<void*>(it->second->GetTextureID());
					
					min = { it2->second->m_max.x,it2->second->m_min.y };
					 max = { it2->second->m_min.x,it2->second->m_max.y };
					if (ImGui::ThumbNail(tex_id, ImVec2(thumbnail_size_x, thumbnail_size_y), &label,true,min,max))
					{

					}
					++count;
					if (count < col_max)
					{
						ImGui::SameLine();
					}
					else
					{
						count = 1;
					}
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						spritesheetdata.sprite_name = spritesheet_name;
						spritesheetdata.texture_name = texture_name;

						ImGui::SetDragDropPayload("ApplyTexture", &spritesheetdata, sizeof(SpriteSheetstring));
						
						std::string drag_msg = "Add " + texture_name + " to scene.";
						ImGui::Text(drag_msg.c_str());
						ImGui::EndDragDropSource();
					}
				}
			}
		}
		}
		else
		{
			for (auto & p : std::filesystem::directory_iterator{ current_selected_directory.c_str() })
			{
				if (!p.path().has_extension())
				{
					std::string label = p.path().filename().string() + "##" + p.path().string();
				
					if (ImGui::ThumbNail(&tex_id, ImVec2(thumbnail_size_x, thumbnail_size_y), &label))
					{
						current_selected_directory = p.path().string();
					}
					++count;
					if (count < col_max)
					{
						ImGui::SameLine();
					}
					else
					{
						count = 1;
					}
				}
			}
			for (auto & p : std::filesystem::directory_iterator{ current_selected_directory.c_str() })
			{
				if (p.path().has_extension())
				{
					std::string label = p.path().filename().string() + "##" + p.path().string();
					if (ImGui::ThumbNail(&tex_id, ImVec2(thumbnail_size_x, thumbnail_size_y), &label))
					{

					}
					++count;
					if (count < col_max)
					{
						ImGui::SameLine();
					}
					else
					{
						count = 1;
					}
				}
			}
		}

}
#pragma warning( pop ) 
void EditorContentBrowser::DestroyPrefabEditorWindow(unsigned key)
{
	if (prefab_editor_window.find(key) != prefab_editor_window.end())
	{
		prefab_editor_window.find(key)->second->SetPrefabEditorWindowActive(false);
		prefab_editor_window.find(key)->second->SetPrefabActorToNullBeforeDestroyWindow();
		delete prefab_editor_window.find(key)->second;
		prefab_editor_window.find(key)->second = nullptr;
		prefab_editor_window.erase(key);
	}
}