/******************************************************************************/
/*!
\file  EditorObjectViewer.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EditorObjectViewer class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EditorObjectViewer.h"

enum BoxType : int
{
	TEXTBOX		 = 0,
	INPUTBOX     = 1,
    SLIDER		 = 2,
	SLIDERROTATE = 3
};

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

EditorObjectViewer::EditorObjectViewer(EditorConsole * console, ResourceFactoryManager* res_fac, CommandManager * cmd, EventHandler * ev, bool & gravity)
	:m_console{ console }, m_ResourceFactoryManager{ res_fac }, m_cmd{cmd}, m_ev{ev},  m_gravity{gravity}, m_profiler{new Profiler{"Inspector"}}
{}

EditorObjectViewer::~EditorObjectViewer()
{
	delete m_profiler;
}

std::string EditorObjectViewer::GetComponentName(ComponentType type)
{
	switch (type)
	{
	case(ComponentType::SpriteComponent):		 return "Sprite Component";	   break;
	case(ComponentType::MovementComponent):		 return "Movement Component";  break;
	case(ComponentType::CollisionComponent):	 return "Collision Component"; break;
	case(ComponentType::RigidBodyComponent):	 return	"RigidBody Component"; break;
	case(ComponentType::StatsComponent):	     return "Stats Component";	   break;
	case(ComponentType::AABBComponent):			 return "AABB Component";	   break;
	case(ComponentType::TransformComponent):	 return "Transform Component"; break;
	case(ComponentType::ScriptComponent):        return "Script Component";	   break;
	case(ComponentType::AudioComponent):		 return "Audio Component";	   break;
	default:									 return "Unknown component";   break;
	}
}

void PrintFloat(const std::string & inputLabel, float * value, BoxType box)
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

void PrintString(const std::string & inputLabel, const std::string & name, std::string * str, BoxType box)
{
	UNREFERENCED_PARAMETER(name);
	ImGui::PushItemWidth(100);
	switch (box)
	{
		case(BoxType::INPUTBOX): ImGui::InputText(inputLabel.c_str(), str); break;
		case(BoxType::TEXTBOX): ImGui::Text(str->c_str()); break;
	}
	ImGui::PopItemWidth();
}

void PrintInt(const std::string & inputLabel, int * value, BoxType box)
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

void EditorObjectViewer::PrintComponentDataAux(const std::string & label, const std::string & name, TypeErasedObj & t)
{
	std::any obj = t.GetValue();
	const type_info& objtype = obj.type();
	std::string inputLabel = "##" + label + name;

	if (objtype == typeid(int *))
	{
		int * value = std::any_cast<int *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, (name + ": ").c_str());
		PrintInt(inputLabel, value, INPUTBOX);
	}

	else if (objtype == typeid(Direction*))
	{
		int * value = (int*)(std::any_cast<Direction*>(obj));
		ImGui::TextColored({ 0,10,255, 1 }, (name + ": ").c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Text(inputLabel.c_str(), value);
		ImGui::PopItemWidth();
	}


	else if (objtype == typeid(unsigned int *))
	{
		int * value = (int*)std::any_cast<unsigned int *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, (name + ": ").c_str());
		ImGui::PushItemWidth(100);
		ImGui::InputInt(inputLabel.c_str(), value);
		ImGui::PopItemWidth();
	}

	else if (objtype == typeid(float *))
	{
		float * value = std::any_cast<float *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, (name + ": ").c_str()); ImGui::SameLine();
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
		ImGui::TextColored({ 0,10,255, 1 }, (name + ": ").c_str()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Text(*value ? "True" : "False");
		ImGui::PopItemWidth();
	}

	else if (objtype == typeid(Vector2D *))
	{
		ImGui::TextColored({ 0,10,255, 1 }, (name + ": ").c_str());
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
			m_cmd->PushCommand(new ValueChange<Vector2D>{ vec, EOVVector::orig2D, *vec });
			EOVVector::y_active = false;
		}

	}

	else if (objtype == typeid(Vector3D *))
	{
		ImGui::TextColored({ 0,10,255, 1 }, name.c_str());
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
		ImGui::TextColored({ 0,10,255, 1 }, name.c_str()); ImGui::SameLine();
		PrintString(inputLabel, name, str, BoxType::TEXTBOX);
	}

	else if (objtype == typeid(Rect *))
	{
		Rect * rec = std::any_cast<Rect *>(obj);
		ImGui::TextColored({ 0,10,255, 1 }, name.c_str());

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
		
		ImGui::Columns(3);
		ImGui::Text("State"); ImGui::NextColumn();
		ImGui::Text("File"); ImGui::NextColumn();
		ImGui::Text("Update"); ImGui::NextColumn();
		ImGui::Separator();
		for (auto & data : *audioMap)
		{
			static std::string previousAudio = data.second.c_str();
			ImGui::Text(data.first.c_str()); ImGui::SameLine();
			ImGui::NextColumn();
			if (ImGui::BeginCombo((inputLabel + data.first + "combo").c_str(), data.second.c_str()))
			{
				
				for (auto & p : std::experimental::filesystem::v1::directory_iterator("Resource/Audio"))
				{
					if (p.path().extension() == ".wav" || p.path().extension() == ".mp3"
						|| p.path().extension() == ".ogg")
					{
						if (ImGui::MenuItem(p.path().string().c_str()))
							data.second = p.path().string();
					}
				}

				if (ImGui::MenuItem("(NO AUDIO)"))
					data.second = "NO AUDIO";
				ImGui::EndCombo();
			}

			ImGui::NextColumn();

			if (ImGui::Button(const_cast<char*>((std::string{"Update##"} + inputLabel + data.first).c_str())))
				ImGui::OpenPopup(("Update Audio##" +inputLabel + data.first).c_str());

			if (ImGui::BeginPopupModal(("Update Audio##" + inputLabel + data.first).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Would you like to apply the changes?");
				if (ImGui::Button("Yes", ImVec2(120, 0)))
				{
					m_ev->PublishEvent(new ChangeAudio{});
					m_cmd->PushCommand(new ValueChange<std::string>{ &data.second, previousAudio, data.second });
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(120, 0)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		//Add new state and audio
		if (ImGui::Button("Add new state"))
			ImGui::OpenPopup(("Audio" + std::string{ "##" } +inputLabel).c_str());

		if (ImGui::BeginPopupModal(("Audio" + std::string{ "##" } +inputLabel).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static std::string newState;
			static std::string newAudio;

			ImGui::Text("State: "); ImGui::SameLine(); (ImGui::InputText((inputLabel + "state").c_str(), &newState));

			ImGui::Text("Path: "); ImGui::SameLine();
			if (ImGui::BeginCombo((inputLabel + "Audio Path").c_str(), newAudio.c_str()))
			{
				for (auto & p : std::experimental::filesystem::v1::directory_iterator("Resource/Audio"))
				{
					if (p.path().extension() == ".wav" || p.path().extension() == ".mp3"
						|| p.path().extension() == ".ogg")
					{
						if (ImGui::MenuItem(p.path().string().c_str()))
							newAudio = p.path().string();
					}
				}

				if (ImGui::MenuItem("(NO AUDIO)"))
					newAudio = "NO AUDIO";

				ImGui::EndCombo();
			}

			if (ImGui::Button("Add Audio", ImVec2(120, 0))) { (*audioMap)[newState] = newAudio;  ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}
	}

	else
	{

	}

}

template<typename T>
void EditorObjectViewer::PrintComponentData(const std::string & label, Component * comp)
{
	T *  component = static_cast<T*>(comp);
	auto map = component->metadata.reflect<TypeErasedObj>(*component);

	for (auto itor = map.begin(); itor != map.end(); ++itor)
	{
		PrintComponentDataAux(label, itor->first, itor->second);
	}
}

template<>
void EditorObjectViewer::PrintComponentData<SpriteComponent>(const std::string & label, Component * comp)
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
				unsigned key = GetSpriteKeyBySpriteName(m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer(), spritesheetdata.sprite_name);
				auto tex = m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer()->find(key)->second;
				sprite->SetTexture(tex);
				sprite->SetAnimation(spritesheetdata.texture_name);
				sprite->SetMesh(spritesheetdata.texture_name);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo(spritename_label_id.c_str(), sprite->GetSpriteName().c_str()))
		{
			for (auto it = m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer()->begin(); it != m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer()->end(); ++it)
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
								sprite->SetAnimation(texture_name);
								sprite->SetMesh(texture_name);
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
				unsigned key = GetSpriteKeyBySpriteName(m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer(), spritesheetdata.sprite_name);
				auto tex = m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer()->find(key)->second;
				sprite->SetTexture(tex);
				sprite->SetAnimation(spritesheetdata.texture_name);
				sprite->SetMesh(spritesheetdata.texture_name);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo(spritename_label_id.c_str(), sprite->GetSpriteName().c_str()))
		{
			for (auto it = m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer()->begin(); it != m_ResourceFactoryManager->GetSpriteSheetFactory()->GetContainer()->end(); ++it)
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
							sprite->SetAnimation(texture_name);
							sprite->SetMesh(texture_name);
						}
					}
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Text("Layer "); ImGui::SameLine(); 
	int layer = sprite->GetLayer();
	const char * items[5]{ "0 - Background", "1 - Background Props", "2 - Foreground", "3 - UI" , "4 - Misc"};
	if (ImGui::Combo(("##" + spritename_label_id + "layer").c_str(), &layer, items, 5))
	{
		unsigned ori = sprite->GetLayer();
		if (layer < 0) layer = 0;
		*(sprite->GetLayerPtr()) = layer;
		m_cmd->PushCommand(new ValueChange<unsigned>{ sprite->GetLayerPtr(), ori, (unsigned)layer });
	}
}

template<>
void EditorObjectViewer::PrintComponentData<ScriptComponent>(const std::string & label, Component * comp)
{
	ScriptComponent * script = dynamic_cast<ScriptComponent *>(comp);
	std::string inputLabel = "##" + label + "ScriptComponent";
	static std::string stateString = "";
	static std::string scriptPath = "";

	ImGui::TextColored({ 0,10,255, 1 }, "Script file: ");
	ImGui::Columns(2);
	ImGui::Text("State"); ImGui::NextColumn(); ImGui::Text("Script Path"); ImGui::NextColumn();
	for (auto & elem : *(script->GetScriptList()))
	{
		ImGui::Text(elem.first.c_str()); ImGui::NextColumn();
		ImGui::Text(elem.second->GetPath().c_str());
		ImGui::NextColumn();
		ImGui::Separator();
	}


	ImGui::InputText("##State", &stateString);
	ImGui::NextColumn();
	if (ImGui::BeginCombo((inputLabel).c_str(), scriptPath.c_str()))
	{
		for (auto & p : std::experimental::filesystem::v1::directory_iterator("Resource/Scripts"))
		{
			if (p.path().extension() == ".lua")
			{
				if (ImGui::MenuItem(p.path().string().c_str()))
					scriptPath = p.path().string();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::NextColumn();
	ImGui::Columns(1);

	if (ImGui::Button("Add Script"))
	{
		if(stateString.size() & scriptPath.size())
			script->AddScript(stateString, scriptPath);
	}
}

void EditorObjectViewer::PrintComponent(const std::string & label, Component * comp)
{
	if (comp)
	{
		switch (comp->GetComponentType())
		{
		case(ComponentType::StatsComponent):	  PrintComponentData<StatsComponent>(label, comp); break;
		case(ComponentType::CollisionComponent):  PrintComponentData<CollisionComponent>(label, comp); break;
		case(ComponentType::AABBComponent):		  PrintComponentData<AABBComponent>(label, comp); break;
		case(ComponentType::RigidBodyComponent):  PrintComponentData<RigidBodyComponent>(label, comp); break;
		case(ComponentType::MovementComponent):   PrintComponentData<MovementComponent>(label, comp); break;
		case(ComponentType::TransformComponent):  PrintComponentData<TransformComponent>(label, comp); break;
		case(ComponentType::SpriteComponent):	  PrintComponentData<SpriteComponent>(label, comp); break;
		case(ComponentType::ScriptComponent):	  PrintComponentData<ScriptComponent>(label, comp); break;
		case(ComponentType::AudioComponent):	  PrintComponentData<AudioComponent>(label, comp); break;
		default: break;
		}
	}
}

void EditorObjectViewer::PrintQuickAccess(SceneActor * gameobj, unsigned id)
{

	std::stringstream ss;

	//Print name
	std::string name = gameobj->GetSceneActorName();
	ImGui::PushItemWidth(40);
	ImGui::TextColored({ 0,10,255, 1 }, "Name: "); ImGui::SameLine();
	ImGui::PopItemWidth();
	ImGui::Text(name.c_str());
	ss.str(std::string());

	//Print active
	ImGui::TextColored({ 0,10,255, 1 }, "Active: "); ImGui::SameLine();
	ImGui::Checkbox(("##active" + name).c_str(), gameobj->GetActivePtr());

	//print id;
	ss << id;
	ImGui::PushItemWidth(40);
	ImGui::TextColored({ 0,10,255, 1 }, "ID: "); ImGui::SameLine(); ImGui::Text(ss.str().c_str());
	ImGui::PopItemWidth();
	ss.clear();

	//check for transform component
	auto map = gameobj->GetPrefabActor()->GetComponents();
	std::string inputLabel = "##" + gameobj->GetSceneActorName();
	if (map.find(ComponentType::TransformComponent) != map.end())
	{
		TransformComponent * trans = dynamic_cast<TransformComponent *>(map[ComponentType::TransformComponent]);
		AABBComponent	   * aabb = nullptr;
		if (map.find(ComponentType::AABBComponent) != map.end())
			aabb = dynamic_cast<AABBComponent *>(map[ComponentType::AABBComponent]);
		if (trans)
		{
			auto dataMap = trans->metadata.reflect<TypeErasedObj>(*trans);
			float * rotate = std::any_cast<float *>(dataMap["m_Rotate"].GetValue());
			Vector3D* scale = std::any_cast<Vector3D *>(dataMap["m_Scale"].GetValue());

			/****************************  SCALE  ****************************************/
			std::stringstream ss_scale;
			ss_scale << "##" << gameobj->GetSceneActorName() << "scale_x";
			ImGui::TextColored({ 0,10,255, 1 }, "Scale:");

			ImGui::Text("x: "); ImGui::SameLine();
			ImGui::InputFloat(ss_scale.str().c_str(), &scale->x);
			ss_scale.clear();
			if (ImGui::IsItemActive() && !EOVVector::x_active)
			{
				EOVVector::x_active = true;
				EOVVector::orig3D = *scale;
			}
			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				std::cout << "Registered" << std::endl;
				m_cmd->PushCommand(new ValueChange<Vector3D>{ scale, EOVVector::orig3D, *scale });
				EOVVector::x_active = false;
			}

			ss_scale << "##" << gameobj->GetSceneActorName() << "scale_y";
			ImGui::Text("y: "); ImGui::SameLine();
			ImGui::InputFloat(ss_scale.str().c_str(), &scale->y);
			ss_scale.clear();

			if (ImGui::IsItemActive() && !EOVVector::y_active)
			{
				EOVVector::y_active = true;
				EOVVector::orig3D = *scale;
			}
			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				std::cout << "Registered" << std::endl;
				m_cmd->PushCommand(new ValueChange<Vector3D>{ scale, EOVVector::orig3D, *scale });
				EOVVector::x_active = false;
			}
			/************************************************************************/

			/***************************   ROTATE    *******************************/
			std::stringstream ss_rotate;
			ss_rotate << "##" << gameobj->GetSceneActorName() << "rotate";
			ImGui::TextColored({ 0,10,255, 1 }, "Rotate: ");
			ImGui::SliderFloat(ss_rotate.str().c_str(), rotate, EOVLimits::ROTATE_MIN, EOVLimits::ROTATE_MAX);
			if (ImGui::IsItemActive() && !EOVFloat::float_active)
			{
				EOVFloat::float_active = true;
				EOVFloat::orig = *rotate;
			}
			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				std::cout << "Registered" << std::endl;
				m_cmd->PushCommand(new ValueChange<float>{ rotate, EOVFloat::orig, *rotate });
				EOVFloat::float_active = false;
			}
			/************************************************************************/

			/***************************   POSITION    *******************************/
			Vector2D* m_pos = std::any_cast<Vector2D *>(dataMap["m_Pos"].GetValue());
			std::stringstream ss_position;

			ImGui::TextColored({ 0,10,255, 1 }, "Position:"); ImGui::SameLine();

			if (ImGui::Button("Snap to center"))
			{
				m_pos->SetX(0);
				m_pos->SetY(0);
			}

			ImGui::Text("x: "); ImGui::SameLine();
			PrintFloat((inputLabel + "x").c_str(), &m_pos->x, INPUTBOX);


			ImGui::Text("y: "); ImGui::SameLine();
			PrintFloat((inputLabel + "y").c_str(), &m_pos->y, INPUTBOX);

			if (aabb)
				aabb->SetInit(true);
			/************************************************************************/
		}

		else ImGui::Text("No data");
	}

	//Tagging
	if (ImGui::CollapsingHeader(("Tagging" + inputLabel).c_str()))
	{
		auto & tagmap = gameobj->GetSceneActorTagging();
		ImGui::Columns(2);
		ImGui::TextColored({ 0,10,255, 1 }, "Key"); ImGui::NextColumn(); ImGui::TextColored({ 0,10,255, 1 }, "Value"); 
		ImGui::NextColumn();
		ImGui::Separator();
		for (auto & pairs : tagmap)
		{
			ImGui::Text(pairs.first.c_str()); ImGui::NextColumn();
			ImGui::Text(pairs.second.c_str()); ImGui::NextColumn();
			ImGui::Separator();
		}
		ImGui::Columns(1);
		if (ImGui::Button(("Add Tag##" + inputLabel).c_str()))
			ImGui::OpenPopup(("Add a tag##" + inputLabel).c_str());

		static std::string key;
		static std::string value;

		if (ImGui::BeginPopupModal(("Add a tag##" + inputLabel).c_str()))
		{
	
			ImGui::Columns(2);
			ImGui::Text("Key");
			ImGui::NextColumn();
			ImGui::InputText(("##Add a tagkey" + inputLabel).c_str(), &key);
			ImGui::NextColumn();
			ImGui::Separator();

			ImGui::Text("Value"); ImGui::NextColumn();
			ImGui::InputText(("##Add a tagvalue" + inputLabel).c_str(), &value);
			ImGui::NextColumn();

			ImGui::Separator();
			ImGui::Columns(1);
			
			if (ImGui::Button(("Add##" + inputLabel).c_str(), ImVec2{150, 0}))
			{
				if (key.size() && value.size())
				{
					tagmap.insert(std::make_pair(key, value));
					ImGui::CloseCurrentPopup();
				}
				else
					ImGui::OpenPopup("Error##Empty Parameters");
				if (ImGui::BeginPopupModal("Error##Empty Parameters"))
				{
					ImGui::Text("Please fill up both parameters before adding!");
					if(ImGui::Button("Ok", ImVec2{ 120,0 }))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
			}
			ImGui::EndPopup();
		}
	}
}

