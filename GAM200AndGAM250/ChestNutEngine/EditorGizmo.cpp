#include "EditorGizmo.h"

EditorGizmo::EditorGizmo(TransformComponent * trans, AABBComponent * aabb)
	:m_trans{trans}, m_aabb{aabb}
{
}

EditorGizmo::EditorGizmo()
{
}



EditorGizmo::~EditorGizmo()
{
}

void EditorGizmo::Update()
{
	
}

void EditorGizmo::Draw()
{

	ImGui::GetWindowDrawList()->AddRect({ 30,30 }, { 40,40 }, 3);


}
