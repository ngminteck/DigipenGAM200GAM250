/******************************************************************************/
/*!
\file  GameObject.cpp

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains  the prototypes for the HealthSystem class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "GameObject.h"


GameObject::GameObject(ObjectType type)
	:m_ObjType{ type }, selected{ false }, childobj_id{ 1 }
{
	m_ObjPath = "";
	m_ObjIDName = "";
	m_parent_obj = nullptr;
	
}

GameObject::GameObject(unsigned id, ObjectType type)
	:m_ObjType{ type }, selected{ false }, childobj_id{ 1 }
{
	std::stringstream ss;
	ss << id;
	m_ObjPath = "";
	m_ObjIDName = "GameObject " + ss.str();
	m_parent_obj = nullptr;
}

GameObject::GameObject(const GameObject &rhs)
	:m_ObjType{ rhs.m_ObjType }, m_ObjIDName{ rhs.m_ObjIDName }, m_ObjPath{ rhs.m_ObjPath }
	, m_parent_obj{ nullptr }, m_archeType{ rhs.m_archeType }, selected{ false }, childobj_id{ 1 }
{

	//Deep copy components
	for (auto itor = rhs.m_ObjComponents.begin(); itor != rhs.m_ObjComponents.end(); ++itor)
	{
		m_ObjComponents[itor->first] = itor->second->Clone();
	}

}

void GameObject::SetPointer(GameObject* parent_obj, GameObject* myself_obj)
{
	myself_obj->SetParentObject(parent_obj);

	
}

 GameObject::~GameObject()
{
	EmptyComponents();

}

std::string GameObject::GetIDName() const
{
	return m_ObjIDName;
}

void GameObject::SetIDName(const std::string & id_name)
{
	m_ObjIDName = id_name;
}

std::string GameObject::GetPath() const
{
	return m_ObjPath;
}


void GameObject::SetPath(const std::string & path)
{
	m_ObjPath = path;
}
std::unordered_map<ComponentType, Component *> * GameObject::CloneComponents() const
{
	std::unordered_map<ComponentType, Component *> * map= new std::unordered_map<ComponentType, Component *>{};

	for (auto elem : m_ObjComponents) // this is instance obj components not the object components
	{
		map->insert(std::make_pair(elem.first, elem.second->Clone()));
	}

	return map;
}

ObjectType GameObject::GetType() const
{
	return m_ObjType;
}

GameObject * GameObject::GetParentObject() const
{
	return m_parent_obj == nullptr ? nullptr: m_parent_obj;
}

void GameObject::SetParentObject(GameObject* parent)
{
	m_parent_obj = parent;
}

void GameObject::AddComponent(Component* comp)
{
	m_ObjComponents.insert(std::make_pair(comp->GetComponentType(), comp ));
	return;
}

void GameObject::EmptyComponents()
{
	for (auto it = m_ObjComponents.begin(); it != m_ObjComponents.end(); ++it)
	{
		delete it->second;
	}

	m_ObjComponents.clear();
}

std::unordered_map<ComponentType, Component*> & GameObject::GetComponents() 
{ 
	return m_ObjComponents;
}

std::unordered_map<ComponentType, Component*>* GameObject::GetComponentsPtr()
{
	return &m_ObjComponents;
}


bool GameObject::GetSelection() const
{
	return selected;
}

void GameObject::SetSelection()
{
	selected = !selected;
	
}

void GameObject::SetSelection(bool selection)
{
	selected = selection;
}

void GameObject::SetChildSelection(std::unordered_map<unsigned, GameObject *> child_object, bool selection)
{
	for (auto it = child_object.begin(); it != child_object.end(); ++it)
	{
		it->second->SetSelection(selection);
		
	}
}


GameObject * GameObject::GetMostParentObject(GameObject * obj) const
{
	GameObject * top_parent = GetParentObject();

	// already the top parent so return back the pointer of obj
	if (top_parent == nullptr)
	{
		std::cout << obj->GetIDName() << std::endl;
		return obj;
	}
	else
	{
		GameObject * tmp = top_parent;
		// search until not nullptr
		while (tmp->GetParentObject() != nullptr)
		{
			top_parent = top_parent->GetParentObject();
			tmp = top_parent;
		}
		tmp = nullptr;
		std::cout << top_parent->GetIDName() << std::endl;
		return top_parent;
	}
}


void GameObject::ClearSelection()
{
	selected = false;

}

void GameObject::ClearChildSelection(std::unordered_map<unsigned, GameObject *> child_object)
{
	for (auto it = child_object.begin(); it != child_object.end(); ++it)
	{
		(*it).second->ClearSelection();
		
	}
}


std::string GameObject::GetArcheType() const { return m_archeType; }
void GameObject::SetArcheType(std::string value) { m_archeType = value; }