void EditorObjectViewer::PrintCameraData()
{
	if (ImGui::Button("Snap all objects"))
	{
		auto m_fac = m_ResourceFactoryManager->GetSceneActorFactory();
		for (auto & elem : m_fac->GetSceneActorContainer())
		{
			auto components = elem.second->GetPrefabActor()->GetComponents();
			if (components.find(ComponentType::TransformComponent) != components.end())
			{
				TransformComponent * trans = dynamic_cast<TransformComponent *>(components[ComponentType::TransformComponent]);
				float x = trans->GetPosition().GetX();
				float y = trans->GetPosition().GetY();
				trans->SetPosition(Vector2D((int)(x)+0.5f, (int)(y)+0.5f));
			}
		}




	}
	 //Save the original position of the camera
	auto map = m_cam->metadata.reflect<TypeErasedObj>(*m_cam);
	std::any t_pos = map["m_CamPos"].GetValue();
	glm::vec3 * cam = std::any_cast<glm::vec3 *>(t_pos);

	ImGui::TextColored({ 0,10,255, 1 }, "Camera:");

	ImGui::Text("x: "); ImGui::SameLine();
	ImGui::InputFloat("##camera x", &cam->x);

	if (ImGui::IsItemActive() && !EOVCamera::x_active)
	{
		EOVCamera::x_active = true;
		EOVCamera::orig = *cam;
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		std::cout << "Registered" << std::endl;
		m_cmd->PushCommand(new ValueChange<glm::vec3>{ cam, EOVCamera::orig, *cam});
		EOVCamera::x_active = false;
	}

	ImGui::Text("y: "); ImGui::SameLine();
	ImGui::InputFloat("##camera y", &cam->y);

	if (ImGui::IsItemActive() && !EOVCamera::y_active)
	{
		EOVCamera::y_active = true;
		EOVCamera::orig = *cam;
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		std::cout << "Registered" << std::endl;
		m_cmd->PushCommand(new ValueChange<glm::vec3>{ cam, EOVCamera::orig, *cam });
		EOVCamera::orig = *cam;
		EOVCamera::y_active = false;
	}

	ImGui::Text("z: "); ImGui::SameLine();
	ImGui::SliderFloat("##camera z", &cam->z, EOVLimits::CAMERA_Z_MIN, EOVLimits::CAMERA_Z_MAX);
	if (ImGui::IsItemActive() && !EOVCamera::z_active)
	{
		EOVCamera::z_active = true;
		EOVCamera::orig = *cam;
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		std::cout << "Registered" << std::endl;
		m_cmd->PushCommand(new ValueChange<glm::vec3>{ cam, EOVCamera::orig, *cam });
		EOVCamera::orig = *cam;
		EOVCamera::z_active = false;
	}

	ImGui::TextColored({ 0,10,255, 1 }, "Gravity:"); ImGui::SameLine();
	if (ImGui::Checkbox("##ToggleGravity", &m_gravity))
	{
	}
}

