#pragma once
#include "imgui.h"
#include "imgui_stl.h"
#include "TransformComponent.h"
#include "AABBComponent.h"
struct EditorGizmo
{
	EditorGizmo(TransformComponent *, AABBComponent *);
	EditorGizmo();
	~EditorGizmo();
	void Update();
	void Draw();

	TransformComponent * m_trans;
	AABBComponent      * m_aabb;
};
