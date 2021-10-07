/******************************************************************************/
/*!
\file  SceneActor.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the scene actor object, for level


All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SceneActor.h"
SceneActor::SceneActor()
	: Selection(), child_scene_actor_key{ 1 }, parent_scene_actor{ nullptr }, prefab_actor_object{ nullptr }, active{true}, invul{false}, timer{0.0}
{
	std::string scene_actor_name_value = "";
	scene_actor_tagging.emplace(scene_actor_name_key, scene_actor_name_value);
	std::string scene_actor_prefab_type_value = "NotPrefab";
	scene_actor_tagging.emplace(scene_actor_prefab_type_key, scene_actor_prefab_type_value);
}

SceneActor::SceneActor(PrefabActor * prefab_actor, SceneActor * parent_sceneactor)
	:Selection(), child_scene_actor_key{ 1 }, parent_scene_actor{ parent_sceneactor }, prefab_actor_object{ new PrefabActor(*prefab_actor) }, active{ true }
{
	// mean copy from default prefab, need give them new name and path
	
	// update parent scene actor and child actor transform relations
	if (parent_sceneactor != nullptr)
	{
		auto parent_comp_map = parent_scene_actor->GetPrefabActor()->GetComponentsPtr();
		auto comp_map = prefab_actor_object->GetComponentsPtr();
		if (parent_comp_map->find(ComponentType::TransformComponent) != parent_comp_map->end() && comp_map->find(ComponentType::TransformComponent) != comp_map->end())
		{
			TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp_map->find(ComponentType::TransformComponent)->second);
			TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map->find(ComponentType::TransformComponent)->second);
			trans->SetParentTransformComponent(parent_trans);
		}
	}
	// use default prefab actor name as scene actor name, later on will check unique scene actor name;
	std::string scene_actor_name_value = prefab_actor_object->GetPrefabActorName();
	scene_actor_tagging.emplace(scene_actor_name_key, scene_actor_name_value);

	std::string scene_actor_prefab_type_value = prefab_actor_object->GetPrefabActorName();
	scene_actor_tagging.emplace(scene_actor_prefab_type_key, scene_actor_prefab_type_value);
}

SceneActor::SceneActor(const SceneActor & rhs, bool copy_child )
:Selection(), child_scene_actor_key{ 1 }, parent_scene_actor{ nullptr }, prefab_actor_object{ new PrefabActor( *(rhs.GetPrefabActorConst())) }, active{ true }
{
	for (auto tagging_it = rhs.scene_actor_tagging.begin(); tagging_it != rhs.scene_actor_tagging.end(); ++tagging_it)
	{
		scene_actor_tagging.emplace(tagging_it->first, tagging_it->second);
	}

	if (copy_child)
	{
		for (auto it = rhs.child_scene_actor_container.begin(); it != rhs.child_scene_actor_container.end(); ++it, ++child_scene_actor_key)
		{
			SceneActor * child_scene = new SceneActor(*(it->second), copy_child);
			child_scene->SetParentSceneActorPointer(this);
			auto parent_map = prefab_actor_object->GetComponents();
			auto child_map = child_scene->GetPrefabActor()->GetComponents();
			if (parent_map.find(ComponentType::TransformComponent) != parent_map.end() && child_map.find(ComponentType::TransformComponent) != child_map.end())
			{
				TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_map.find(ComponentType::TransformComponent)->second);
				TransformComponent* trans = dynamic_cast<TransformComponent*>(child_map.find(ComponentType::TransformComponent)->second);
				trans->SetParentTransformComponent(parent_trans);
			}
			child_scene_actor_container.emplace(child_scene_actor_key, child_scene);
		}
	}
}

SceneActor::~SceneActor()
{
	if (child_scene_actor_container.size()> 0)
	{
		for (auto it = child_scene_actor_container.begin(); it != child_scene_actor_container.end(); ++it)
		{
			EmptyChildSceneActors(it->second);
			delete it->second->GetPrefabActor();
			it->second->SetPrefabActor(nullptr);
			it->second->GetSceneActorTagging().clear();
			delete it->second;
			it->second = nullptr;
		}
		child_scene_actor_container.clear();
	}
	delete prefab_actor_object;
	prefab_actor_object = nullptr;
	scene_actor_tagging.clear();
}

std::string SceneActor::GetSceneActorName() const
{
	if (scene_actor_tagging.find("scene_actor_name")!= scene_actor_tagging.end())
	{
		return scene_actor_tagging.find("scene_actor_name")->second;
	}
	return scene_actor_tagging.find("scene_actor_name_key")->second;
}

void SceneActor::SetSceneActorName(std::string name)
{
	scene_actor_tagging.find("scene_actor_name_key")->second = name;
}

void SceneActor::SetSceneActorName(std::map<unsigned, SceneActor *> * scene_container, std::string name)
{
	std::string name_id = name;
	std::stringstream find_id;
	find_id.str(std::string());
	find_id << name_id;
	unsigned no = 1;

	bool unique = CheckSceneActorUniqueName(scene_container, find_id.str());

	while (!unique)
	{
		++no;
		find_id.str(std::string());
		find_id << name_id << "(" << no << ")";
		unique = CheckSceneActorUniqueName(scene_container, find_id.str());
	}
	SetSceneActorName(find_id.str());

	//if (find_id.str().compare(name) != 0)
	//	std::cout << name << " already existed, it will be rename as " << find_id.str() << " instead." << std::endl;
}

std::string SceneActor::GetSceneActorPrefabType() const
{
	return scene_actor_tagging.find("scene_actor_prefab_type_key")->second;
}

void SceneActor::SetSceneActorPrefabTYpe(std::string name)
{
	scene_actor_tagging.find("scene_actor_prefab_type_key")->second = name;
}

std::unordered_map<std::string, std::string> & SceneActor::GetSceneActorTagging()
{
	return scene_actor_tagging;
}

std::string SceneActor::GetSceneActorTaggingValue(std::string key)
{
	if (scene_actor_tagging.find(key) != scene_actor_tagging.end())
	{
		return scene_actor_tagging.find(key)->second;
	}
	else
	{
	//	std::cout << key << " tagging is not found in scene_actor_tagging." << std::endl;
		return "";
	}
}

void SceneActor::SetNewSceneActorTagging(std::string key, std::string value)
{
	std::stringstream find_id;
	find_id.str(std::string());
	find_id << key;
	unsigned no = 1;
	bool unique = CheckSceneActorTaggingUniqueKey(find_id.str());

	while (!unique)
	{
		++no;
		find_id.str(std::string());
		find_id << key << "(" << no << ")";
		unique = CheckSceneActorTaggingUniqueKey(find_id.str());
	}
	scene_actor_tagging.emplace(find_id.str(), value);

	if (key.compare(find_id.str()) != 0)
	{
		std::cout << key << " have been auto name to " << find_id.str() << " as " << key << " already existed in scene actor tagging." << std::endl;
	}
}

bool SceneActor::ChangeExistedSceneActorTaggingValue(std::string key, std::string value)
{
	if (scene_actor_tagging.find(key) != scene_actor_tagging.end())
	{
		scene_actor_tagging.find(key)->second = value;
		return true;
	}
	else
	{
		std::cout << key << ":key not found in scene_actor_tagging." << std::endl;
		return false;
	}
}

bool SceneActor::ChangeExistedSceneActorTaggingKey(std::string oldkey, std::string newkey)
{
	if (scene_actor_tagging.find(newkey) == scene_actor_tagging.end())
	{
		if (scene_actor_tagging.find(oldkey) != scene_actor_tagging.end())
		{
			std::string back_up_value = scene_actor_tagging.find(oldkey)->second;
			scene_actor_tagging.emplace(newkey, back_up_value);
			scene_actor_tagging.erase(oldkey);
			return true;
		}
		else
		{
			std::cout << "Error cannot find old key" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << newkey << "'s key already existed scene_actor_tagging." << std::endl;
		return false;
	}
}

std::map<unsigned, SceneActor *> & SceneActor::GetChildSceneActors()
{
	return child_scene_actor_container;
}

std::map<unsigned, SceneActor *> * SceneActor::GetChildSceneActorsPtr()
{
	return &child_scene_actor_container;
}

void SceneActor::AddChildSceneActor(SceneActor * child_actor)
{
	std::string name_id = child_actor->GetSceneActorName();
	std::stringstream find_id;
	find_id.str(std::string());
	find_id << name_id;
	unsigned no = 1;

	bool unique = CheckSceneActorUniqueName(&child_scene_actor_container, find_id.str());

	while (!unique)
	{
		++no;
		find_id.str(std::string());
		find_id << name_id << "(" << no << ")";
		unique = CheckSceneActorUniqueName(&child_scene_actor_container, find_id.str());
	}

	child_actor->SetSceneActorName(find_id.str());
	child_scene_actor_container.emplace(std::make_pair(child_scene_actor_key, child_actor));
	++child_scene_actor_key;
}

void SceneActor::EmptyChildSceneActors(SceneActor * actor)
{
	if (actor->GetChildSceneActorsPtr()->size() > 0)
	{
	    for (auto it = actor->GetChildSceneActorsPtr()->begin(); it != actor->GetChildSceneActorsPtr()->end(); ++it)
		{
			EmptyChildSceneActors(it->second);
			delete it->second->GetPrefabActor();
			it->second->SetPrefabActor(nullptr);
			it->second->GetSceneActorTagging().clear();
			delete it->second;
			it->second = nullptr;
		}
		actor->GetChildSceneActorsPtr()->clear();
	}
	
}

PrefabActor * SceneActor::GetPrefabActor()
{
   return prefab_actor_object;
}


PrefabActor * SceneActor::GetPrefabActorConst() const
{
	return prefab_actor_object;
}

void SceneActor::SetPrefabActor(PrefabActor * ptr)
{
	prefab_actor_object = ptr;
}

void SceneActor::SetParentSceneActorPointer(SceneActor * ptr)
{
	parent_scene_actor = ptr;
}

SceneActor * SceneActor::GetParentSceneActorPointer()
{
	return parent_scene_actor;
}

SceneActor * SceneActor::GetChildByName(std::string name)
{
	for (auto & child : child_scene_actor_container)
	{
		if (child.second->GetSceneActorName() == name)
			return child.second;
	}
	return nullptr;
}

void SceneActor::UpdateParentSceneActorPtr(SceneActor * parent_actor, SceneActor * child_actor)
{
	child_actor->SetParentSceneActorPointer(parent_actor);
	auto & child_scene_actor = child_actor->GetChildSceneActors();
	for (auto it = child_scene_actor.begin(); it != child_scene_actor.end(); ++it)
	{
		UpdateParentSceneActorPtr(child_actor, it->second);
	}
}

Component * SceneActor::GetComponent(ComponentType type)
{
	auto map = prefab_actor_object->GetComponents();
	auto it = map.find(type);
	if (it != map.end()) 
		return it->second;
	else 
		return nullptr;
}

bool SceneActor::CheckSceneActorTaggingUniqueKey(std::string key)
{
	std::string search_key = key;
	for (auto it = scene_actor_tagging.begin(); it != scene_actor_tagging.end(); ++it)
	{
		if (search_key.compare(it->second) == 0)
			return false;
	}
	return true;
}

void SceneActor::ClearSceneActorSelection()
{
	SetSelection(false);
	for (auto it = child_scene_actor_container.begin(); it != child_scene_actor_container.end(); ++it)
	{
		ClearSceneActorSelection(it->second);
	}
}

void SceneActor::ClearSceneActorSelection(SceneActor * child_actor)
{
	child_actor->SetSelection(false);
	for (auto it = child_actor->GetChildSceneActors().begin(); it != child_actor->GetChildSceneActors().end(); ++it)
	{
		ClearSceneActorSelection(it->second);
	}
}

void SceneActor::SelectWithControlKey()
{
	bool value = !GetSelection();

	if (value == true)
	{
		SetSceneActorSelection(value);
		
	}
	else
	{
		SetSelection(value);
	}

}

void SceneActor::SetSceneActorSelection(bool value)
{
	SetSelection(value);
	for (auto it = child_scene_actor_container.begin(); it != child_scene_actor_container.end(); ++it)
	{
		SetSceneActorSelection(value, it->second);
	}

}

void SceneActor::SetSceneActorSelection(bool value, SceneActor * child_actor)
{
	child_actor->SetSelection(value);
	for (auto it = child_actor->GetChildSceneActors().begin(); it != child_actor->GetChildSceneActors().end(); ++it)
	{
		SetSceneActorSelection(value, it->second);
	}
}

bool SceneActor::GetSceneActorSelection() const
{
	return GetSelection();
}

bool SceneActor::GetActive()const
{
	return active;
}

void SceneActor::SetOnlyActive(bool value)
{
	active = value;
}

void SceneActor::SetActive(bool value)
{
	active = value;
	for (auto it = child_scene_actor_container.begin(); it != child_scene_actor_container.end(); ++it)
	{
		SetActive(value, it->second);
	}
}

bool SceneActor::GetInvul() const
{
	return invul;
}

void SceneActor::SetInvul(bool value)
{
	invul = value;
}

float SceneActor::GetTimer() const
{
	return timer;
}

void SceneActor::SetTimer(float value)
{
	timer = value;
}

void SceneActor::IncTimer()
{
	timer += 2.0f;
}

void SceneActor::SetDead(bool value)
{
	std::cout << isDead;
	isDead = value;
}

bool SceneActor::GetDead()
{
	return isDead;
}



void SceneActor::SetActive(bool value, SceneActor * child_actor)
{
	child_actor->SetActive(value);
	for (auto it = child_actor->GetChildSceneActors().begin(); it != child_actor->GetChildSceneActors().end(); ++it)
	{
		SetActive(value, it->second);
	}
}

unsigned * SceneActor::GetCurrentKeyIDPtr()
{
	return &child_scene_actor_key;
}


bool CheckSceneActorUniqueName(std::map<unsigned, SceneActor *> * container, std::string search)
{
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_name_id = it->second->GetSceneActorName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
			return false;

	}
	return true;
}

unsigned GetSceneActorKeyByName(std::map<unsigned, SceneActor *> * container, std::string search)
{
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_name_id = it->second->GetSceneActorName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
			return it->first;
	}
	return 0;
}