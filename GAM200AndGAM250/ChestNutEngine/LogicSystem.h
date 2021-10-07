/******************************************************************************/
/*!
\file   LogicSystem.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of the LogicSystem.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "System.h"
#include "SceneActor.h"
#include "ScriptManager.h"
#include "AIScript.h"
#include "Input.h"
#include "Macros.h"	
#include "GameObject.h"
#include "ScriptComponent.h"
#include "PauseGame.h"
#include "DamageEvent.h"
#include "PlayLevel.h"
#include "WinLoseEvent.h"
#include "SwitchLevelEvent.h"
#include "PlayerDies.h"
#include "QuitGame.h"
#include "LoadGameState.h"

class LogicSystem : System
{
public:
	LogicSystem(EventHandler *, bool &);
	~LogicSystem();

	void InitScripts();
	void Update(std::map<unsigned, SceneActor *> & obj);
	void SetSceneFactory(std::map<unsigned, SceneActor *> * obj) { m_objs = obj; }
	void ReInitScripts(PlayLevel *);
	Profiler profiler;

private:
	void UpdatePlayer(std::map<unsigned, SceneActor *> & obj);
	void UpdateAllCooldown(std::map<unsigned, SceneActor *> & obj);
	
	void UpdatePlatform(std::map<unsigned, SceneActor *> & obj);
	void UpdateOnewayPlatform(TransformComponent* playerTrans, AABBComponent* playerAABB, RigidBodyComponent* onewayRB, AABBComponent* onewayAABB);
	void UpdateTimedPlatform(RigidBodyComponent* timedRB); // start time changeable // duration changeable // toggles based on starttime + duration
	void UpdateCrumblingPlatform(RigidBodyComponent* crumblingRB, CollisionComponent* playerCol); //start time always 0 // duration determines crumble time
	void UpdateTornadoPlatform(RigidBodyComponent* tornadoRB, RigidBodyComponent* playerRB, CollisionComponent* tornadoCol); // duration determines pulse time
	void UpdateMovingPlatform(RigidBodyComponent* movingRB, TransformComponent* movingTrans, TransformComponent* playerTrans, CollisionComponent* movingCol, AABBComponent* playerAABB, AABBComponent* movingAABB);
	void UpdatePitTraps(RigidBodyComponent* pitRB, StatsComponent* playerStat, CollisionComponent* playerCol);
	void UpdateSwitchGate(RigidBodyComponent * gateRB, AABBComponent* gateAABB, CollisionComponent* playerCol);
	void UpdateRespawnPlatform(StatsComponent * playerStats, CollisionComponent * playerCol, TransformComponent* respawnTrans);
	void UpdateStatsPlayer(StatsComponent* objStat, CollisionComponent* objCol, CollisionComponent* hitboxCol);
	void UpdateStatsEnemy(StatsComponent* objStat, CollisionComponent* objCol);
	SceneActor * FindPlayer(std::string, std::map<unsigned, SceneActor *> & objs);
	void PauseSystem(PauseGame *) { pause = !pause; }
	void ApplyDamage(DamageEvent* ev);
	void WinLose(WinLoseEvent* ev);

	std::unordered_map<ComponentType, Component*>* components;
	std::map<unsigned, SceneActor *> * m_objs;
	Vector2D vel;
	Vector2D accel;
	Vector2D velValk;
	EventHandler * m_ev;

	const float dt = 1.f / 60;

	bool & pause;
};