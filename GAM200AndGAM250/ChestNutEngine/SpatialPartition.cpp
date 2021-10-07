/******************************************************************************/
/*!
\file   SpatialPartition.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of SpatialPartition

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SpatialPartition.h"
namespace SpatialPartition
{
	void SweepAndPrune(const std::vector<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>>& axisVec, std::vector<std::pair<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>, std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>>>& result)
	{
		std::vector<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>> activeVec;
		activeVec.reserve(1000);
		for (int i = 0; i < axisVec.size(); ++i)
		{
			activeVec.push_back(axisVec[i]);
			for (size_t j = activeVec.size() - 1; j > 0; --j)
			{
				if (std::get<3>(activeVec[activeVec.size() - 1])->GetMin().x <= std::get<3>(activeVec[j - 1])->GetMax().x)
					result.push_back(std::pair<std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>, std::tuple<CollisionComponent*, RigidBodyComponent*, TransformComponent*, AABBComponent*, SceneActor*>>(activeVec[activeVec.size() - 1], activeVec[j - 1]));//form pair
				else if (std::get<3>(activeVec[activeVec.size() - 1])->GetMin().x > std::get<3>(activeVec[j - 1])->GetMax().x)
					activeVec.erase(activeVec.begin() + j - 1);
				else break;
			}
		}
	}
}