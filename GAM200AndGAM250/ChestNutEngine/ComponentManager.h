#pragma once
#include "GameObject.h"
class ComponentManager
{
	ComponentManager();
	~ComponentManager();

	void UpdatePlayerHealth(const std::vector<GameObject>&);

};