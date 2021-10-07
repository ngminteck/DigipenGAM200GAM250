#pragma once
#include "Prefab.h"
#include <sol.hpp>

class Entity
{
public:
	Entity();
	~Entity();

	//LUA
	static void AddToScript(sol::state* luaState)
	{
		/*
		luaState->new_usertype<Entity>("Entity",

			//"GetChildEntity", &Entity::GetChildEntitysPtr,

			"GetActive", &Entity::GetActive,
			//"SetSelfActive", &Entity::SetOnlyActive,
			"SetActive", sol::resolve<void(bool)>(&Entity::SetActive),

			"SetChildActive", sol::resolve<void(bool, Entity*)>(&Entity::SetActive),

			"GetPrefab", &Entity::GetPrefabActor,
			"GetChild", &Entity::GetChildByName,

			"GetTimer", &Entity::GetTimer,
			"SetTimer", &Entity::SetTimer,
			"IncTimer", &Entity::IncTimer,
			"GetInvul", &Entity::GetInvul,
			"SetInvul", &Entity::SetInvul,

			"GetDead", &Entity::GetDead,
			"SetDead", &Entity::SetDead
			);
			*/
	}

	//Active
	bool GetActive()const;
	void SetActive(bool);

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

private:
	
	Prefab * m_prefab;

	bool isActive;
	bool isDead;
	bool isInvul;
	float timer;

};