/******************************************************************************/
/*!
\file   LogicSystem.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of LogicSystem

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "LogicSystem.h"

LogicSystem::LogicSystem(EventHandler * ev, bool & p)
	:profiler{"Logic System"}, m_ev{ev}, pause{p}
{
	ev->SubscribeEvent(this, &LogicSystem::ApplyDamage);
	ev->SubscribeEvent(this, &LogicSystem::ReInitScripts);
	ev->SubscribeEvent(this, &LogicSystem::WinLose);
}

LogicSystem::~LogicSystem()
{

}

void LogicSystem::ReInitScripts(PlayLevel *)
{
	InitScripts();
}

void LogicSystem::InitScripts()
{
	ScriptComponent* scriptComp= nullptr;
	SceneActor* player=nullptr;

	if (m_objs->find(GetSceneActorKeyByName(m_objs, "Surtur")) != m_objs->end())
		player = m_objs->find(GetSceneActorKeyByName(m_objs, "Surtur"))->second;

	bool initthis, initthis2;
	// Getting obj which matches the required components.
	for (auto itor = m_objs->begin(); itor != m_objs->end(); ++itor)
	{
		initthis = itor->second->GetActive();
		scriptComp = nullptr;
		components = itor->second->GetPrefabActor()->GetComponentsPtr();
		if ((*components).find(ComponentType::ScriptComponent) == (*components).end())
			continue;
		scriptComp = dynamic_cast<ScriptComponent*>((*components)[ComponentType::ScriptComponent]);
		scriptComp->InitScripts(itor->second,player);
		initthis2 = itor->second->GetActive();
		if (initthis != initthis2)
			m_ev->PublishEvent(new SwitchLevelEvent());
	}
}

void LogicSystem::Update(std::map<unsigned, SceneActor*>& obj)
{
	if (!pause) 
	{
		profiler.UpdateStart();
		//UpdatePlatform(obj);
		UpdatePlayer(obj);
		UpdateAllCooldown(obj);
		profiler.UpdateEnd();
	}
}

void LogicSystem::UpdatePlayer(std::map<unsigned, SceneActor*>& obj)
{
	MovementComponent * moveComp;
	//TransformComponent* transComp;
	ScriptComponent* scriptComp;

	Script* script;
	sol::protected_function Updater;
	// EventHandler * eventHandler;
	// Getting obj which matches the required components.

	SceneActor * player = FindPlayer("Surtur", obj);
	if (player)
	{
		StatsComponent * hp = dynamic_cast<StatsComponent*>(player->GetComponent(ComponentType::StatsComponent));
		if (hp)
		{
			if (hp->GetHP() == 0.0f)
			{
				std::cout << "Player dies." << std::endl;
				m_ev->PublishEvent(new PlayerDies{});
				return;
			}
		}
	}

	for (auto itor = obj.begin(); itor != obj.end(); ++itor) 
	{
		moveComp = nullptr;
		scriptComp = nullptr;

		components = itor->second->GetPrefabActor()->GetComponentsPtr();
		// if object isnt meant to be control, skip
		// Get relevant components
		if ((*components).find(ComponentType::MovementComponent) != (*components).end())
			moveComp = dynamic_cast<MovementComponent*>((*components)[ComponentType::MovementComponent]);
		if ((*components).find(ComponentType::ScriptComponent) != (*components).end())
			scriptComp = dynamic_cast<ScriptComponent*>((*components)[ComponentType::ScriptComponent]);

		// Check if object contains relevant component
		if (!scriptComp)
			continue;
		if (!moveComp)
		{
			script = scriptComp->GetScript("Default");
			Updater = script->GetEnvironment()["Update"];
			Updater();
		}
		else
		{
			script = scriptComp->GetScript(moveComp->GetStateLogic());
			if (!script)
				continue;
			Updater = script->GetEnvironment()["Update"];
			Updater();
		}
	}
}

void LogicSystem::UpdateAllCooldown(std::map<unsigned, SceneActor*>& obj)
{
	StatsComponent * statsComp = nullptr;

	for (auto itor = obj.begin(); itor != obj.end(); ++itor)
	{
		statsComp = nullptr;

		components = itor->second->GetPrefabActor()->GetComponentsPtr();
		// if object isnt meant to be control, skip
		// Get relevant components
		if ((*components).find(ComponentType::StatsComponent) != (*components).end())
			statsComp = dynamic_cast<StatsComponent*>((*components)[ComponentType::StatsComponent]);

		if (statsComp)
			statsComp->UpdateCooldown(dt);
	}
}

void LogicSystem::UpdatePlatform(std::map<unsigned, SceneActor*>& obj)
{
	std::unordered_map<ComponentType, Component *>* map;
	RigidBodyComponent * onewayplatform_rbComp = nullptr;
	RigidBodyComponent * timedplatform_rbComp = nullptr;
	RigidBodyComponent * crumbling_rbComp = nullptr;
	RigidBodyComponent * tornado_rbComp = nullptr;
	RigidBodyComponent * pittrap_rbComp = nullptr;
	RigidBodyComponent * moving_rbComp = nullptr;
	RigidBodyComponent * c_gate_rbComp = nullptr;
	RigidBodyComponent * player_rbComp = nullptr;
	CollisionComponent * player_colComp = nullptr;
	CollisionComponent * enemy_colComp = nullptr;
	CollisionComponent * crumbling_colComp = nullptr;
	CollisionComponent * switch_colComp = nullptr;
	CollisionComponent * tornado_colComp = nullptr;
	CollisionComponent * moving_colComp = nullptr;
	CollisionComponent * c_hitbox_colComp = nullptr;
	TransformComponent * player_transComp = nullptr;
	TransformComponent * moving_transComp = nullptr;
	TransformComponent * respawn_transComp = nullptr;
	AABBComponent * c_gate_aabbComp = nullptr;
	AABBComponent * player_aabbComp = nullptr;
	AABBComponent * onewayplatform_aabbComp = nullptr;
	AABBComponent * moving_aabbComp = nullptr;
	StatsComponent * player_statComp = nullptr;
	StatsComponent * enemy_statComp = nullptr;

	std::vector<CollisionComponent*> c_hitbox_vec;
	std::vector<std::pair<RigidBodyComponent*, CollisionComponent*>> tornado_RBvec;
	std::vector<std::pair<RigidBodyComponent*, CollisionComponent*>> crumbling_vec;
	std::vector<std::pair<RigidBodyComponent*, AABBComponent*>> oneway_vec;
	std::vector<std::tuple<StatsComponent*, CollisionComponent*>> enemy_vec;
	std::vector<std::tuple<RigidBodyComponent*, TransformComponent*, CollisionComponent*, AABBComponent*, AABBComponent*>> moving_vec;
	std::vector<std::tuple<RigidBodyComponent*, AABBComponent*, CollisionComponent*>> switchgate_vec;
	std::vector<std::tuple<StatsComponent*, CollisionComponent*, TransformComponent*>> respawn_vec;
	for (auto itor = obj.begin(); itor != obj.end(); ++itor)
	{

		map = itor->second->GetPrefabActor()->GetComponentsPtr();
		//std::cout << itor->second->GetPrefabActor()->GetObjectType() << std::endl;
		if (itor->second->GetPrefabActor()->GetObjectType().compare("Player") == 0)
		{
			if ((*map).find(ComponentType::TransformComponent) != (*map).end())
				player_transComp = dynamic_cast<TransformComponent*>((*map)[ComponentType::TransformComponent]);
			if ((*map).find(ComponentType::AABBComponent) != (*map).end())
				player_aabbComp = dynamic_cast<AABBComponent*>((*map)[ComponentType::AABBComponent]);
			if ((*map).find(ComponentType::StatsComponent) != (*map).end())
				player_statComp = dynamic_cast<StatsComponent*>((*map)[ComponentType::StatsComponent]);
			if ((*map).find(ComponentType::CollisionComponent) != (*map).end())
				player_colComp = dynamic_cast<CollisionComponent*>((*map)[ComponentType::CollisionComponent]);
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				player_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Oneway_Platform") == 0)
		{
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				onewayplatform_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
			if ((*map).find(ComponentType::AABBComponent) != (*map).end())
				onewayplatform_aabbComp = dynamic_cast<AABBComponent*>((*map)[ComponentType::AABBComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Timed_Platform") == 0)
		{
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				timedplatform_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Crumbling_Platform") == 0)
		{
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				crumbling_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
			if ((*map).find(ComponentType::CollisionComponent) != (*map).end())
				crumbling_colComp = dynamic_cast<CollisionComponent*>((*map)[ComponentType::CollisionComponent]);

			crumbling_vec.push_back(std::pair<RigidBodyComponent*, CollisionComponent*>(crumbling_rbComp, crumbling_colComp));
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Tornado_Platform") == 0)
		{
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				tornado_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
			if ((*map).find(ComponentType::CollisionComponent) != (*map).end())
				tornado_colComp = dynamic_cast<CollisionComponent*>((*map)[ComponentType::CollisionComponent]);

			tornado_RBvec.push_back(std::pair<RigidBodyComponent*, CollisionComponent*>(tornado_rbComp, tornado_colComp));
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Pit_Traps") == 0)
		{
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				pittrap_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Moving_Platform") == 0)
		{
			if ((*map).find(ComponentType::RigidBodyComponent) != (*map).end())
				moving_rbComp = dynamic_cast<RigidBodyComponent*>((*map)[ComponentType::RigidBodyComponent]);
			if ((*map).find(ComponentType::TransformComponent) != (*map).end())
				moving_transComp = dynamic_cast<TransformComponent*>((*map)[ComponentType::TransformComponent]);
			if ((*map).find(ComponentType::CollisionComponent) != (*map).end())
				moving_colComp = dynamic_cast<CollisionComponent*>((*map)[ComponentType::CollisionComponent]);
			if ((*map).find(ComponentType::AABBComponent) != (*map).end())
				moving_aabbComp = dynamic_cast<AABBComponent*>((*map)[ComponentType::AABBComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Switch") == 0)
		{
			if ((*map).find(ComponentType::CollisionComponent) != (*map).end())
				switch_colComp = dynamic_cast<CollisionComponent*>((*map)[ComponentType::CollisionComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("Enemy") == 0)
		{
			if ((*map).find(ComponentType::StatsComponent) != (*map).end())
				enemy_statComp = dynamic_cast<StatsComponent*>((*map)[ComponentType::StatsComponent]);
			if ((*map).find(ComponentType::CollisionComponent) != (*map).end())
				enemy_colComp = dynamic_cast<CollisionComponent*>((*map)[ComponentType::CollisionComponent]);
		}
		else if (itor->second->GetPrefabActor()->GetObjectType().compare("RespawnBox") == 0)
		{
			if ((*map).find(ComponentType::TransformComponent) != (*map).end())
				respawn_transComp = dynamic_cast<TransformComponent*>((*map)[ComponentType::TransformComponent]);
		}

		if (itor->second->GetChildSceneActorsPtr()->size() > 0) // has child
		{

			auto obj1 = itor->second->GetChildSceneActorsPtr();
			for (auto itor2 = obj1->begin(); itor2 != obj1->end(); ++itor2)
			{
				if (itor2->second->GetPrefabActor()->GetObjectType().compare("Gate") == 0)
				{
					c_gate_rbComp = dynamic_cast<RigidBodyComponent*>(itor2->second->GetComponent(ComponentType::RigidBodyComponent));
					c_gate_aabbComp = dynamic_cast<AABBComponent*>(itor2->second->GetComponent(ComponentType::AABBComponent));
				}
				if (itor2->second->GetPrefabActor()->GetObjectType().compare("PlayerHitbox") == 0)
				{
					c_hitbox_colComp = dynamic_cast<CollisionComponent*>(itor2->second->GetComponent(ComponentType::CollisionComponent));
				}
				if (c_hitbox_colComp && itor2->second->GetActive())
				{
					c_hitbox_vec.push_back(c_hitbox_colComp);
					c_hitbox_colComp = nullptr;
				}
			}
		}
		if (c_gate_rbComp && c_gate_aabbComp && switch_colComp)
			switchgate_vec.push_back(std::make_tuple(c_gate_rbComp, c_gate_aabbComp, switch_colComp));
		if (onewayplatform_rbComp && onewayplatform_aabbComp)
			oneway_vec.push_back(std::pair<RigidBodyComponent*, AABBComponent*>(onewayplatform_rbComp, onewayplatform_aabbComp));
		if (moving_rbComp && moving_transComp && moving_colComp && player_aabbComp && moving_aabbComp)
		{
			moving_vec.push_back(std::make_tuple(moving_rbComp, moving_transComp, moving_colComp, player_aabbComp, moving_aabbComp));
			moving_rbComp = nullptr;
			moving_transComp = nullptr;
			moving_colComp = nullptr;
			moving_aabbComp = nullptr;
		}
		if (enemy_statComp && enemy_colComp && itor->second->GetActive())
		{
			enemy_vec.push_back(std::tuple<StatsComponent*, CollisionComponent*>(enemy_statComp, enemy_colComp));
			enemy_statComp = nullptr;
			enemy_colComp = nullptr;
		}
		if (respawn_transComp && player_statComp && player_colComp)
		{
			respawn_vec.push_back(std::tuple<StatsComponent*, CollisionComponent*, TransformComponent*>(player_statComp, player_colComp, respawn_transComp));
			respawn_transComp = nullptr;
		}
	}

	for(auto& e : oneway_vec)
		if (player_transComp && player_aabbComp && e.first && e.second)
			UpdateOnewayPlatform(player_transComp, player_aabbComp, e.first, e.second);

	if (timedplatform_rbComp)
		UpdateTimedPlatform(timedplatform_rbComp);

	for(auto& e : crumbling_vec)
		if (e.first && e.second)
			UpdateCrumblingPlatform(e.first, e.second);

	for(auto& e : tornado_RBvec)
		if (e.first && player_rbComp && e.second)
			UpdateTornadoPlatform(e.first, player_rbComp, e.second);

	if (pittrap_rbComp && player_statComp && player_colComp)
		UpdatePitTraps(pittrap_rbComp, player_statComp, player_colComp);

	for(auto& e : moving_vec)
		if (std::get<0>(e) && std::get<1>(e) && player_transComp && std::get<2>(e) && std::get<3>(e) && std::get<4>(e))
			UpdateMovingPlatform(std::get<0>(e), std::get<1>(e), player_transComp, std::get<2>(e), std::get<3>(e), std::get<4>(e));

	for (auto& e : switchgate_vec)
		if (std::get<0>(e) && std::get<1>(e) && std::get<2>(e))
			UpdateSwitchGate(std::get<0>(e), std::get<1>(e), std::get<2>(e));

	for(auto& e : c_hitbox_vec)
		if (player_statComp && player_colComp)
			UpdateStatsPlayer(player_statComp, player_colComp, e);

	for (auto& e : enemy_vec)
		if (std::get<0>(e) && std::get<1>(e))
			UpdateStatsEnemy(std::get<0>(e), std::get<1>(e));

	for(auto& e : respawn_vec)
		if (std::get<0>(e) && std::get<1>(e) && std::get<2>(e))
			UpdateRespawnPlatform(std::get<0>(e), std::get<1>(e), std::get<2>(e));

	tornado_RBvec.clear();
	crumbling_vec.clear();
	switchgate_vec.clear();
	oneway_vec.clear();
	moving_vec.clear();
	c_hitbox_vec.clear();
	enemy_vec.clear();
	respawn_vec.clear();
}

void LogicSystem::UpdateOnewayPlatform(TransformComponent * playerTrans, AABBComponent* playerAABB, RigidBodyComponent * onewayRB, AABBComponent* onewayAABB)
{
	Vector2D aabbScale = playerAABB->GetAABBScale();
	Vector2D pos = playerTrans->GetPosition();
	Vector3D objectScale = playerTrans->GetScale();
	if (pos.y - (aabbScale.y * objectScale.y / 2)  < onewayAABB->GetMax().y)
		onewayRB->SetObjType("NONEXIST");
	else
		onewayRB->SetObjType("ONEWAYPLATFORM");
}

void LogicSystem::UpdateTimedPlatform(RigidBodyComponent * timedRB)
{
	timedRB->SetCounter(timedRB->GetCounter() + 1.0f / 60.0f);
	if (timedRB->GetCounter() >= timedRB->GetStartTime())
	{
		if (timedRB->GetObjType() == "NONEXIST")
			timedRB->SetObjType("TIMEDPLATFORM");
		else
			timedRB->SetObjType("NONEXIST");

		if (timedRB->GetCounter() >= timedRB->GetStartTime() + timedRB->GetDuration()) //2sec
		{
			timedRB->SetCounter(timedRB->GetStartTime());
		}
	}
}

void LogicSystem::UpdateCrumblingPlatform(RigidBodyComponent * crumblingRB, CollisionComponent* crumblingCol)
{
	if (crumblingCol->GetCollidingvsCrumbling())
		crumblingRB->SetCounter(crumblingRB->GetCounter() + 1.0f / 60.0f);

	if (crumblingRB->GetCounter() >= crumblingRB->GetStartTime() + crumblingRB->GetDuration() && crumblingRB->GetObjType() != "NONEXIST")
	{
		crumblingRB->SetObjType("NONEXIST");
		crumblingRB->SetCounter(crumblingRB->GetStartTime());
	}
	if (crumblingRB->GetObjType() == "NONEXIST")
	{
		crumblingRB->SetCounter(crumblingRB->GetCounter() + 1.0f / 60.0f);
		if (crumblingRB->GetCounter() >= crumblingRB->GetDuration())
		{
			crumblingRB->SetObjType("CRUMBLINGPLATFORM");
			crumblingRB->SetCounter(crumblingRB->GetStartTime());
		}
	}
}

void LogicSystem::UpdateTornadoPlatform(RigidBodyComponent * tornadoRB, RigidBodyComponent* playerRB, CollisionComponent* tornadoCol)
{
	tornadoRB->SetCounter(tornadoRB->GetCounter() + 1.0f / 60.0f);
	if (tornadoRB->GetCounter() >= tornadoRB->GetDuration())
	{
		tornadoRB->SetCounter(0.0f);
		//apply impulse
		if (tornadoCol->GetCollidingvsTornado())
		{
			playerRB->SetAcceleration(Vector2D(0.0f, 0.0f));
			playerRB->SetVelocity(Vector2D(0.0f, 0.0f));
			playerRB->SetForce(Force(tornadoRB->GetDirection(), 0.000001f, 0.0f, true));
		}
	}
}

void LogicSystem::UpdateMovingPlatform(RigidBodyComponent * movingRB, TransformComponent* movingTrans, TransformComponent * playerTrans, CollisionComponent * movingCol, AABBComponent* playerAABB,
										AABBComponent* movingAABB)
{
	Vector2D aabbScale = playerAABB->GetAABBScale();
	Vector2D pos = playerTrans->GetPosition();
	Vector3D objectScale = playerTrans->GetScale();

	Vector2D m_aabbScale = movingAABB->GetAABBScale();
	Vector2D m_pos = movingTrans->GetPosition();
	Vector3D m_objectScale = movingTrans->GetScale();

	if (pos.y - (aabbScale.y * objectScale.y / 2) < m_pos.y + (m_aabbScale.y * m_objectScale.y / 2))
		movingRB->SetObjType("NONEXIST");
	else
		movingRB->SetObjType("MOVINGPLATFORM");



	movingRB->SetCounter(movingRB->GetCounter() + 1.0f / 60.0f);
	if (movingRB->GetCounter() >= movingRB->GetDuration())
	{
		movingRB->SetDirection(-movingRB->GetDirection());
		movingRB->SetCounter(0.0f);
	}
	movingTrans->SetPosition(movingTrans->GetPosition() + movingRB->GetDirection() * 1.0f / 60.0f);

	if (movingCol->GetCollidingvsMoving())
		playerTrans->SetPosition(playerTrans->GetPosition() + movingRB->GetDirection() * 1.0f / 60.0f);

}

void LogicSystem::UpdatePitTraps(RigidBodyComponent * pitRB, StatsComponent * playerStat, CollisionComponent* playerCol)
{
	if (playerCol->GetCollidingvsPit())
	{
		if (pitRB->GetCounter() == 0.0f)
		{
			playerStat->SetHP(playerStat->GetHP() - 50);
		}
		pitRB->SetCounter(pitRB->GetCounter() + 1.0f / 60.0f);
		if (pitRB->GetCounter() >= pitRB->GetDuration())
		{
			pitRB->SetCounter(0.0f);
		}
	}
}

void LogicSystem::UpdateSwitchGate(RigidBodyComponent * gateRB, AABBComponent* gateAABB, CollisionComponent * switchCol)
{
	if (switchCol->GetCollidingvsSwitch())
	{
		if (gateRB->GetCounter() >= gateRB->GetDuration()) return;
		gateRB->SetCounter(gateRB->GetCounter() + 1.0f / 60.0f);
		gateAABB->SetOffset(gateAABB->GetOffset() + gateRB->GetDirection());
	}
}

void LogicSystem::UpdateRespawnPlatform(StatsComponent * playerStats, CollisionComponent * playerCol, TransformComponent* respawnTrans)
{
	if (playerCol->GetCollidingvsRespawn())
	{
		playerStats->SetRespawnPos(respawnTrans->GetPosition());
		std::cout << playerStats->GetRespawnPos() << std::endl;
	}
}

void LogicSystem::UpdateStatsPlayer(StatsComponent * playerStat, CollisionComponent * playerCol, CollisionComponent* hitboxCol)
{
	// player gets hit
	if (!playerStat->GetInvul())
	{
		if (playerCol->GetCollideEnemyHitbox())
		{
			playerStat->SetHP(playerStat->GetHP() - 1);
			std::cout << "player -1 from hit" << std::endl;
		}
		if (playerCol->GetCollideEnemyDashHitbox())
		{
			playerStat->SetHP(playerStat->GetHP() - 2);
			std::cout << "player -2 from dash" << std::endl;
		}
		if (playerCol->GetCollideEnemyCounterHitbox())
		{
			playerStat->SetHP(playerStat->GetHP() - 2);
			std::cout << "player -2 from counter" << std::endl;
		}
	}
	// player hits enemies
	if (hitboxCol->GetCollidePlayerHitbox())
	{
		playerStat->SetMP(playerStat->GetMP() + 0.2f);
		std::cout << "player +0.2 from hitting" << std::endl;
	}
}

void LogicSystem::UpdateStatsEnemy(StatsComponent * enemyStat, CollisionComponent * enemyCol)
{
	// enemy gets hit
		if (enemyCol->GetCollidePlayerHitbox())
		{
			enemyStat->SetHP(enemyStat->GetHP() - 3.0f);
			std::cout << "enemy -3 from hit" << std::endl;
		}
		if (enemyCol->GetCollideBigPunchHitbox())
		{
			enemyStat->SetHP(enemyStat->GetHP() - 10.0f);
			std::cout << "enemy -10 from bigpunch" << std::endl;
		}
		if (enemyCol->GetCollidePlayerDashHitbox())
		{
			enemyStat->SetHP(enemyStat->GetHP() - 1.0f);
			std::cout << "enemy -1 from dash" << std::endl;
		}
	
}

SceneActor * LogicSystem::FindPlayer(std::string name, std::map<unsigned, SceneActor*>& objs)
{
	for (auto & elem : objs)
	{
		if (elem.second->GetSceneActorName() == name)
		{
			return elem.second;
		}
	}

	return nullptr;
}

void LogicSystem::ApplyDamage(DamageEvent* ev)
{
	StatsComponent* statComp;
	statComp = ev->m_statComp;
	statComp->SetHP(statComp->GetHP() - ev->m_damage);

	//std::cout << "INFLICT" << std::endl;
}

void LogicSystem::WinLose(WinLoseEvent * ev)
{
	if (ev->m_winlose)
	{
		// do win stuff
		std::cout << "WIN";
	}
	else
	{
		// lose stuff
		std::cout << "LOSE";
	}
}
