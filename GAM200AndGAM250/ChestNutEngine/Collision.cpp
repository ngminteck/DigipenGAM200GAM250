/******************************************************************************/
/*!
\file   Collision.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of Collision functions

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Collision.h"

namespace Collision
{
	void ComputeProjInterval(Rect a, Vector2D n, float& min, float& max)
	{
		float val;
		min = max = Vector2DDotProduct(n, a.m_vertices[0]);
		for (unsigned i = 1; i < a.m_vertices.size(); ++i)
		{
			val = Vector2DDotProduct(n, a.m_vertices[i]);
			if (val < min)
				min = val;
			else if (val > max)
				max = val;
		}
	}

	bool SeperatingAxis(const AABBComponent * aabb1, const AABBComponent * aabb2, MTV& mtv)
	{
		Rect a(aabb1->GetMin(), Vector2D(aabb1->GetMax().x, aabb1->GetMin().y),
			aabb1->GetMax(), Vector2D(aabb1->GetMin().x, aabb1->GetMax().y));
		Rect b(aabb2->GetMin(), Vector2D(aabb2->GetMax().x, aabb2->GetMin().y),
			aabb2->GetMax(), Vector2D(aabb2->GetMin().x, aabb2->GetMax().y));
		float minA, maxA, minB, maxB, overlap = 999999.0f;
		Vector2D smallest;
		Vector2D normal;
		Vector2D edge;
		for (unsigned i = 0; i < a.m_vertices.size(); ++i)
		{
			edge = a.m_vertices[(i + 1 == a.m_vertices.size() ? 0 : i + 1)] - a.m_vertices[i];
			normal = Vector2DNormal(edge);
			Vector2DNormalize(normal);
			ComputeProjInterval(a, normal, minA, maxA);
			ComputeProjInterval(b, normal, minB, maxB);
			if (minA > maxB || maxA < minB)
				return false;
			else
			{
				float o = fabs(minA - maxB);
				if (o < overlap)
				{
					overlap = o;
					smallest = normal;
				}
			}
		}
		for (unsigned i = 0; i < b.m_vertices.size(); ++i)
		{
			edge = b.m_vertices[(i + 1 == b.m_vertices.size() ? 0 : i + 1)] - b.m_vertices[i];
			normal = Vector2DNormal(edge);
			Vector2DNormalize(normal);
			ComputeProjInterval(a, normal, minA, maxA);
			ComputeProjInterval(b, normal, minB, maxB);
			if (minA > maxB || maxA < minB)
				return false;
			else
			{
				float o = fabs(minB - maxA);
				if (o < overlap)
				{
					overlap = o;
					smallest = normal;
				}
			}
		}
		mtv = { smallest, overlap };
		return true;
	}

	void SATCollisionResolution(RigidBodyComponent* rbComp1, TransformComponent* transComp1, CollisionComponent* colComp1, RigidBodyComponent* rbComp2, TransformComponent* transComp2, CollisionComponent* colComp2, const MTV& mtv)
	{
		if (!rbComp1->GetIsStaticObj())
		{
			if (mtv.smallest.y > 0.0f) // push up
			{
				if(!colComp1->GetCollidingBottom())
					transComp1->SetPosition(Vector2D(transComp1->GetPosition().x, transComp1->GetPosition().y + mtv.smallest.y * mtv.overlap));
				rbComp1->SetAcceleration(Vector2D(rbComp1->GetAcceleration().x, 0.0f));
				rbComp1->SetVelocity(Vector2D(rbComp1->GetVelocity().x, 0.0f));
				if (colComp1)
					colComp1->SetCollidingBottom(true);
			}
			if (mtv.smallest.x < 0.0f) // push left
			{
				if (!colComp1->GetCollidingRight())
					transComp1->SetPosition(Vector2D(transComp1->GetPosition().x + mtv.smallest.x * mtv.overlap, transComp1->GetPosition().y));
				rbComp1->SetAcceleration(Vector2D(0.0f, rbComp1->GetAcceleration().y));
				rbComp1->SetVelocity(Vector2D(0.0f, rbComp1->GetVelocity().y));
				if (colComp1)
					colComp1->SetCollidingRight(true);
			}
			if (mtv.smallest.x > 0.0f) // push right
			{
				if (!colComp1->GetCollidingLeft())
					transComp1->SetPosition(Vector2D(transComp1->GetPosition().x + mtv.smallest.x * mtv.overlap, transComp1->GetPosition().y));
				rbComp1->SetAcceleration(Vector2D(0.0f, rbComp1->GetAcceleration().y));
				rbComp1->SetVelocity(Vector2D(0.0f, rbComp1->GetVelocity().y));
				if (colComp1)
					colComp1->SetCollidingLeft(true);
			}
			if (mtv.smallest.y < 0.0f) // push down
			{
				if (!colComp1->GetCollidingTop())
					transComp1->SetPosition(Vector2D(transComp1->GetPosition().x, transComp1->GetPosition().y + mtv.smallest.y * mtv.overlap));
				rbComp1->SetAcceleration(Vector2D(rbComp1->GetAcceleration().x, 0.0f));
				rbComp1->SetVelocity(Vector2D(rbComp1->GetVelocity().x, 0.0f));
				if (colComp1)
					colComp1->SetCollidingTop(true);
			}
		}
		else if (!rbComp2->GetIsStaticObj())
		{
			//transComp2->SetPosition(transComp2->GetPosition() - mtv.smallest * mtv.overlap);
			if (mtv.smallest.y < 0.0f) // push up
			{
				if (!colComp2->GetCollidingBottom())
					transComp2->SetPosition(Vector2D(transComp2->GetPosition().x, transComp2->GetPosition().y - mtv.smallest.y * mtv.overlap));
				rbComp2->SetAcceleration(Vector2D(rbComp2->GetAcceleration().x, 0.0f));
				rbComp2->SetVelocity(Vector2D(rbComp2->GetVelocity().x, 0.0f));
				if (colComp2)
					colComp2->SetCollidingBottom(true);
			}
			if (mtv.smallest.x < 0.0f) // push right 
			{
				if (!colComp2->GetCollidingLeft())
					transComp2->SetPosition(Vector2D(transComp2->GetPosition().x - mtv.smallest.x * mtv.overlap, transComp2->GetPosition().y));
				rbComp2->SetAcceleration(Vector2D(0.0f, rbComp2->GetAcceleration().y));
				rbComp2->SetVelocity(Vector2D(0.0f, rbComp2->GetVelocity().y));
				if (colComp2)
					colComp2->SetCollidingLeft(true);
			}
			if (mtv.smallest.x > 0.0f) // push left
			{
				if (!colComp2->GetCollidingRight())
					transComp2->SetPosition(Vector2D(transComp2->GetPosition().x - mtv.smallest.x * mtv.overlap, transComp2->GetPosition().y));
				rbComp2->SetAcceleration(Vector2D(0.0f, rbComp2->GetAcceleration().y));
				rbComp2->SetVelocity(Vector2D(0.0f, rbComp2->GetVelocity().y));
				if (colComp2)
					colComp2->SetCollidingRight(true);
			}
			if (mtv.smallest.y > 0.0f) // push down
			{
				if (!colComp2->GetCollidingTop())
					transComp2->SetPosition(Vector2D(transComp2->GetPosition().x, transComp2->GetPosition().y - mtv.smallest.y * mtv.overlap));
				rbComp2->SetAcceleration(Vector2D(rbComp2->GetAcceleration().x, 0.0f));
				rbComp2->SetVelocity(Vector2D(rbComp2->GetVelocity().x, 0.0f));
				if (colComp2)
					colComp2->SetCollidingTop(true);
			}
		}
	}

}