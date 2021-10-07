/******************************************************************************/
/*!
\file   Collision.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface file for collision functions

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "AABBComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "CollisionComponent.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include <array>
namespace Collision
{
	struct MTV
	{
		MTV() {}
		MTV(Vector2D smol, float overla) : smallest(smol), overlap(overla) { }
		MTV& operator=(const MTV& rhs)
		{
			smallest = rhs.smallest;
			overlap = rhs.overlap;
			return *this;
		}
		~MTV() {}
		Vector2D smallest;
		float overlap;
	};
	enum CollisionType
	{
		NOTCOLLIDING = 0,
		COLLIDE = 1,
		COLLIDEBOTTOM =2,
		COLLIDELEFT =3,
		COLLIDETOP =4,
		COLLIDERIGHT=5,
	};
	bool SeperatingAxis(const AABBComponent* aabb1, const AABBComponent* aabb2, MTV& mtv);
	void SATCollisionResolution(RigidBodyComponent* rbComp1, TransformComponent* transComp1, CollisionComponent* colComp1, RigidBodyComponent* rbComp2, TransformComponent* transComp2, CollisionComponent* colComp2, const MTV& mtv);
}