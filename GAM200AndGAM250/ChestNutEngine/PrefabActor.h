/******************************************************************************/
/*!
\file   PrefabActor.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
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

//All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
//reserved.
//*/
/******************************************************************************/
#pragma once
#include <sstream>

#include "DataFileInfo.h"
#include "Component.h"
#include "ComponentFactory.h"
#include "Selection.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"

#include "ButtonComponent.h"
#include "CameraComponent.h"


class PrefabActor : public DataFileInfo, private Selection
{
public:
	PrefabActor();
	// default prefab actor construct
	PrefabActor(std::string prefabname, std::string prefabtype, PrefabActor * parent_actor = nullptr);
	// read from json
	PrefabActor(std::string path, std::string filename , std::string prefabtype, PrefabActor * parent_actor = nullptr);
	PrefabActor(const PrefabActor & rhs, bool copy_child = true);
	~PrefabActor();

	std::string GetPrefabTaggingValue(std::string key) const;
	std::string GetObjectType() const;
	std::string GetPrefabActorName() const;
	void SetObjectType(std::string value);
	void SetPrefabActorName(std::unordered_map<unsigned, PrefabActor *> * prefab_container, std::string name);
	void SetPrefabActorName(std::string value);
	void SetNewPrefabActorTagging(std::string key, std::string value);
	bool ChangeExistedPrefabTaggingValue(std::string key, std::string value);
	bool ChangeExistedPrefabTaggingKey(std::string oldkey, std::string newkey);

	std::unordered_map<std::string,std::string> & GetPrefabActorTagging();
	
	void AddComponent(Component*);
	void EmptyComponents();
	void EmptyComponents(PrefabActor * actor);
	std::unordered_map<ComponentType, Component *> & GetComponents();
	std::unordered_map<ComponentType, Component *> * GetComponentsPtr();
	std::unordered_map<ComponentType, Component *> * CloneComponents() const;

	void AddChildPrefabActor(PrefabActor * child_actor);
	void EmptyChildPrefabActors(PrefabActor * child);
	std::unordered_map<unsigned, PrefabActor *> & GetChildPrefabActors();
	std::unordered_map<unsigned, PrefabActor *> * GetChildPrefabActorsPtr();

	void SetParentPrefabActorPointer(PrefabActor * ptr);
	PrefabActor * GetParentPrefabActorPointer();

	bool GetPrefabActorSelection() const;
	void ClearPrefabActorSelection();
	void SetPrefabActorSelection(bool value);
	void SelectWithControlKey();

	bool GetActive()const;
	bool* GetActivePtr();
	void SetOnlyActive(bool value);
	void SetActive(bool value);

	template<typename T>
	T* GetComponent()
	{
		if (typeid(T) == typeid(AABBComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::AABBComponent)->second);
		else if (typeid(T) == typeid(CollisionComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::CollisionComponent)->second);
		else if (typeid(T) == typeid(MovementComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::MovementComponent)->second);
		else if (typeid(T) == typeid(RigidBodyComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::RigidBodyComponent)->second);
		else if (typeid(T) == typeid(StatsComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::StatsComponent)->second);
		else if(typeid(T) == typeid(TransformComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::TransformComponent)->second);
		else if (typeid(T) == typeid(AudioComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::AudioComponent)->second);
		else if (typeid(T) == typeid(SpriteComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::SpriteComponent)->second);
		else if (typeid(T) == typeid(ButtonComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::ButtonComponent)->second);
		else if (typeid(T) == typeid(CameraComponent))
			return dynamic_cast<T*>(m_ObjComponents.find(ComponentType::CameraComponent)->second);

		return nullptr;
	}
	static void AddToScript(sol::state * luaState)
	{
		luaState->new_usertype<PrefabActor>
			(
				"PrefabActor",
				"GetAABBComponent", &PrefabActor::GetComponent<AABBComponent>,
				"GetCollisionComponent", &PrefabActor::GetComponent<CollisionComponent>,
				"GetMovementComponent", &PrefabActor::GetComponent<MovementComponent>,
				"GetRigidBodyComponent", &PrefabActor::GetComponent<RigidBodyComponent>,
				"GetStatsComponent",&PrefabActor::GetComponent<StatsComponent>,
				"GetTransformComponent", &PrefabActor::GetComponent<TransformComponent>,
				"GetSpriteComponent", &PrefabActor::GetComponent<SpriteComponent>,
				"GetCameraComponent", &PrefabActor::GetComponent<CameraComponent>
			);
	}


private:
	unsigned child_prefab_actor_key;
	PrefabActor* parent_prefab_actor;
	bool active;
	std::unordered_map<ComponentType, Component*> m_ObjComponents;
	std::unordered_map<unsigned, PrefabActor *> child_prefab_actor_container;
	std::unordered_map<std::string, std::string> prefab_actor_tagging;
	
	void UpdateParentPrefabActorPtr(PrefabActor * parent_actor, PrefabActor * child_actor);
	bool CheckPrefabActorTaggingUniqueKey(std::string key);
	void ClearPrefabActorSelection(PrefabActor * child_actor);
	void SetPrefabActorSelection(bool value, PrefabActor * child_actor);

	void SetActive(bool value, PrefabActor * child_actor);

};

bool CheckPrefabUniqueName(std::unordered_map<unsigned, PrefabActor *> * container, std::string search);
unsigned GetPrefabKeyByName(std::unordered_map<unsigned, PrefabActor *> * container, std::string search);

bool CheckPrefabActorFileLoaded(std::unordered_map<unsigned, PrefabActor *> * container, std::string path);
unsigned GetPrefabKeyByPath(std::unordered_map<unsigned, PrefabActor *> * container, std::string search);