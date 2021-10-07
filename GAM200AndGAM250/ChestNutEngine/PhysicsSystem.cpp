/******************************************************************************/
/*!
\file   PhysicsSystem.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of the physics system

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "PhysicsSystem.h"
#define PI 3.14159265358979323846f
PhysicsSystem::PhysicsSystem(EventHandler * ev, bool & p)
	:eventHandler{ ev }, profiler{"Physics System"}, init(true), gravity{true}, pause{p}
{
	xAxisVec.reserve(m_max_objects);
	cmp.reserve(m_max_objects);
	eventHandler->SubscribeEvent(this, &PhysicsSystem::SetInit);
	eventHandler->SubscribeEvent(this, &PhysicsSystem::SwitchGravity);	
	//eventHandler->SubscribeEvent(this,)
}

PhysicsSystem::~PhysicsSystem()
{

}

void PhysicsSystem::Update(std::map<unsigned, SceneActor*>& obj, float dt)
{
	profiler.UpdateStart();
	// Obj component map
	std::unordered_map<ComponentType, Component *>* map1;
	//std::unordered_map<ComponentType, Component *>* map2;

	TransformComponent	* transComp1 = nullptr;
	TransformComponent	* c_transComp1 = nullptr;
	AABBComponent		* aabbComp1 = nullptr;
	AABBComponent       * c_aabbComp1 = nullptr;
	
	RigidBodyComponent	* rbComp1 = nullptr;
	RigidBodyComponent  * c_rbComp1 = nullptr;
	CollisionComponent	* colComp1 = nullptr;
	CollisionComponent	* c_colComp1 = nullptr;
	StatsComponent      * statComp1 = nullptr;
	MovementComponent   * moveComp1 = nullptr;

	auto lambda = [](std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*> x1, 
					 std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*> x2) 
				{ return std::get<3>(x1)->GetMin().x < std::get<3>(x2)->GetMin().x; };


	// Getting first obj which matches the required components.
	// Then applying the necessary physics in sequence:
	// Apply forces -> Update pos/vel etc -> Detect collision -> Resolve -> Display
	for (auto itor1 = obj.begin(); itor1 != obj.end(); ++itor1)
	{
		transComp1 = nullptr;
		aabbComp1 = nullptr;
		rbComp1 = nullptr;
		colComp1 = nullptr;
		statComp1 = nullptr;
		moveComp1 = nullptr;

		map1 = itor1->second->GetPrefabActor()->GetComponentsPtr();
		// Get relevant components
		if ((*map1).find(ComponentType::AABBComponent) != (*map1).end())
			aabbComp1 = dynamic_cast<AABBComponent*>((*map1)[ComponentType::AABBComponent]);
		if ((*map1).find(ComponentType::CollisionComponent) != (*map1).end())
			colComp1 = dynamic_cast<CollisionComponent*>((*map1)[ComponentType::CollisionComponent]);
		if ((*map1).find(ComponentType::TransformComponent) != (*map1).end())
			transComp1 = dynamic_cast<TransformComponent*>((*map1)[ComponentType::TransformComponent]);
		if ((*map1).find(ComponentType::RigidBodyComponent) != (*map1).end())
			rbComp1 = dynamic_cast<RigidBodyComponent*>((*map1)[ComponentType::RigidBodyComponent]);
		if ((*map1).find(ComponentType::StatsComponent) != (*map1).end())
			statComp1 = dynamic_cast<StatsComponent*>((*map1)[ComponentType::StatsComponent]);
		if ((*map1).find(ComponentType::MovementComponent) != (*map1).end())
			moveComp1 = dynamic_cast<MovementComponent*>((*map1)[ComponentType::MovementComponent]);

		if (colComp1 && itor1->second->GetActive())
			colComp1->SetAll(false);
		

		if (rbComp1 && !rbComp1->GetIsStaticObj())
		{
			if (transComp1 && itor1->second->GetActive())
				UpdateRigidBody(rbComp1, transComp1, dt);
		}

		if (aabbComp1 && rbComp1 && transComp1 && itor1->second->GetActive() && (aabbComp1->GetInit() || !rbComp1->GetIsStaticObj()))
			UpdateAABB(aabbComp1, transComp1);

		if (init && aabbComp1 && colComp1)
			xAxisVec.push_back(std::tie(colComp1, rbComp1, transComp1, aabbComp1, itor1->second));

		if (itor1->second->GetChildSceneActorsPtr()->size() > 0) // has child
		{

			auto obj1 = itor1->second->GetChildSceneActorsPtr();
			for (auto itor2 = obj1->begin(); itor2 != obj1->end(); ++itor2)
			{

				c_transComp1 = nullptr;
				c_aabbComp1 = nullptr;
				c_rbComp1 = nullptr;
				c_colComp1 = nullptr;

				c_transComp1 = dynamic_cast<TransformComponent*>(itor2->second->GetComponent(ComponentType::TransformComponent));
				c_aabbComp1 = dynamic_cast<AABBComponent*>(itor2->second->GetComponent(ComponentType::AABBComponent));
				c_rbComp1 = dynamic_cast<RigidBodyComponent*>(itor2->second->GetComponent(ComponentType::RigidBodyComponent));
				c_colComp1 = dynamic_cast<CollisionComponent*>(itor2->second->GetComponent(ComponentType::CollisionComponent));

				if (c_colComp1)
					c_colComp1->SetAll(false);

				//if (!itor2->second->GetActive()) continue


				if (c_rbComp1 && c_transComp1 && (c_rbComp1->GetObjType() == "BOSSSPIKES" || c_rbComp1->GetObjType() == "NONEXISTBOSSSPIKES"))
				{
					UpdateRigidBody(c_rbComp1, c_transComp1, dt);
					if (c_aabbComp1 && c_transComp1)
						UpdateAABB(c_aabbComp1, c_transComp1);
					if (init && c_aabbComp1 && c_colComp1)
						xAxisVec.push_back(std::tie(c_colComp1, c_rbComp1, c_transComp1, c_aabbComp1, itor2->second));
					continue;
				}

				if (c_transComp1 && c_aabbComp1 && transComp1)
					UpdateChildRigidBody(c_transComp1, c_aabbComp1, transComp1, moveComp1);
				
				if (c_aabbComp1 && c_transComp1)
					UpdateAABB(c_aabbComp1, c_transComp1);
				
				if (init && c_aabbComp1 && c_colComp1)
					xAxisVec.push_back(std::tie(c_colComp1, c_rbComp1, c_transComp1, c_aabbComp1, itor2->second));
			}
		}
	}
	std::sort(xAxisVec.begin(), xAxisVec.end(), lambda);
	// above this line is 1.3ms
	SpatialPartition::SweepAndPrune(xAxisVec, cmp); // 0.4ms
	Collision::MTV mtv(Vector2D(0.0f, 0.0f), 0.0f);
	for (auto& e : cmp)
	{
		if (std::get<1>(e.first) && std::get<1>(e.second) && 
			std::get<1>(e.first)->GetIsStaticObj() && 
			std::get<1>(e.second)->GetIsStaticObj()) 
			continue;

		if (!std::get<4>(e.first)->GetActive() || !std::get<4>(e.second)->GetActive())
			continue;
		if (Collision::SeperatingAxis(std::get<3>(e.first), std::get<3>(e.second), mtv))
		{
			if (std::get<1>(e.first) && std::get<1>(e.second))
			{
				if (std::get<1>(e.first)->GetObjType() != "NONEXIST" && std::get<1>(e.second)->GetObjType() != "NONEXIST")
				{
					if (std::get<1>(e.first)->GetObjType() == "NONEXISTBOSSSPIKES" || std::get<1>(e.second)->GetObjType() == "NONEXISTBOSSSPIKES")
						continue;

					if (std::get<0>(e.first))
					{
						std::get<0>(e.first)->SetFlag(true);
					}
					if (std::get<0>(e.second))
					{
						std::get<0>(e.second)->SetFlag(true);
					}
					
					if (std::get<0>(e.first) && std::get<0>(e.second))
					{
						if (std::get<1>(e.first)->GetObjType() == "PLAYER_HIT" && (std::get<1>(e.second)->GetObjType() == "ENEMY" || std::get<1>(e.second)->GetObjType() == "TRIGGERBOX" || std::get<1>(e.second)->GetObjType() == "PITTRAPS"))
						{
							std::get<0>(e.first)->SetPlayerHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetPlayerHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PLAYER_HIT" && (std::get<1>(e.first)->GetObjType() == "ENEMY" || std::get<1>(e.first)->GetObjType() == "TRIGGERBOX" || std::get<1>(e.first)->GetObjType() == "PITTRAPS"))
						{
							std::get<0>(e.first)->SetPlayerHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetPlayerHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "PLAYER_DASH" && std::get<1>(e.second)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetPlayerDashHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetPlayerDashHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PLAYER_DASH" && std::get<1>(e.first)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetPlayerDashHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetPlayerDashHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "PLAYER_BIGPUNCH" && std::get<1>(e.second)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetBigPunchHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetBigPunchHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PLAYER_BIGPUNCH" && std::get<1>(e.first)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetBigPunchHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetBigPunchHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "ENEMY_DASH" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetEnemyDashHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetEnemyDashHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "ENEMY_DASH" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetEnemyDashHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetEnemyDashHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "ENEMY_COUNTER" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetEnemyCounterHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetEnemyCounterHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "ENEMY_COUNTER" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetEnemyCounterHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetEnemyCounterHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "ENEMY_HIT" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetEnemyHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetEnemyHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "ENEMY_HIT" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetEnemyHitboxFlag(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetEnemyHitboxFlag(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
					}
					if (std::get<1>(e.first)->GetObjType().find("ENEMY_") != std::string::npos || std::get<1>(e.second)->GetObjType().find("ENEMY_") != std::string::npos
						|| std::get<1>(e.first)->GetObjType().find("PLAYER_") != std::string::npos || std::get<1>(e.second)->GetObjType().find("PLAYER_") != std::string::npos)
						continue;

					if (std::get<1>(e.first)->GetObjType().find("ENEMY_") == std::string::npos && std::get<1>(e.second)->GetObjType().find("ENEMY_") == std::string::npos
						&& std::get<1>(e.first)->GetObjType().find("PLAYER_") == std::string::npos && std::get<1>(e.second)->GetObjType().find("PLAYER_") == std::string::npos)
					{
						if (std::get<1>(e.first)->GetObjType() == "TORNADOPLATFORM" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagTornado(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagTornado(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "TORNADOPLATFORM" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagTornado(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagTornado(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "BOSSSPIKES" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "BOSSSPIKES" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "PITTRAPS" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PITTRAPS" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "PITTRAPS" && std::get<1>(e.second)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PITTRAPS" && std::get<1>(e.first)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PITTRAPS" && std::get<1>(e.first)->GetObjType() == "PITTRAPS")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "PITTRAPS" && std::get<1>(e.second)->GetObjType() == "ENDGAMEBOX")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "PITTRAPS" && std::get<1>(e.first)->GetObjType() == "ENDGAMEBOX")
						{
							std::get<0>(e.first)->SetFlagPit(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPit(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "SWITCH")
						{
							std::get<0>(e.first)->SetFlagSwitch(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagSwitch(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "SWITCH")
						{
							std::get<0>(e.first)->SetFlagSwitch(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagSwitch(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "CHECKPOINT" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagCheckpoint(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagCheckpoint(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "CHECKPOINT" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagCheckpoint(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagCheckpoint(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "MOVINGPLATFORM" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagMoving(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagMoving(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
						}
						if (std::get<1>(e.second)->GetObjType() == "MOVINGPLATFORM" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagMoving(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagMoving(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
						}
						if (std::get<1>(e.first)->GetObjType() == "CRUMBLINGPLATFORM")
						{
							std::get<0>(e.first)->SetFlagCrumbling(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagCrumbling(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
						}
						if (std::get<1>(e.second)->GetObjType() == "CRUMBLINGPLATFORM")
						{
							std::get<0>(e.first)->SetFlagCrumbling(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagCrumbling(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
						}
						if (std::get<1>(e.first)->GetObjType() == "RESPAWNBOX")
						{
							std::get<0>(e.first)->SetFlagRespawn(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagRespawn(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "RESPAWNBOX")
						{
							std::get<0>(e.first)->SetFlagRespawn(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagRespawn(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "TRIGGERBOX")
						{
							std::get<0>(e.first)->SetFlagTrigger(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagTrigger(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "TRIGGERBOX")
						{
							std::get<0>(e.first)->SetFlagTrigger(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagTrigger(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "DYNAMICCAMERAIN")
						{
							std::get<0>(e.first)->SetFlagDIn(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));
							
							std::get<0>(e.second)->SetFlagDIn(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "DYNAMICCAMERAIN")
						{
							std::get<0>(e.first)->SetFlagDIn(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));
							
							std::get<0>(e.second)->SetFlagDIn(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "DYNAMICCAMERAOUT")
						{
							std::get<0>(e.first)->SetFlagDOut(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));
							
							std::get<0>(e.second)->SetFlagDOut(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "DYNAMICCAMERAOUT")
						{
							std::get<0>(e.first)->SetFlagDOut(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));
							
							std::get<0>(e.second)->SetFlagDOut(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "COLLIDERBOX")
						{
							std::get<0>(e.first)->SetFlagCollider(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagCollider(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
						}
						if (std::get<1>(e.second)->GetObjType() == "COLLIDERBOX")
						{
							std::get<0>(e.first)->SetFlagCollider(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagCollider(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
						}
						if (std::get<1>(e.first)->GetObjType() == "ENDGAMEBOX" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagEndGameBox(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagEndGameBox(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "ENDGAMEBOX" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagEndGameBox(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagEndGameBox(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.first)->GetObjType() == "POPUPBOX" && std::get<1>(e.second)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagPopup(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPopup(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<1>(e.second)->GetObjType() == "POPUPBOX" && std::get<1>(e.first)->GetObjType() == "PLAYER")
						{
							std::get<0>(e.first)->SetFlagPopup(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagPopup(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
						if (std::get<0>(e.first) && std::get<0>(e.second))
						{
							if (std::get<1>(e.first)->GetObjType() == "ENEMY" && std::get<1>(e.second)->GetObjType() != "COLLIDERBOX")
							{
								std::get<0>(e.first)->SetFlagEnemy(true);
								std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

								std::get<0>(e.second)->SetFlagEnemy(true);
								std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
								continue;
							}
							if (std::get<1>(e.second)->GetObjType() == "ENEMY" && std::get<1>(e.first)->GetObjType() != "COLLIDERBOX")
							{
								std::get<0>(e.first)->SetFlagEnemy(true);
								std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

								std::get<0>(e.second)->SetFlagEnemy(true);
								std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
								continue;
							}
						}
						if (std::get<1>(e.first)->GetObjType() == "ENEMY" && std::get<1>(e.second)->GetObjType() == "ENEMY")
						{
							std::get<0>(e.first)->SetFlagEnemy(true);
							std::get<0>(e.first)->PushBackCollidedObject(std::get<4>(e.second));

							std::get<0>(e.second)->SetFlagEnemy(true);
							std::get<0>(e.second)->PushBackCollidedObject(std::get<4>(e.first));
							continue;
						}
					}

					Collision::SATCollisionResolution(std::get<1>(e.first), std::get<2>(e.first), std::get<0>(e.first),
						std::get<1>(e.second), std::get<2>(e.second), std::get<0>(e.second), mtv);
				}
			}
		}
	}
	for (auto& e : cmp) //Setting IsColliding
	{
		if (std::get<0>(e.first))
		{
			std::get<0>(e.first)->CheckFlagAndSet(true);
		}
		if (std::get<0>(e.second))
		{
			std::get<0>(e.second)->CheckFlagAndSet(true);
		}
	}
	cmp.clear();
	for (auto& e : obj)
	{
		map1 = e.second->GetPrefabActor()->GetComponentsPtr();
		if ((*map1).find(ComponentType::TransformComponent) != (*map1).end())
			transComp1 = dynamic_cast<TransformComponent*>((*map1)[ComponentType::TransformComponent]);
		if (transComp1)
			transComp1->SetTranslate(Vector3D(transComp1->GetPosition().x, transComp1->GetPosition().y, 0.f));

		if (e.second->GetChildSceneActorsPtr()->size() > 0) // has child
		{
			c_transComp1 = nullptr;
			c_rbComp1 = nullptr;

			auto obj1 = e.second->GetChildSceneActorsPtr();
			for (auto itor2 = obj1->begin(); itor2 != obj1->end(); ++itor2)
			{
				c_transComp1 = dynamic_cast<TransformComponent*>(itor2->second->GetComponent(ComponentType::TransformComponent));

				if (c_transComp1)
					c_transComp1->SetTranslate(Vector3D(c_transComp1->GetPosition().x, c_transComp1->GetPosition().y, 0.f));
			}
		}
	}
	init = false;
	profiler.UpdateEnd();
	// time above here is 2ms
}

void PhysicsSystem::UpdateRigidBody(RigidBodyComponent * rbComp, TransformComponent * transComp, float dt)
{

	if (!pause)
	{
		rbComp->SetAcceleration(rbComp->GetForce().GetResultForce() / rbComp->GetMass());

		if (gravity)
			UpdateGravity(rbComp);
		else
		{
			rbComp->SetAcceleration(Vector2D(rbComp->GetAcceleration().x, 0.0f));
			rbComp->SetVelocity(Vector2D(rbComp->GetVelocity().x, 0.0f));
		}
		
		rbComp->GetForce().ValidateAge(dt);
		rbComp->SetVelocity(rbComp->GetVelocity() + rbComp->GetAcceleration() * dt);
	}

	else //If pause is true
	{
		rbComp->SetAcceleration({ 0,0 });
		rbComp->SetVelocity({ 0,0 });
	}

	transComp->SetPosition(transComp->GetPosition() + rbComp->GetVelocity() * dt);
	
}

void PhysicsSystem::UpdateChildRigidBody(TransformComponent * childComp, AABBComponent* childAABB, TransformComponent* parentTrans, MovementComponent* parentMove)
{
	if (childComp && childAABB && parentTrans && parentMove)
	{
		if (parentMove->GetDirection() == RIGHT)
			childComp->SetPosition(parentTrans->GetPosition() + Vector2D(-childAABB->GetOffset().x, childAABB->GetOffset().y));
		else if (parentMove->GetDirection() == LEFT)
			childComp->SetPosition(parentTrans->GetPosition() + childAABB->GetOffset());
	}
	else
		childComp->SetPosition(parentTrans->GetPosition() + childAABB->GetOffset());

}

void PhysicsSystem::UpdateGravity(RigidBodyComponent * rbComp)
{
	if(rbComp->GetHasGravity())
		rbComp->SetAcceleration(Vector2D(rbComp->GetAcceleration().GetX(), 
								rbComp->GetAcceleration().GetY() + -rbComp->GetGravity()));
}

void PhysicsSystem::UpdateAABB(AABBComponent * aabbComp, TransformComponent * transComp)
{
	aabbComp->SetInit(false);
	Vector2D aabbScale = aabbComp->GetAABBScale();
	Vector2D pos = transComp->GetPosition();
	Vector3D objectScale = transComp->GetScale();

	aabbComp->SetMin(Vector2D(pos.x - aabbScale.x*objectScale.x / 2,
							  pos.y - aabbScale.y*objectScale.y / 2));
	aabbComp->SetMin(Vector2D(aabbComp->GetMin().x - pos.x, aabbComp->GetMin().y - pos.y));
	aabbComp->SetMin(Vector2D(aabbComp->GetMin().x * cosf(transComp->GetRotate() * PI/180.0f) + aabbComp->GetMin().y * sinf(transComp->GetRotate() * PI/180.0f),
							  -(aabbComp->GetMin().x * sinf(transComp->GetRotate() * PI/180.0f) - aabbComp->GetMin().y * cosf(transComp->GetRotate() * PI/180.0f))));
	aabbComp->SetMin(Vector2D(aabbComp->GetMin().x + pos.x, aabbComp->GetMin().y + pos.y));

	aabbComp->SetMax(Vector2D(pos.x + aabbScale.x*objectScale.x / 2,
							  pos.y + aabbScale.y*objectScale.y / 2));
	aabbComp->SetMax(Vector2D(aabbComp->GetMax().x - pos.x, aabbComp->GetMax().y - pos.y));
	aabbComp->SetMax(Vector2D(aabbComp->GetMax().x * cosf(transComp->GetRotate() * PI/180.0f) + aabbComp->GetMax().y * sinf(transComp->GetRotate() * PI /180.0f),
							  -(aabbComp->GetMax().x * sinf(transComp->GetRotate() * PI /180.0f) - aabbComp->GetMax().y * cosf(transComp->GetRotate() * PI /180.0f))));
	aabbComp->SetMax(Vector2D(aabbComp->GetMax().x + pos.x, aabbComp->GetMax().y + pos.y));

	aabbComp->SetRect(aabbComp->GetMin(), aabbComp->GetMax());
	transComp->SetAABBScale(aabbComp->GetAABBScale());
}
