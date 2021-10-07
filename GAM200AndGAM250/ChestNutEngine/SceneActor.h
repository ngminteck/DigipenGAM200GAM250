/******************************************************************************/
/*!
\file  SceneActor.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the scene actor object, for level


All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include "PrefabActor.h"
#include <sol.hpp>
static std::string scene_actor_name_key = "scene_actor_name_key";
static std::string scene_actor_prefab_type_key = "scene_actor_prefab_type_key";
class SceneActor : private Selection
{
public:
	SceneActor();
	SceneActor(PrefabActor * prefab_actor, SceneActor * parent_sceneactor = nullptr);
	SceneActor(const SceneActor & rhs, bool copy_child = true);
	~SceneActor();

	std::string GetSceneActorName() const;
	Component * GetComponent(ComponentType);
	void SetSceneActorName(std::map<unsigned, SceneActor *> * scene_container, std::string name);
	void SetSceneActorName(std::string name);

	std::string GetSceneActorPrefabType() const;
	void SetSceneActorPrefabTYpe(std::string name);

	std::unordered_map<std::string, std::string> & GetSceneActorTagging();
	std::string GetSceneActorTaggingValue(std::string key);
	void SetNewSceneActorTagging(std::string key, std::string value);
	bool ChangeExistedSceneActorTaggingValue(std::string key, std::string value);
	bool ChangeExistedSceneActorTaggingKey(std::string oldkey, std::string newkey);

	void AddChildSceneActor(SceneActor * child_actor);
	void EmptyChildSceneActors(SceneActor * actor);
	std::map<unsigned, SceneActor *> & GetChildSceneActors();
	std::map<unsigned, SceneActor *> * GetChildSceneActorsPtr();

	void SetParentSceneActorPointer(SceneActor * ptr);
	SceneActor * GetParentSceneActorPointer();

	SceneActor * GetChildByName(std::string name);

	PrefabActor * GetPrefabActor();
	PrefabActor * GetPrefabActorConst() const;
	void SetPrefabActor(PrefabActor * ptr);
	
	void ClearSceneActorSelection();
	void SelectWithControlKey();
	void SetSceneActorSelection(bool value);
	bool GetSceneActorSelection() const;
	
	//Active
	bool GetActive()const;
	void SetOnlyActive(bool value);
	void SetActive(bool value);

	//Invulnerability
	bool GetInvul() const;
	void SetInvul(bool);

	//Timer
	float GetTimer() const;
	void SetTimer(float);
	void IncTimer();

	//Dead
	void SetDead(bool);
	bool GetDead();

	unsigned * GetCurrentKeyIDPtr();

	bool * GetActivePtr() { return &active; }

	static void AddToScript(sol::state* luaState)
	{
		luaState->new_usertype<SceneActor>("SceneActor",
			"GetSceneActorTaggingValue",&SceneActor::GetSceneActorTaggingValue,
			"SetNewSceneActorTagging", &SceneActor::SetNewSceneActorTagging,
			"ChangeExistedSceneActorTaggingValue",&SceneActor::ChangeExistedSceneActorTaggingValue,
			"ChangeExistedSceneActorTaggingKey",&SceneActor::ChangeExistedSceneActorTaggingKey,

			"GetChildSceneActor",&SceneActor::GetChildSceneActorsPtr,


			"GetActive", &SceneActor::GetActive,
			"SetSelfActive",&SceneActor::SetOnlyActive,
			"SetActive", sol::resolve<void(bool)>(&SceneActor::SetActive),

			"SetChildActive",sol::resolve<void(bool,SceneActor*)>(&SceneActor::SetActive),

			"GetPrefab",&SceneActor::GetPrefabActor,
			"GetChild", &SceneActor::GetChildByName,

			"GetTimer", &SceneActor::GetTimer,
			"SetTimer", &SceneActor::SetTimer,
			"IncTimer", &SceneActor::IncTimer,
			"GetInvul", &SceneActor::GetInvul,
			"SetInvul", &SceneActor::SetInvul,

			"GetDead", &SceneActor::GetDead,
			"SetDead", &SceneActor::SetDead,

			"GetSceneActorName", &SceneActor::GetSceneActorName
			);
	}
private:
	unsigned child_scene_actor_key;
	
	SceneActor* parent_scene_actor;
	PrefabActor* prefab_actor_object;

	std::map<unsigned, SceneActor *> child_scene_actor_container;
	std::unordered_map<std::string, std::string> scene_actor_tagging;


	bool CheckSceneActorTaggingUniqueKey(std::string key);

	void UpdateParentSceneActorPtr(SceneActor * parent_actor, SceneActor * child_actor);

	void ClearSceneActorSelection(SceneActor * child_actor);
	void SetSceneActorSelection(bool value, SceneActor * child_actor);

	void SetActive(bool value, SceneActor * child_actor);

	float timer;
	bool active;
	bool invul;
	bool isDead;
};

bool CheckSceneActorUniqueName(std::map<unsigned, SceneActor *> * container, std::string search);
unsigned GetSceneActorKeyByName(std::map<unsigned, SceneActor *> * container, std::string search);