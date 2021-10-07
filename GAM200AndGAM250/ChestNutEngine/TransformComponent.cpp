#include "TransformComponent.h"


TransformComponent::TransformComponent()
	:Component{ ComponentType::TransformComponent }, m_IsUpdated{ true }, m_pos{ 0.0f,0.0f }, m_Translate{ 0,0,0 }, m_Rotate{ 0 }, m_Scale{ 0,0,0 }, parent_transformcomponent{ nullptr }
{
	//Default initialize transformation variables
}
TransformComponent::TransformComponent(Vector2D pos, Vector3D trans, float rot, Vector3D scale)
	: Component{ ComponentType::TransformComponent },
	m_pos{ pos },
	m_Translate{ trans }, m_Rotate{ rot }, m_Scale{ scale },
	m_IsUpdated{ true }, m_IsAABBUpdated{ true }, parent_transformcomponent{ nullptr }
{

}

TransformComponent::TransformComponent(const TransformComponent & rhs)
	:Component{ ComponentType::TransformComponent }, m_CompId {rhs.m_CompId}, m_IsUpdated{ rhs.m_IsUpdated }, m_IsAABBUpdated{ rhs.m_IsAABBUpdated },
	m_pos{rhs.m_pos }, m_Translate{rhs.m_Translate}, m_Rotate{rhs.m_Rotate }, m_Scale{rhs.m_Scale},
	m_AABBscale{rhs.m_AABBscale}, FinalMatrix{rhs.FinalMatrix}, AABBMatrix{rhs.AABBMatrix}, parent_transformcomponent{nullptr}
{

}

void TransformComponent::SetParentTransformComponent(TransformComponent *  parenttransformcomponent)
{
	parent_transformcomponent = parenttransformcomponent;
}

Component * TransformComponent::Clone() const 
{
	return new TransformComponent(*this);
}

Matrix4x4 TransformComponent::GetFinalMatrix()
{
	if (m_IsUpdated)
		ComputeMatrix();
	return FinalMatrix;
}

void TransformComponent::ComputeMatrix()
{
	FinalMatrix.identity();
	Matrix4x4 Scale;
	Scale.scale(m_Scale.x, m_Scale.y, m_Scale.z);
	Matrix4x4 Rotate;
	Rotate.rotateZ(m_Rotate);
	Matrix4x4 Trans;
	Trans.translate(m_Translate.x,m_Translate.y,0.f);
	FinalMatrix = Trans*Rotate*Scale*FinalMatrix;
	m_IsUpdated = false;
}

Matrix4x4 TransformComponent::GetAABBMatrix()
{
	if (m_IsAABBUpdated)
		ComputeAABBMatrix();
	return AABBMatrix;
}

void TransformComponent::ComputeAABBMatrix()
{
	AABBMatrix.identity();
	Matrix4x4 AABBScale;
	AABBScale.scale(m_AABBscale.x *m_Scale.x, m_AABBscale.y*m_Scale.y, 1.f);
	Matrix4x4 AABBRotate;
	AABBRotate.rotateZ(m_Rotate);
	Matrix4x4 AABBTrans;
	AABBTrans.translate(m_Translate.x, m_Translate.y , 0.f);
	AABBMatrix = AABBTrans * AABBRotate*AABBScale*AABBMatrix;
	m_IsAABBUpdated = false;
}

void TransformComponent::SetAABBScale(Vector2D scale)
{
	m_AABBscale = scale;
	m_IsAABBUpdated = true;
}

void TransformComponent::SetPosition(Vector2D pos)
{
	m_pos = pos;
	m_IsAABBUpdated = true;
}

void TransformComponent::SetScale(Vector3D scale)
{
	m_Scale = scale;
	m_IsUpdated = true;
}
void TransformComponent::SetRotate(float rot)
{
	m_Rotate = rot;
	m_IsUpdated = true;
	m_IsAABBUpdated = true;
}
void TransformComponent::SetTranslate(Vector3D trans)
{
	m_Translate = trans;
	m_IsUpdated = true;
	m_IsAABBUpdated = true;
}

void TransformComponent::Scale(float scale)
{
	m_Scale=m_Scale + scale;
	if (m_Scale.x < 1.f)
		m_Scale.x = 1.f;
	else if (m_Scale.x > 5.f)
		m_Scale.x = 5.f;

	if (m_Scale.y < 1.f)
		m_Scale.y = 1.f;
	else if (m_Scale.y > 5.f)
		m_Scale.y = 5.f;
	m_IsUpdated = true;
	m_IsAABBUpdated = true;
}
void TransformComponent::Rotate(float rot)
{
	m_Rotate += rot;
	if (m_Rotate > 360.f)
		m_Rotate = 0.f;
	m_IsUpdated = true;
	m_IsAABBUpdated = true;
}
void TransformComponent::Translate(Vector3D trans)
{
	m_Translate += trans;
	m_IsUpdated = true;
	m_IsAABBUpdated = true;
}

Vector2D TransformComponent::GetAABBScale() const
{
	return m_AABBscale;
}

Vector2D TransformComponent::GetPosition() const
{
	return m_pos;
}

Vector3D TransformComponent::GetScale()const
{
	return m_Scale;
}
float	 TransformComponent::GetRotate()const
{
	return m_Rotate;
}
Vector3D TransformComponent::GetTranslate()const
{
	return m_Translate;
}

void TransformComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("TransformComponent");
	auto map = metadata.reflect<TypeErasedObj>(*this);
	for (auto elem : map)
	{
		AutoWrite(writer, elem.first, elem.second.GetValue());
	}
}

void TransformComponent::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<TransformComponent>("TransformComponent",
		// Member functions
		"SetScale", &TransformComponent::SetScale,
		"SetRotate", &TransformComponent::SetRotate,
		"SetTranslate", &TransformComponent::SetTranslate,
		"SetPosition",&TransformComponent::SetPosition,
		"GetScale", &TransformComponent::GetScale,
		"GetRotate", &TransformComponent::GetRotate,
		"GetTranslate", &TransformComponent::GetTranslate,
		"GetPosition", &TransformComponent::GetPosition,
		"Scale",&TransformComponent::m_Scale,
		"Pos",&TransformComponent::m_pos,
		sol::base_classes, sol::bases<Component>()
		);
}

/*bool CompareData(const TransformComponent & lhs, const TransformComponent & rhs)
{
	// skip postion and scale
	if (lhs.GetRotate() != rhs.GetRotate())
		return false;

	return true;
}*/