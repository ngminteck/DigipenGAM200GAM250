/******************************************************************************/
/*!
\file  GameObject.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains  the prototypes for the GameObject class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <unordered_map>
#include <sstream>
#include <string>
#include <map>
#include "Component.h"
#include "TransformComponent.h"
#include "StatsComponent.h"
#include "AABBComponent.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "StatsComponent.h"
#include "AudioComponent.h"
#include "GameObjectType.h"

#include "HealthEvent.h"

#include <sstream>




class GameObject
{

public:

	GameObject(ObjectType = DEFAULT);
	GameObject(unsigned, ObjectType = DEFAULT);
	GameObject(const GameObject &rhs);

	~GameObject();

	/******************* Getters and Setters ***********************/

	//Components
	void AddComponent(Component*);
	void EmptyComponents();
	std::unordered_map<ComponentType, Component *> & GetComponents();
	std::unordered_map<ComponentType, Component *> * GetComponentsPtr();

	
	//ParentObject
	GameObject * GetParentObject() const;
	void SetParentObject(GameObject* parent);
	GameObject * GetMostParentObject(GameObject * obj) const;




	//Clone Components
	std::unordered_map<ComponentType, Component *> * CloneComponents() const;
	void SetPointer(GameObject* parent_obj, GameObject* myself_obj);

	/****Object data******/
	ObjectType GetType() const;
	
	//Name
	std::string GetIDName() const;
	void SetIDName(const std::string & id_name);

	//Archetype
	std::string GetArcheType() const;
	void SetArcheType(std::string);

	//Archetype path
	std::string GetPath() const;
	void SetPath(const std::string & path);

	//Level Editor
	bool GetSelection() const;
	void SetSelection();
	void ClearSelection();
	

private:
	//Components
	std::unordered_map<ComponentType, Component*> m_ObjComponents;

	//Object info
	std::unordered_map<unsigned, GameObject *> m_ChildObj;
	unsigned childobj_id;
	ObjectType m_ObjType;
	std::string m_ObjIDName;
	std::string m_ObjPath;
	GameObject * m_parent_obj;
	std::string m_archeType;

	constexpr static Metadata < GameObject, std::unordered_map<ComponentType, Component*> GameObject::*,
		ObjectType GameObject:: *, std::string GameObject:: *, std::string GameObject:: *, std::string GameObject:: *, std::unordered_map<unsigned, GameObject *> GameObject::* >  metadata
	{ "Game Object", std::make_tuple(&GameObject::m_ObjComponents, &GameObject::m_ObjType, &GameObject::m_ObjIDName, &GameObject::m_ObjPath,
									 &GameObject::m_archeType, &GameObject::m_ChildObj),
					std::make_tuple("m_ObjComponents", "m_ObjType", "m_ObjIDName", "m_ObjPath", "m_archeType", "m_ChildObj" ) 
	};
	
	//For level editor
	bool selected;

	void ClearChildSelection(std::unordered_map<unsigned, GameObject *> child_object);
	void SetChildSelection(std::unordered_map<unsigned, GameObject *> child_object, bool selection);
	void SetSelection(bool selection);
};