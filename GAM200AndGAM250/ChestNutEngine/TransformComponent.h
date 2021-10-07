/******************************************************************************/
/*!
\file   TransformComponent

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototype for the TransformComponent class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "Matrix4x4.h"



class TransformComponent : public Component
{

private:
	int m_CompId;

	bool m_IsUpdated;
	bool m_IsAABBUpdated;

	Vector2D m_pos;
	Vector3D m_Translate;	// Translation of object along X,Y
	float m_Rotate;			// Rotation along Z axis
	Vector3D m_Scale;		// Scale of object in X,Y
	Vector2D m_AABBscale;

	Matrix4x4 FinalMatrix;
	Matrix4x4 AABBMatrix;

	TransformComponent * parent_transformcomponent;
	
public:
	TransformComponent();
	TransformComponent(Vector2D pos, Vector3D trans, float rot, Vector3D scale);
	TransformComponent(const TransformComponent & rhs);
	~TransformComponent() {}
	Component * Clone() const override;

	void SetParentTransformComponent(TransformComponent *  parenttransformcomponent);

	Matrix4x4 GetFinalMatrix();
	void ComputeMatrix();

	Matrix4x4 GetAABBMatrix();
	void ComputeAABBMatrix();

	void SetAABBScale(Vector2D);
	void SetPosition(Vector2D pos);
	void SetScale(Vector3D);
	void SetRotate(float);
	void SetTranslate(Vector3D);

	void Scale(float);
	void Rotate(float);
	void Translate(Vector3D);

	Vector2D GetAABBScale() const;

	Vector2D GetPosition() const;

	Vector3D GetScale()const;
	float GetRotate()const;
	Vector3D GetTranslate()const;

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);
	constexpr static Metadata < TransformComponent, Vector2D TransformComponent::*, Vector3D TransformComponent::*, float TransformComponent::*, Vector3D TransformComponent::*> metadata
	{ "TransformComponent", std::make_tuple(&TransformComponent::m_pos, &TransformComponent::m_Translate,
		&TransformComponent::m_Rotate, &TransformComponent::m_Scale),
		std::make_tuple("m_Pos", "m_Translate", "m_Rotate", "m_Scale") };
	
	static void AddToScript(sol::state * luaState);
};

//bool CompareData(const TransformComponent & lhs, const TransformComponent & rhs);