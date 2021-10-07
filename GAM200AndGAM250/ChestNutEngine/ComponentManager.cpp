#include "ComponentManager.h"
#include "Component.h"

ComponentManager::ComponentManager()
{

}

ComponentManager::~ComponentManager()
{


}
void ComponentManager::UpdatePlayerHealth(const std::vector<GameObject>& vec)
{
	for (auto & elem : vec)
	{
		if (elem.GetType() == GameObject::PLAYER)
		{
			//std::vector<Component*> vec = elem.GetComponents();
			//Find the health component
			//Update the health component
		}
	}


}