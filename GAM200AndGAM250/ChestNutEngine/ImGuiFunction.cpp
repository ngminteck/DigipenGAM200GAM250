#include "ImGuiFunction.h"

int ComponentTypeToInt(ComponentType comp_type)
{
	switch (comp_type)
	{
	case (ComponentType::TransformComponent): return 1; break;
	case (ComponentType::SpriteComponent): return 2; break;
	case (ComponentType::MovementComponent): return 3; break;
	case (ComponentType::CollisionComponent): return 4; break;
	case (ComponentType::RigidBodyComponent): return 5; break;
	case (ComponentType::HPComponent): return 6; break;
	case (ComponentType::AABBComponent): return 7; break;
	case (ComponentType::ControlComponent): return 8; break;
	case (ComponentType::GravityComponent): return 9; break;
	case (ComponentType::TriangleComponent): return 10; break;
	case (ComponentType::HotspotComponent): return 11; break;
	 default: return 0; break;

	}
}

void GameObjectRightClickPopUp(GameObject* gameobject)
{
	std::stringstream popup_label;
	popup_label.str(std::string());
	popup_label << "##" << gameobject->GetIDName();

	if (ImGui::BeginPopupContextItem(popup_label.str().c_str()))
	{
		std::stringstream rename_text;
		std::stringstream duplicate_text;
		std::stringstream delete_text;

		rename_text.str(std::string());
		duplicate_text.str(std::string());
		delete_text.str(std::string());

		rename_text << "Rename" << popup_label.str();
		duplicate_text << "Duplicate" << popup_label.str();
		delete_text << "Delete" << popup_label.str();

		if (ImGui::Selectable(rename_text.str().c_str()))
		{

		}
		if (ImGui::Selectable(duplicate_text.str().c_str()))
		{

		}
		if (ImGui::Selectable(delete_text.str().c_str()))
		{

		}
		ImGui::EndPopup();
	}
}

void ComponentRightClickPopUp(GameObject * game_object, Component* comp)
{
	std::stringstream popup_label;
	popup_label.str(std::string());
	popup_label << "##" << game_object->GetIDName() << "Component" << ComponentTypeToInt(comp->GetComponentType());

	if (ImGui::BeginPopupContextItem(popup_label.str().c_str()))
	{
		std::stringstream delete_text;

		delete_text.str(std::string());

		delete_text << "Delete" << popup_label.str();

		std::cout << ComponentTypeToInt(comp->GetComponentType()) << std::endl;

		if (ImGui::Selectable(delete_text.str().c_str()))
		{
			game_object->GetComponents().erase(comp->GetComponentType());
			delete comp;
			comp = nullptr;
		}
		ImGui::EndPopup();
	}
}