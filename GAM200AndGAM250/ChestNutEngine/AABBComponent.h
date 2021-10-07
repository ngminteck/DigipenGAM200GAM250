/******************************************************************************/
/*!
\file   AABBComponent.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Vector2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "Vector2D.h"
#include "Shapes.h"

class AABBComponent : public Component
{
private:
	Vector2D m_min;
	Vector2D m_max;
	Rect m_rect;
	Vector2D m_aabbScale;
	bool m_aabb_init;
	Vector2D m_offset;
public:
	AABBComponent();
	AABBComponent(float scalex, float scaley, float offsetx, float offsety);
	AABBComponent(const AABBComponent& aabbComp);
	~AABBComponent();

	Component * Clone() const;

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	

	void SetMin(Vector2D min);
	void SetMax(Vector2D max);
	void SetRect(Vector2D min, Vector2D max);
	void SetInit(bool val);
	void SetAABBScale(Vector2D);
	void SetOffset(Vector2D offset);

	Vector2D GetMin() const;
	Vector2D GetMax() const;
	Rect GetRect() const;
	Vector2D GetAABBScale() const;
	Vector2D GetOffset() const;
	bool GetInit() const;
	
	constexpr static Metadata <AABBComponent, Vector2D AABBComponent::*, Vector2D AABBComponent::*,
								Rect AABBComponent::*, Vector2D AABBComponent::*, Vector2D AABBComponent::*> metadata
	{ "AABBComponent", std::make_tuple(&AABBComponent::m_min, &AABBComponent::m_max,
										&AABBComponent::m_rect, &AABBComponent::m_aabbScale,
										&AABBComponent::m_offset),
										std::make_tuple("m_min", "m_max", "m_rect", "m_aabbScale", "m_offset") };
	
	static void AddToScript(sol::state * luaState);
};

bool CompareData(const AABBComponent & lhs, const AABBComponent & rhs);