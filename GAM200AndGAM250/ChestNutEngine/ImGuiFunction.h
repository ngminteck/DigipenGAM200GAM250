#pragma once
#include "GameObject.h"
#include "imgui.h"
#include "Component.h"

int ComponentTypeToInt(ComponentType comp_type);


void GameObjectRightClickPopUp( GameObject * game_object);

void ComponentRightClickPopUp(GameObject * game_object, Component* comp);
