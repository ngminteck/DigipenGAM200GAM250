/******************************************************************************/
/*!
\file   SpriteComponent.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains the declarations of SpriteComponent class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SpriteComponent.h"
SpriteComponent::SpriteComponent()
:Component {ComponentType::SpriteComponent},m_Tex{nullptr}, m_Animation{nullptr}, animated_texture{ false },m_Mesh{nullptr}, detect_toggle_change{ animated_texture }
,m_FrameCount{ 0 }, m_timeCount{ 0.0f }, m_OrthogonalCamera{false}
{

}

SpriteComponent::SpriteComponent(Texture2D *tex, Animation* animation, std::string spritename , std::string texturename, bool orthogonal_camera = false , unsigned layer=2, bool bool_value=false)
: Component{ ComponentType::SpriteComponent }, m_Tex{ tex }, m_Animation{ animation }, animated_texture{ bool_value }, detect_toggle_change{ animated_texture },
m_layer{layer}, m_FrameCount{ 0 }, m_timeCount{ 0.0f }, m_OrthogonalCamera{orthogonal_camera}
{
	if(animation) m_Mesh =  animation->GetMesh() ;
	m_SpriteName = spritename;
	m_TextureName = texturename;
}
SpriteComponent::SpriteComponent(const SpriteComponent&rhs )
  :Component{rhs.GetComponentType()},
  m_Tex{ rhs.m_Tex }, m_Animation{ rhs.m_Animation },
  animated_texture{ rhs.animated_texture }, detect_toggle_change{ rhs.detect_toggle_change },
  m_layer{ rhs.m_layer }, m_FrameCount{ 0 }, m_timeCount{ 0.0f },
  m_SpriteName{rhs.m_SpriteName},m_TextureName{rhs.m_TextureName},
  m_Mesh{rhs.m_Mesh}, m_OrthogonalCamera{rhs.m_OrthogonalCamera}
{

}
SpriteComponent::~SpriteComponent()
{
}

Component * SpriteComponent::Clone() const
{
	return new SpriteComponent(*this);
}

std::string SpriteComponent::GetSpriteName() const
{
	return m_SpriteName;
}
std::string SpriteComponent::GetTextureName() const
{
	return m_TextureName;
}

void SpriteComponent::SetSpriteName(std::string name)
{
	m_SpriteName = name;
}

void SpriteComponent::SetTextureName(std::string name)
{
	m_TextureName = name;
}

void SpriteComponent::SetLayer(unsigned layer)
{
	m_layer = layer;
}

bool * SpriteComponent::GetAnimatedTexturePtr() 
{
	return &animated_texture;
}

void SpriteComponent::CheckBoolAnimatedTextureChange()
{
	if (animated_texture != detect_toggle_change)
	{
		m_SpriteName = "NO SELECTED";
		m_TextureName = "NO SELECTED";
		m_Tex = nullptr;
		detect_toggle_change = animated_texture;
	}
}

void SpriteComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("SpriteComponent");
	writer.Key("Sprite_Name");
	writer.String(m_SpriteName.c_str());
	writer.Key("Texture_Name");
	writer.String(m_TextureName.c_str());
	writer.Key("Animated_Texture");
	writer.Bool(animated_texture);
	writer.Key("Orthogonal_Camera");
	writer.Bool(m_OrthogonalCamera);
	writer.Key("Layer");
	writer.Int(m_layer);
}

Texture2D*	SpriteComponent::GetTexture() const
{
	return m_Tex;
}
Animation* SpriteComponent::GetAnimation()const
{
	return m_Animation;
}
Mesh*	SpriteComponent::GetMesh() const
{
	return m_Mesh;
}

unsigned SpriteComponent::GetLayer() const
{
	return m_layer;
}

unsigned * SpriteComponent::GetLayerPtr()
{
	return &m_layer;
}

bool SpriteComponent::IsOrthCam() const
{
	return m_OrthogonalCamera;
}

unsigned SpriteComponent::GetFrameCount() const
{
	return m_FrameCount;
}

float SpriteComponent::GetTimeCount() const
{
	return m_timeCount;
}

void SpriteComponent::SetTexture(Texture2D* texname)
{
	m_Tex = texname;
}

void SpriteComponent::SetOrthCam(bool value)
{
	m_OrthogonalCamera = value;
}


void SpriteComponent::SetSprite(std::string aniname)
{
	m_Animation = m_Tex->GetAnimationData(aniname);
	Mesh* tmp = m_Tex->GetAnimationMesh(aniname);
	if (tmp)
		m_Mesh = tmp;
}
void SpriteComponent::SetAnimation(std::string aniname, MovementComponent* movecomp)
{
	std::string retVal;
	switch (movecomp->GetDirection())
	{
	case LEFT:
		retVal = "LEFT_";
		break;
	case RIGHT:
		retVal = "RIGHT_";
		break;
	}
	retVal += aniname;
	m_Animation = m_Tex->GetAnimationData(retVal);
	Mesh* tmp = m_Tex->GetAnimationMesh(retVal);
	if (tmp)
		m_Mesh = tmp;
}
void SpriteComponent::SetFrameCount(unsigned c)
{
	m_FrameCount = c;
}

void SpriteComponent::SetTimeCount(float val)
{
	m_timeCount = val;
}

void SpriteComponent::SetAnimatedTexture(bool value)
{
	animated_texture = value;
}

void SpriteComponent::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<SpriteComponent>("SpriteComponent",
		// Member functions
		"SetSprite", &SpriteComponent::SetSprite,
		"SetFrameCount", &SpriteComponent::SetFrameCount,
		"SetTimeCount", &SpriteComponent::SetTimeCount,
		"SetAnimation",&SpriteComponent::SetAnimation,
		"GetFrameCount", &SpriteComponent::GetFrameCount,
		"GetSpriteName", &SpriteComponent::GetSpriteName,
		"GetTextureName", &SpriteComponent::GetTextureName,
		sol::base_classes, sol::bases<Component>()
		);
}

bool CompareData(const SpriteComponent & lhs, const SpriteComponent & rhs)
{
	if (lhs.GetSpriteName().compare(rhs.GetSpriteName()) != 0)
		return false;
	if (lhs.GetTextureName().compare(rhs.GetTextureName()) != 0)
		return false;
	//if (lhs.GetLayer() != rhs.GetLayer())
		//return false;
	return true;
}