void EditorObjectViewer::Draw()
{
	m_profiler->UpdateStart();
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	//float main_size_x = viewport->Size.x;
	float main_size_y = viewport->Size.y * 0.3f;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	ImGui::Begin("Inspector", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ 300, 300 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ viewport->Pos.x , (viewport->Pos.y + viewport->Size.y) - main_size_y }, ImGuiCond_FirstUseEver);

	PrintCameraData();

	ImGui::Separator();

	auto & gameObjs = m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer();

	for (auto obj : gameObjs)
	{
		if (obj.second->GetSceneActorSelection())
		{
			PrintQuickAccess(obj.second, obj.first);

			if (ImGui::Button(("Clone object##" + obj.second->GetSceneActorName()).c_str()))
			{
				m_ResourceFactoryManager->GetSceneActorFactory()->CloneSceneActor(obj.second);
				m_console->WriteToConsole(obj.second->GetSceneActorName() + " cloned!");

			}
			ImGui::SameLine();
			if (ImGui::Button(("Delete object##" + obj.second->GetSceneActorName()).c_str()))
			{
				std::string name = obj.second->GetSceneActorName();
				m_ResourceFactoryManager->GetSceneActorFactory()->DeleteSceneActor(obj.first);
				m_console->WriteToConsole(name + " deleted!");
				m_cam->UnlatchObject();
				ImGui::End();
				return;
			}

			std::stringstream name_ss;
			name_ss << "Components " << "(" << obj.second->GetPrefabActor()->GetComponents().size() << ")##" << obj.second->GetSceneActorName();

			if (ImGui::CollapsingHeader(name_ss.str().c_str()))
			{
				auto & map = obj.second->GetPrefabActor()->GetComponents();

				std::unordered_map<ComponentType, Component*>::iterator it = map.begin();
				while (it != map.end())
				{
					std::string componentName = GetComponentName(it->first);
					if (ImGui::CollapsingHeader(componentName.c_str()))
					{
						std::string buttonLabel = "Delete component##" + name_ss.str() + componentName;
						PrintComponent(obj.second->GetSceneActorName(), it->second);
						if (ImGui::Button(buttonLabel.c_str()))
						{
							delete it->second;
							it->second = nullptr;
							it = map.erase(it);
							m_console->WriteToConsole(obj.second->GetSceneActorName() + "'s " + componentName + " deleted!");
							m_ev->PublishEvent(new SwitchLevelEvent{});
							break;
						}
					}
					++it;
				}

				ImGui::TextColored({ 0,10,255, 1 },"Components: ");
	
				auto & component_list = m_ResourceFactoryManager->GetPrefabActorFactory()->GetComponentTypeContainer();
				std::string addcomp_id = "##AddComponent" + obj.second->GetPrefabActor()->GetPath();
				if (ImGui::BeginCombo(addcomp_id.c_str(), "Select a component"))
				{
					for (auto & elem : component_list)
					{
						if (map.find(elem.first) == map.end())
						{
							if (ImGui::MenuItem(elem.second.c_str()))
							{
								auto factories = m_ResourceFactoryManager->GetPrefabActorFactory()->GetFactories();
								ComponentFactory * compfac = factories[elem.second.c_str()];
								Component * comp = compfac->CreateComponent();
								obj.second->GetPrefabActor()->AddComponent(comp);
							}
						}
					}
					ImGui::EndCombo();
				}				
			}

			ImGui::Separator();
		}

		auto & gameChildObjs = obj.second->GetChildSceneActors();

		for (auto child_obj : gameChildObjs)
		{
			DrawChildInspector(child_obj.first,child_obj.second);
			//DrawChildInspector(child_obj.first,child_obj.second);
		}
	}
	ImGui::End();
	m_profiler->UpdateEnd();
}

