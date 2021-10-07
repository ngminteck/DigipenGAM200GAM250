/******************************************************************************/
/*!
\file   SpatialPartition.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of LogicSystemHotspotComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <list>
#include <array>
#include "Vector2D.h"
#include "Macros.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "AABBComponent.h"
#include "CollisionComponent.h"
#include <tuple>

namespace SpatialPartition
{
	void SweepAndPrune(const std::vector<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>>& axisVec, std::vector<std::pair<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>, std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>>>& result);
}