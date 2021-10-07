/******************************************************************************/
/*!
\file   PhysicsSystem.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of the physics system.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "System.h"
#include "HealthEvent.h" //Debug
#include "SceneActor.h"
#include "Shapes.h"
#include "SpatialPartition.h"
#include "Collision.h"

#include "ToggleGravity.h"
#include "SwitchLevelEvent.h"
#include "DamageEvent.h"
#include "PauseGame.h"

class PhysicsSystem : System
{
public:
	PhysicsSystem(EventHandler *, bool &);
	~PhysicsSystem();

	void Update(std::map<unsigned, SceneActor*>&, float);
	void UpdateRigidBody(RigidBodyComponent *, TransformComponent *, float dt);
	void UpdateChildRigidBody(TransformComponent * childComp, AABBComponent* childAABB, TransformComponent* parentTrans, MovementComponent* parentMove);
	void UpdateGravity(RigidBodyComponent *);
	void UpdateAABB(AABBComponent *, TransformComponent *);
	void PauseSystem(PauseGame *);

	void SwitchGravity(ToggleGravity * ) { gravity = !gravity; }
	void SetInit(SwitchLevelEvent * ev) { UNREFERENCED_PARAMETER(ev); std::cout << "INIT"; init = true; xAxisVec.clear(); }

	Profiler profiler;
	bool gravity;
	bool & pause;

private:
	//bool CheckAllowCollision(CollisionComponent* colComp1, RigidBodyComponent* rbComp1, CollisionComponent* colComp2, RigidBodyComponent* rbComp2);
	EventHandler * eventHandler; //Pointer to the game engine's event handler.
	std::vector<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>> xAxisVec;
	std::vector<std::pair<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>, 
						  std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>>> cmp;
	bool init;
	const int m_max_objects = 1000;
};
