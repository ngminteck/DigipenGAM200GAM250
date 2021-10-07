/******************************************************************************/
/*!
\file   EditorWorldOutliner.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	the world outliner

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EditorWorldOutliner.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

EditorWorldOutliner::EditorWorldOutliner(EditorConsole * console, ResourceFactoryManager * resource_fac, EventHandler * ev, CommandManager * cmd, bool & pause)
	:m_console{ console }, m_ResourceFactoryManager{ resource_fac }, m_ev{ ev }, deleting{ false }, moving{ false }, m_pause{ pause },
	 dest{ nullptr }, m_cmd{ cmd }, levelName{""}, m_profiler{new Profiler{"World Outliner"}}
{
	search = "";
	new_name = "";
	levelfilepath = "Resource/Json/Level";

}

EditorWorldOutliner::~EditorWorldOutliner()
{
	delete m_profiler;
}
float times=0.f;
void EditorWorldOutliner::Draw()
{
	m_profiler->UpdateStart();
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { viewport->Size.x, 5.0f });
	ImGui::PushStyleColor(ImGuiStyleVar_FramePadding, { 0,0,0,1 });
	MainMenu();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	std::stringstream ss;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	ImGui::Begin("World Outliner", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ viewport->Size.x * 0.2f ,viewport->Size.y * 0.7f }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{viewport->Pos.x , viewport->Pos.y }, ImGuiCond_FirstUseEver);




	if (ImGui::Button("New##EditorWorldOutliner", ImVec2(60.0f, 20.0f)))
	{
		ImGui::OpenPopup("New Object##Popup");
	}
	
	if (ImGui::BeginPopup("New Object##Popup"))
	{
		
		auto & prefab = m_ResourceFactoryManager->GetPrefabActorFactory()->GetPrefabActorContainer();
		auto & default_prefab = m_ResourceFactoryManager->GetPrefabActorFactory()->GetDefaultPrefabActorContainer();
		for (auto & elem : prefab)
		{
			if (default_prefab.find(elem.second->GetPrefabActorName()) != default_prefab.end())
			{
			
				if (ImGui::MenuItem(default_prefab.find(elem.second->GetPrefabActorName())->second.c_str()))
				{
					SceneActor * actor = m_ResourceFactoryManager->GetSceneActorFactory()->NewSceneActor(elem.first);
					m_console->WriteToConsole(elem.second->GetPrefabActorName() + " added in level");
					m_cam->ViewObject(actor);
				}
			}
			else
			{
				if (ImGui::MenuItem(elem.second->GetPrefabActorName().c_str()))
				{
					SceneActor * actor = m_ResourceFactoryManager->GetSceneActorFactory()->NewSceneActor(elem.first);
					m_console->WriteToConsole(elem.second->GetPrefabActorName() + " added in level");
					m_cam->ViewObject(actor);
				}
			}
		
		}
		ImGui::EndPopup();
	}
	ImGui::SameLine();

	if (ImGui::Button("Save##EditorWorldOutliner", ImVec2(60.0f, 20.0f)))
	{
		m_ResourceFactoryManager->GetSceneActorFactory()->SaveSceneActor(levelName);
		m_console->WriteToConsole(levelName + " saved!");
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::InputText("##labelLevel name", &levelName);
	ImGui::PopItemWidth();

	if (ImGui::Button("Sort by Name", ImVec2(100.0f, 20.0f)))
	{
		SortByName();
	}
	
	ImGui::Text("Search: ");
	ImGui::SameLine();
	static char buf1[64] = "";
	ImGuiInputTextFlags text_flag = ImGuiInputTextFlags_None;
	ImGui::InputText("##WorldOutliner", buf1, 64, text_flag, NULL, NULL);
	search = buf1;
	
	ImGuiTreeNodeFlags  node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	ss << m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer().size();

	std::string header = "Entities (" + ss.str() + "): ";
	ss.clear();


	bool node_open = ImGui::TreeNodeEx("All Objects##EditorWorldOutliner", node_flags, header.c_str());
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabToScene"))
		{
			m_ResourceFactoryManager->GetSceneActorFactory()->NewSceneActor(*(static_cast<unsigned *>(payload->Data)));
		}

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MoveActor"))
		{
			std::cout << "run this" << std::endl;
			moving = true;
		}
		ImGui::EndDragDropTarget();

	}
	if (node_open)
	{
		ObjectBranch();
		ImGui::TreePop();
	}
	ImGui::End();
	if (moving)
	{
		m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorSelectionMove(dest);
		dest = nullptr;
		moving = false;
	}

	if (deleting)
	{
		
		//Register delete action
		std::cout << "Delete " << std::endl;
		auto fac = m_ResourceFactoryManager->GetSceneActorFactory();
		for (auto & elem: fac->GetSceneActorContainer())
		{
			if (elem.second->GetSceneActorSelection())
			{
				m_cmd->PushCommand(new DeleteActor{ fac, elem.first, elem.second, m_ev, m_cam});
				if (elem.second == m_cam->GetGameObject())
					m_cam->UnlatchObject();
				break;
			}

		}

		m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorSelectionDelete();
		deleting = false;
	}
	
}

void EditorWorldOutliner::MainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Level"))
			{
				m_cam->UnlatchObject();
				m_ResourceFactoryManager->GetSceneActorFactory()->UnloadSceneActorFactory();
				m_ev->PublishEvent(new SwitchLevelEvent{});
				m_ev->PublishEvent(new LoadLevel{});
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Load"))
			{
				for (auto & p : std::filesystem::directory_iterator{ levelfilepath.c_str() })
				{
					if (p.path().extension() == ".json")
					{
						std::string slash = "/";
						std::string jsonfile = levelfilepath + slash + p.path().filename().string();
						std::string filedisplay = p.path().filename().string().substr(0, p.path().filename().string().find("."));
						
						if (ImGui::MenuItem(filedisplay.c_str()))
						{
							levelName = filedisplay;
							m_cam->UnlatchObject();
							m_ResourceFactoryManager->GetSceneActorFactory()->UnloadSceneActorFactory();
							m_ResourceFactoryManager->GetSceneActorFactory()->LoadSceneActorFromJsonFile(jsonfile.c_str());
							m_cam->LatchToObject(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(), "Surtur");
							m_ev->PublishEvent(new PauseGame{}); //ensure that it pauses at when new level is loaded.
						}
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Export"))
			{
				m_ResourceFactoryManager->GetSceneActorFactory()->SaveSceneActor(levelName);
				m_console->WriteToConsole(levelName + " saved!");
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Play"))
		{
			std::string jsonfile = levelfilepath + "/" + levelName + ".json";
			
			m_cam->UnlatchObject();
			m_ResourceFactoryManager->GetSceneActorFactory()->UnloadSceneActorFactory();
			m_ResourceFactoryManager->GetSceneActorFactory()->LoadSceneActorFromJsonFile(jsonfile.c_str());
			m_cam->LatchToObject(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(), "Surtur");

		}

		if (ImGui::MenuItem(m_pause ? "Unpause" : "Pause"))
		{
			m_ev->PublishEvent(new PauseGame{});
		}
		
		ImGui::EndMainMenuBar();
	}

	m_profiler->UpdateEnd();
}

bool SortFunction(SceneActor * lhs, SceneActor * rhs)
{
	return lhs->GetSceneActorName() < rhs->GetSceneActorName();
}

void EditorWorldOutliner::SortByName()
{
	std::vector<SceneActor *> temp;
	auto & fac = m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer();

	//Load the actors into a vector
	for (auto & elem : fac)
		temp.push_back(elem.second);

	//Sort the vector
	std::sort(temp.begin(), temp.end(), SortFunction);

	//Empty the old map
	fac.clear();
	int id = 0;
	
	for (auto & elem : temp)
	{
		fac.emplace(std::make_pair(id, elem));
		++id;
	}
}

void EditorWorldOutliner::ObjectBranch()
{
	std::stringstream ss;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;

	auto & sceneactor_container = m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer();
	
	unsigned id = 0;
	
	if (sceneactor_container.size() > 0)
	{
		for (auto & elem : sceneactor_container)
		{
			ss.str("");
			ss << id++;
			ss << ". ";

			if (search.size() == 0 || (search.size() > 0 && elem.second->GetSceneActorName().find(search) != std::string::npos))
			{
				node_flags = (elem.second->GetSceneActorSelection() == true ? ImGuiTreeNodeFlags_Selected : 0);

				if (elem.second->GetChildSceneActors().size() == 0)
				{
					node_flags |= ImGuiTreeNodeFlags_Leaf;
				}
				else
				{
					node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				}
				bool node_open = ImGui::TreeNodeEx(elem.second->GetSceneActorName().c_str(), node_flags, (ss.str() + elem.second->GetSceneActorName()).c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabToScene"))
					{
						m_ResourceFactoryManager->GetSceneActorFactory()->NewSceneActor(*(static_cast<unsigned *>(payload->Data)), elem.second);
						m_ev->PublishEvent(new SwitchLevelEvent());
					}

					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MoveActor"))
					{
						dest = elem.second;
						moving = true;
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::IsItemHovered())
				{
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						std::cout << "dragging" << std::endl;
						ImGui::SetDragDropPayload("MoveActor", NULL, NULL);
						ImGui::Text("Move to...");
						ImGui::EndDragDropSource();
					}
					else
					{
						if (ImGui::IsKeyDown(CVK_SHIFT) && ImGui::IsMouseReleased(0))
						{
							std::cout << "shift click" << std::endl;
							m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorShiftSelection(elem.second);
						}
						else if (ImGui::IsKeyDown(CVK_CTRL) && ImGui::IsMouseReleased(0))
						{
							std::cout << "control click" << std::endl;
							elem.second->SelectWithControlKey();
						}
						else if (ImGui::IsMouseReleased(0))
						{
							std::cout << "normal mouse click" << std::endl;
							m_ResourceFactoryManager->GetSceneActorFactory()->ClearWholeSceneActorSelection();

							if (elem.second->GetSceneActorSelection())
								elem.second->SetSceneActorSelection(false);
							else
								elem.second->SetSceneActorSelection(true);

							//m_cam->LatchToObject(elem.second);
							m_cam->ViewObject(elem.second);
						}
					}

				}
				std::string BeginPopupContextItem_label = "Options##" + elem.second->GetSceneActorName();
				if (ImGui::BeginPopupContextItem(BeginPopupContextItem_label.c_str()))
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::MenuItem("Duplicate All Selected"))
					{
						m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorSelectionDuplicate();
						m_ev->PublishEvent(new SwitchLevelEvent{});
					}
					std::string OpenPopup_label = "Rename##OpenPopup" + elem.second->GetSceneActorName();
					ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
					if (ImGui::MenuItem("Rename All Selected"))
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
							m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorSelectionRename(new_name);
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
						moving = true;
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
					ChildObjectBranch(elem.second->GetChildSceneActors());
					ImGui::TreePop();
				}
			}
		}
	}
}

void EditorWorldOutliner::ChildObjectBranch(std::map<unsigned, SceneActor *> child_object)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
	if (child_object.size() > 0)
	{
		for (auto & elem : child_object)
		{
			node_flags = (elem.second->GetSceneActorSelection() == true ? ImGuiTreeNodeFlags_Selected : 0);

			if (elem.second->GetChildSceneActors().size() == 0)
			{
				node_flags |= ImGuiTreeNodeFlags_Leaf;
			}
			else
			{
				node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			}
			bool node_open = ImGui::TreeNodeEx(elem.second->GetSceneActorName().c_str(), node_flags, elem.second->GetSceneActorName().c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabToScene"))
				{
					m_ResourceFactoryManager->GetSceneActorFactory()->NewSceneActor(*(static_cast<unsigned *>(payload->Data)), elem.second);
					m_ev->PublishEvent(new SwitchLevelEvent{});
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MoveActor"))
				{
					dest = elem.second;
					moving = true;
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
					m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorShiftSelection(elem.second);
				}
				else if (ImGui::GetIO().KeyCtrl && ImGui::IsMouseReleased(0))
				{
					elem.second->SelectWithControlKey();
				}
				else if (ImGui::IsMouseReleased(0))
				{
					m_ResourceFactoryManager->GetSceneActorFactory()->ClearWholeSceneActorSelection();

					if (elem.second->GetSceneActorSelection() == true)
						elem.second->SetSceneActorSelection(false);
					else
						elem.second->SetSceneActorSelection(true);

					m_cam->ViewObject(elem.second);
				}

			}
			std::string BeginPopupContextItem_label = "Options##" + elem.second->GetSceneActorName();
			if (ImGui::BeginPopupContextItem(BeginPopupContextItem_label.c_str()))
			{
				ImGui::PushItemWidth(-1);
				if (ImGui::MenuItem("Duplicate All Selected"))
				{
					m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorSelectionDuplicate();

				}
				std::string OpenPopup_label = "Rename##OpenPopup" + elem.second->GetSceneActorName();
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				if (ImGui::MenuItem("Rename All Selected"))
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
						m_ResourceFactoryManager->GetSceneActorFactory()->SceneActorSelectionRename(new_name);
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
					dest = elem.second;
					moving = true;
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
				ChildObjectBranch(elem.second->GetChildSceneActors());
				ImGui::TreePop();
			}
		}
	}
}