void EditorObjectViewer::DrawChildInspector(unsigned id , SceneActor * obj)
{
	if (obj->GetSceneActorSelection())
	{
		PrintQuickAccess(obj, id);
		if (ImGui::Button("Clone object"))
		{
			m_ResourceFactoryManager->GetSceneActorFactory()->CloneSceneActor(obj);
			m_console->WriteToConsole(obj->GetSceneActorName() + " cloned!");
			m_ev->PublishEvent(new SwitchLevelEvent{});
		}

		ImGui::SameLine();

		if (ImGui::Button("Delete object"))
		{
			std::string name = obj->GetSceneActorName();
			delete obj;
			m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer().erase(id);
			m_console->WriteToConsole(name + " deleted!");
			m_ev->PublishEvent(new SwitchLevelEvent{});
			m_ev->PublishEvent(new LoadLevel{});
		}

		std::stringstream name_ss;
		name_ss << "Components " << "(" << obj->GetPrefabActor()->GetComponents().size() << ")##" << obj->GetSceneActorName();

		if (ImGui::CollapsingHeader(name_ss.str().c_str()))
		{
			auto & map = obj->GetPrefabActor()->GetComponents();

			std::unordered_map<ComponentType, Component*>::iterator it = map.begin();
			while (it != map.end())
			{
				std::string componentName = GetComponentName(it->first);
				if (ImGui::CollapsingHeader(componentName.c_str()))
				{
					PrintComponent(obj->GetSceneActorName(), it->second);
					if (ImGui::Button("Delete component"))
					{
						delete it->second;
						it->second = nullptr;
						it = map.erase(it);
						m_console->WriteToConsole(obj->GetSceneActorName() + "'s " + componentName + " deleted!");
						break;
					}
				}
				++it;
			}


			ImGui::TextColored({ 0,10,255, 1 }, "Components");

			const char* items[] = { "TransformComponent", "SpriteComponent", "MovementComponent", "CollisionComponent",
				"RigidBodyComponent", "StatsComponent", "AABBComponent", 
				"ScriptComponent" };

			static int item_current = 0;
			ImGui::Combo("Components", &item_current, items, IM_ARRAYSIZE(items));

			ComponentType compType;
			SceneActor * gameObj = nullptr;

			if (ImGui::Button("Create Component"))
			{

				std::string compType_s{ items[item_current] };

				switch (item_current)
				{
					case(0):  compType = ComponentType::TransformComponent; break;
					case(1):  compType = ComponentType::SpriteComponent;	break;
					case(2):  compType = ComponentType::MovementComponent;  break;
					case(3):  compType = ComponentType::CollisionComponent; break;
					case(4):  compType = ComponentType::RigidBodyComponent; break;
					case(5):  compType = ComponentType::StatsComponent;		break;
					case(6):  compType = ComponentType::AABBComponent;		break;
					case(11): compType = ComponentType::ScriptComponent;	break;
					case(12): compType = ComponentType::AudioComponent;     break;
				}

				for (auto & obj1 : m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer())
				{
					if (obj1.second->GetSceneActorSelection())
						gameObj = obj1.second;
				}

				if (gameObj != nullptr)
				{
					auto components = gameObj->GetPrefabActor()->GetComponents();
					if (components.find(compType) == components.end()) //If component exist
					{
						auto factories = m_ResourceFactoryManager->GetPrefabActorFactory()->GetFactories();
						ComponentFactory * compfac = factories[compType_s];
						Component * comp = compfac->CreateComponent();
						gameObj->GetPrefabActor()->AddComponent(comp);
						m_console->WriteToConsole(compType_s + " added to " + gameObj->GetSceneActorName());
					}

					else
						m_console->WriteToConsole(gameObj->GetSceneActorName() + " already has that component!");
				}

			}


		}

		ImGui::Separator();

	}

	auto & gameChildObjs = obj->GetChildSceneActors();

	for (auto child_obj : gameChildObjs)
	{
		DrawChildInspector(child_obj.first, child_obj.second);
	}
}

