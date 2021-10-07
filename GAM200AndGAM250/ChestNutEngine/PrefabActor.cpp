/******************************************************************************/
/*!
\file   PrefabActor.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the prefab actor object, which something like the base component
and handle all additional component by adding to remove, it also have the tagging
system on it

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "PrefabActor.h"

PrefabActor::PrefabActor()
:DataFileInfo(),Selection(), child_prefab_actor_key{1}, parent_prefab_actor{nullptr}, active{true}
{
	std::string object_type_key = "object_type_key";
	std::string object_type_value = "";
	prefab_actor_tagging.emplace(object_type_key, object_type_value);
	std::string prefab_name_key = "prefab_name_key";
	std::string prefab_name_value = "";
	prefab_actor_tagging.emplace(prefab_name_key, prefab_name_value);
}

PrefabActor::PrefabActor(std::string prefabname, std::string prefabtype, PrefabActor * parent_actor)
:DataFileInfo(), Selection(), child_prefab_actor_key{ 1 }, parent_prefab_actor{ parent_actor }, active{ true }
{
	std::string object_type_key = "object_type_key";
	std::string object_type_value = prefabtype;
	prefab_actor_tagging.emplace(object_type_key, object_type_value);
	std::string prefab_name_key = "prefab_name_key";
	std::string prefab_name_value = prefabname;
	prefab_actor_tagging.emplace(prefab_name_key, prefab_name_value);

}

PrefabActor::PrefabActor(std::string path, std::string filename, std::string objecttype, PrefabActor * parent_actor)
:DataFileInfo(path, filename), Selection(), child_prefab_actor_key{1}, parent_prefab_actor{ parent_actor}, active{ true }
{
	std::string object_type_key = "object_type_key";
	std::string object_type_value = objecttype;
	prefab_actor_tagging.emplace(object_type_key, object_type_value);
	std::string prefab_name_key = "prefab_name_key";
	// filename is surely unique in this context 
	// unless allow user load from different path and filename will be not unique
	std::string prefab_name_value = filename.substr(0,filename.find("."));
	prefab_actor_tagging.emplace(prefab_name_key, prefab_name_value);
}

PrefabActor::PrefabActor(const PrefabActor & rhs , bool copy_child)
:DataFileInfo(rhs), Selection(), child_prefab_actor_key{ 1}, parent_prefab_actor{nullptr}, active{ true }
{
	//std::cout << "Cloning object " << rhs.GetPrefabActorName() << std::endl;
	for (auto itor = rhs.m_ObjComponents.begin(); itor != rhs.m_ObjComponents.end(); ++itor)
	{
		
		//std::cout << "Cloning " << checkComponentType(itor->first) << std::endl;

		if (itor->second == nullptr)
		{
			std::cout << checkComponentType(itor->first) << " is nullptr" << std::endl;
			//Component * comp = itor->second->Clone();
			//m_ObjComponents.emplace(itor->first, comp);
		}
		else
		{
			Component * comp = itor->second->Clone();
			m_ObjComponents.emplace(itor->first, comp);
		}

		//std::cout << "Success Cloning " << checkComponentType(itor->first) << std::endl;
	}

	for (auto tagging_it = rhs.prefab_actor_tagging.begin(); tagging_it != rhs.prefab_actor_tagging.end(); ++tagging_it)
	{
		prefab_actor_tagging.emplace(tagging_it->first, tagging_it->second);
	}
	
	if (copy_child)
	{
		for (auto it = rhs.child_prefab_actor_container.begin(); it != rhs.child_prefab_actor_container.end(); ++it, ++child_prefab_actor_key)
		{
			PrefabActor * child_prefab = new PrefabActor(*(it->second), copy_child);
			// link parent actor pointer
			child_prefab->SetParentPrefabActorPointer(this);

			auto parent_comp_map = m_ObjComponents;
			auto child_comp_map = child_prefab->GetComponents();
			// link parent and child transfrom comp pointer
			if (parent_comp_map.find(ComponentType::TransformComponent) != parent_comp_map.end() && child_comp_map.find(ComponentType::TransformComponent) != child_comp_map.end())
			{
				TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp_map.find(ComponentType::TransformComponent)->second);
				TransformComponent* trans = dynamic_cast<TransformComponent*>(child_comp_map.find(ComponentType::TransformComponent)->second);
				trans->SetParentTransformComponent(parent_trans);
			}
			child_prefab_actor_container.emplace(child_prefab_actor_key, child_prefab);
		}
	}
}


PrefabActor::~PrefabActor()
{
	//std::cout << std::endl;
	//std::cout << "Deleting... " << GetPrefabActorName() << std::endl;
	if (child_prefab_actor_container.size() > 0)
	{
		for (auto it = child_prefab_actor_container.begin(); it != child_prefab_actor_container.end(); ++it)
		{
			EmptyChildPrefabActors(it->second);
			it->second->SetParentPrefabActorPointer(nullptr);
			delete it->second;
			it->second = nullptr;
		}
	}
	child_prefab_actor_container.clear();
	EmptyComponents();
	prefab_actor_tagging.clear();
	
}

std::string PrefabActor::GetPrefabTaggingValue(std::string key) const
{
	if (prefab_actor_tagging.find(key) != prefab_actor_tagging.end())
	{
		return prefab_actor_tagging.find(key)->second;
	}
	else
	{
		std::cout << key << " tagging is not found in prefab tagging." << std::endl;
		return "";
	}
		
}

std::string PrefabActor::GetPrefabActorName() const
{
	return prefab_actor_tagging.find("prefab_name_key")->second;
}

std::string PrefabActor::GetObjectType() const
{
	return prefab_actor_tagging.find("object_type_key")->second;
}

void PrefabActor::SetObjectType(std::string value)
{
	prefab_actor_tagging.find("object_type_key")->second = value;
}

void PrefabActor::SetPrefabActorName(std::string value)
{
	prefab_actor_tagging.find("prefab_name_key")->second = value;

	std::string filename = value + ".json";
	SetFileName(filename);
	std::string path = GetPath();
	std::size_t found = path.find_last_of("/\\");
	path = path.substr(0, found) + "/" + value + ".json";
	SetPath(path);
}

void PrefabActor::SetNewPrefabActorTagging(std::string key, std::string value)
{
	std::stringstream find_id;
	find_id.str(std::string());
	find_id << key;
	unsigned no = 1;
	bool unique = CheckPrefabActorTaggingUniqueKey(find_id.str());

	while (!unique)
	{
		++no;
		find_id.str(std::string());
		find_id << key << "(" << no << ")";
		unique = CheckPrefabActorTaggingUniqueKey(find_id.str());
	}

	prefab_actor_tagging.emplace(find_id.str(), value);

	if (key.compare(find_id.str()) != 0)
	{
		std::cout << key << " have been auto name to " << find_id.str() << " as " << key << " already existed in prefab actor tagging." << std::endl;
	}
	
}

bool PrefabActor::ChangeExistedPrefabTaggingValue(std::string key, std::string value)
{
	if (prefab_actor_tagging.find(key) != prefab_actor_tagging.end())
	{
		prefab_actor_tagging.find(key)->second = value;
		return true;
	}
	else
	{
		std::cout << key << ":key not found in prefab_actor_tagging." << std::endl;
		return false;
	}
}

bool PrefabActor::ChangeExistedPrefabTaggingKey(std::string oldkey, std::string newkey)
{
	if (prefab_actor_tagging.find(newkey) == prefab_actor_tagging.end())
	{
		if (prefab_actor_tagging.find(oldkey) != prefab_actor_tagging.end())
		{
			std::string back_up_value = prefab_actor_tagging.find(oldkey)->second;
			prefab_actor_tagging.emplace(newkey, back_up_value);
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
		std::cout << newkey << "'s key already existed in prefab_actor_tagging." << std::endl;
		return false;
	}
}

void PrefabActor::SetPrefabActorName(std::unordered_map<unsigned, PrefabActor *> * prefab_container, std::string name)
{
	std::string name_id = name;
	std::stringstream find_id;
	find_id.str(std::string());
	find_id << name_id;
	unsigned no = 1;

	bool unique = CheckPrefabUniqueName(prefab_container, find_id.str());

	while (!unique)
	{
		++no;
		find_id.str(std::string());
		find_id << name_id << "(" << no << ")";
		unique = CheckPrefabUniqueName(prefab_container, find_id.str());
	}

	SetPrefabActorName(find_id.str());

	if (find_id.str().compare(name) != 0)
		std::cout << name << " already existed, it will be rename as " << find_id.str() << " instead." << std::endl;
}

std::unordered_map<std::string, std::string> & PrefabActor::GetPrefabActorTagging()
{
	return prefab_actor_tagging;
}

std::unordered_map<ComponentType, Component *> * PrefabActor::CloneComponents() const
{
	std::unordered_map<ComponentType, Component *> * map = new std::unordered_map<ComponentType, Component *>{};

	for (auto elem : m_ObjComponents) // this is instance obj components not the object components
	{
		map->insert(std::make_pair(elem.first, elem.second->Clone()));
	}
	return map;
}

void PrefabActor::AddComponent(Component* comp)
{
	m_ObjComponents.insert(std::make_pair(comp->GetComponentType(), comp));
	return;
}

void PrefabActor::EmptyComponents()
{
	for (auto it = m_ObjComponents.begin(); it != m_ObjComponents.end(); ++it)
	{
		//std::cout << "Deleting " << checkComponentType(it->first) << std::endl;
		if (it->first == ComponentType::TransformComponent)
		{
			TransformComponent* trans = dynamic_cast<TransformComponent*>(it->second);
			trans->SetParentTransformComponent(nullptr);
		}
		delete it->second;
		it->second = nullptr;
	}
	m_ObjComponents.clear();
}

void PrefabActor::EmptyComponents(PrefabActor * actor)
{
	auto actor_comp = actor->GetComponentsPtr();
	for (auto it = actor_comp->begin() ; it != actor_comp->end(); ++it)
	{
		if (it->first == ComponentType::TransformComponent)
		{
			TransformComponent* trans = dynamic_cast<TransformComponent*>(it->second);
			trans->SetParentTransformComponent(nullptr);
		}
		delete it->second;
		it->second = nullptr;
	}
	actor_comp->clear();
}

std::unordered_map<ComponentType, Component*> & PrefabActor::GetComponents()
{
	return m_ObjComponents;
}

std::unordered_map<ComponentType, Component*>* PrefabActor::GetComponentsPtr()
{
	return &m_ObjComponents;
}

std::unordered_map<unsigned, PrefabActor *> & PrefabActor::GetChildPrefabActors()
{
	return child_prefab_actor_container;
}

std::unordered_map<unsigned, PrefabActor *> * PrefabActor::GetChildPrefabActorsPtr()
{
	return &child_prefab_actor_container;
}

void PrefabActor::AddChildPrefabActor(PrefabActor * child_actor)
{
	std::string name_id = child_actor->GetPrefabActorName();
	std::stringstream find_id;
	find_id.str(std::string());
	find_id << name_id;
	unsigned no = 1;

	bool unique = CheckPrefabUniqueName(&child_prefab_actor_container, find_id.str());

	while (!unique)
	{
		++no;
		find_id.str(std::string());
		find_id << name_id << "(" << no << ")";
		unique = CheckPrefabUniqueName(&child_prefab_actor_container, find_id.str());
	}

	child_actor->SetPrefabActorName(find_id.str());
	child_prefab_actor_container.emplace(std::make_pair(child_prefab_actor_key, child_actor));
	++child_prefab_actor_key;
}

void PrefabActor::EmptyChildPrefabActors(PrefabActor *child)
{
	if (child->GetChildPrefabActors().size() > 0)
	{
		for (auto it = child->GetChildPrefabActorsPtr()->begin(); it != child->GetChildPrefabActorsPtr()->end(); ++it)
		{
			EmptyChildPrefabActors(it->second);
			it->second->SetParentPrefabActorPointer(nullptr);
			delete it->second;
			it->second = nullptr;
		}
		child->GetChildPrefabActors().clear();
	}
	EmptyComponents(child);
	child->GetPrefabActorTagging().clear();

}


void PrefabActor::SetParentPrefabActorPointer(PrefabActor * ptr)
{
	parent_prefab_actor = ptr;
}

void PrefabActor::UpdateParentPrefabActorPtr(PrefabActor * parent_actor, PrefabActor * child_actor)
{
	child_actor->SetParentPrefabActorPointer(parent_actor);
	auto & child_prefab_actor = child_actor->GetChildPrefabActors();
	for (auto it = child_prefab_actor.begin(); it != child_prefab_actor.end(); ++it)
	{
		UpdateParentPrefabActorPtr(child_actor, it->second);
	}
}

bool PrefabActor::CheckPrefabActorTaggingUniqueKey(std::string key)
{
	std::string search_key = key;
	for (auto it = prefab_actor_tagging.begin(); it != prefab_actor_tagging.end(); ++it)
	{
		if (search_key.compare(it->second) == 0)
			return false;
	}
	return true;
}

bool PrefabActor::GetPrefabActorSelection() const
{
	return GetSelection();
}

void PrefabActor::ClearPrefabActorSelection()
{
	SetSelection(false);
	for (auto it = child_prefab_actor_container.begin(); it != child_prefab_actor_container.end(); ++it)
	{
		ClearPrefabActorSelection(it->second);
	}
}

void PrefabActor::ClearPrefabActorSelection(PrefabActor * child_actor)
{
	child_actor->SetSelection(false);
	for (auto it = child_actor->GetChildPrefabActors().begin(); it != child_actor->GetChildPrefabActors().end(); ++it)
	{
		ClearPrefabActorSelection(it->second);
	}
}

void PrefabActor::SetPrefabActorSelection(bool value)
{
	SetSelection(value);
	for (auto it = child_prefab_actor_container.begin(); it != child_prefab_actor_container.end(); ++it)
	{
		SetPrefabActorSelection(value,it->second);
	}
}

void PrefabActor::SelectWithControlKey()
{
	bool value = !GetSelection();

	if (value == true)
	{
		SetPrefabActorSelection(value);
	}
	else
	{
		SetSelection(value);
	}
}

void PrefabActor::SetPrefabActorSelection(bool value, PrefabActor * child_actor)
{
	child_actor->SetSelection(value);
	for (auto it = child_actor->GetChildPrefabActors().begin(); it != child_actor->GetChildPrefabActors().end(); ++it)
	{
		SetPrefabActorSelection(value,it->second);
	}
}

bool PrefabActor::GetActive()const
{
	return active;
}

bool* PrefabActor::GetActivePtr()
{
	return &active;
}

void PrefabActor::SetOnlyActive(bool value)
{
	active = value;
}

void PrefabActor::SetActive(bool value)
{
	active = value;
	for (auto it = child_prefab_actor_container.begin(); it != child_prefab_actor_container.end(); ++it)
	{
		SetActive(value, it->second);
	}
	
}

void PrefabActor::SetActive(bool value, PrefabActor * child_actor)
{
	child_actor->SetActive(value);
	for (auto it = child_actor->GetChildPrefabActors().begin(); it != child_actor->GetChildPrefabActors().end(); ++it)
	{
		SetActive(value, it->second);
	}
	
}

PrefabActor * PrefabActor::GetParentPrefabActorPointer()
{
	return parent_prefab_actor;
}

bool CheckPrefabUniqueName(std::unordered_map<unsigned, PrefabActor *> * container, std::string search)
{
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_name_id = it->second->GetPrefabActorName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
			return false;

	}
	return true;
}

unsigned GetPrefabKeyByName(std::unordered_map<unsigned, PrefabActor *> * container, std::string search)
{
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_name_id = it->second->GetPrefabActorName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
			return it->first;
	}
	return 0;
}

bool CheckPrefabActorFileLoaded(std::unordered_map<unsigned, PrefabActor *> * container, std::string path)
{
	std::string search_path = path;
	std::transform(search_path.begin(), search_path.end(), search_path.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_path = it->second->GetPath();
		std::transform(container_path.begin(), container_path.end(), container_path.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_path.compare(container_path) == 0)
			return true;
	}
	return false;
}

unsigned GetPrefabKeyByPath(std::unordered_map<unsigned, PrefabActor *> * container, std::string path)
{
	std::string search_path = path;
	std::transform(search_path.begin(), search_path.end(), search_path.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_name_id = it->second->GetPath();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_path.compare(container_name_id) == 0)
			return it->first;
	}
	// 0 is default prefab from the prefab actor factory
	return 0;
}