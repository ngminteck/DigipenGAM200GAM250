#pragma once
/******************************************************************************/
/*!
\file  CollisioNFactory.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for CollisionFactory

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ComponentFactory.h"
#include "CollisionComponent.h"
class CollisionFactory : public ComponentFactory
{
public:
	Component * CreateComponent(const rapidjson::Value& node) const { UNREFERENCED_PARAMETER(node); return new CollisionComponent{}; }
	Component * CreateComponent() const { return new CollisionComponent{}; }
};