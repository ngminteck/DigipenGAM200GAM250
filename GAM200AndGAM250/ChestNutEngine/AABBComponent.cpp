/******************************************************************************/
/*!
\file   AABBComponent.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Vector2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "AABBComponent.h"

AABBComponent::AABBComponent()
	:Component{ ComponentType::AABBComponent }
{
}

AABBComponent::AABBComponent(float scalex, float scaley, float offsetx, float offsety)
	: Component{ ComponentType::AABBComponent },
	m_aabbScale{scalex,scaley},
	m_aabb_init(true),
	m_offset(offsetx, offsety)
{
}


AABBComponent::AABBComponent(const AABBComponent & aabbComp)
	: Component(aabbComp),
	m_min(aabbComp.m_min),
	m_max(aabbComp.m_max),
	m_aabbScale{ aabbComp.m_aabbScale },
	m_aabb_init(true),
	m_offset(aabbComp.m_offset)
{ }

AABBComponent::~AABBComponent()
{ }


Component * AABBComponent::Clone() const
{
	return new AABBComponent(*this);
}

void AABBComponent::SetMin(Vector2D min)
{
	m_min = min;
}

void AABBComponent::SetMax(Vector2D max)
{
	m_max = max;
}

void AABBComponent::SetRect(Vector2D min,Vector2D max)
{
	m_rect.m_vertices[0] = { min.x,max.y };
	m_rect.m_vertices[1] = min;
	m_rect.m_vertices[2] = { max.x,min.y };
	m_rect.m_vertices[3] = max;
}

void AABBComponent::SetInit(bool val)
{
	m_aabb_init = val;
}

Vector2D AABBComponent::GetMin() const
{
	return m_min;
}

Vector2D AABBComponent::GetMax() const
{
	return m_max;
}

Rect AABBComponent::GetRect() const
{
	return m_rect;
}
Vector2D AABBComponent::GetAABBScale() const
{
	return m_aabbScale;
}

Vector2D AABBComponent::GetOffset() const
{
	return m_offset;
}

void AABBComponent::SetAABBScale(Vector2D vec)
{
	m_aabbScale.x = vec.x;
	m_aabbScale.y = vec.y;
}

void AABBComponent::SetOffset(Vector2D offset)
{
	m_offset = offset;
}


bool AABBComponent::GetInit() const
{
	return m_aabb_init;
}
void AABBComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("AABBComponent");
	auto map = metadata.reflect<TypeErasedObj>(*this);
	for (auto elem : map)
	{
		AutoWrite(writer, elem.first, elem.second.GetValue());
	}
}

void AABBComponent::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<AABBComponent>("AABBComponent",
		// Member functions
		"GetMin", &AABBComponent::GetMin,
		"GetMax", &AABBComponent::GetMax,
		"GetOffset", &AABBComponent::GetOffset,
		"SetOffset", &AABBComponent::SetOffset,

		sol::base_classes, sol::bases<Component>()
		);
}

bool CompareData(const AABBComponent & lhs, const AABBComponent & rhs)
{
	UNREFERENCED_PARAMETER(lhs);
	UNREFERENCED_PARAMETER(rhs);
	return true;
}