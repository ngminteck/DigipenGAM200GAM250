/******************************************************************************/
/*!
\file   SpriteComponent.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains the declarations of SpriteComponent class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include "MovementComponent.h"
#include "Texture2D.h"

class SpriteComponent :public Component
{

private:
	Texture2D * m_Tex;
	Animation * m_Animation;
	Mesh * m_Mesh;
	bool isAnimated;
	unsigned m_FrameCount;
	float m_timeCount;

	unsigned m_layer;
	std::string m_SpriteName;
	std::string m_TextureName;
	bool animated_texture;
	bool detect_toggle_change;

public:
	SpriteComponent();
	SpriteComponent(Texture2D *tex, Animation* animation, std::string spritename, std::string texturename,bool orthogonal_camera, unsigned layer, bool bool_value);
  SpriteComponent(const SpriteComponent&);

  ~SpriteComponent();

   bool m_OrthogonalCamera;

	Component * Clone() const override;

	Texture2D*	GetTexture()	const;
	Mesh*		GetMesh()		const;
	Animation*	GetAnimation()	const;
	unsigned	GetFrameCount() const;
	float		GetTimeCount() const;


	void		SetTexture(Texture2D* texname);
	void		SetSprite( std::string aniname);
	void		SetAnimation(std::string aniname, MovementComponent*);
	void		SetLayer(unsigned);
	void		SetFrameCount(unsigned c);
	void		SetTimeCount(float val);


	std::string GetSpriteName() const;
	std::string GetTextureName() const;
	bool* GetAnimatedTexturePtr();

	void CheckBoolAnimatedTextureChange();

	void SetSpriteName(std::string name);
	void SetTextureName(std::string name);

	unsigned GetLayer() const;
	unsigned * GetLayerPtr();

	bool IsOrthCam() const;
	void SetOrthCam(bool value);

	void SetAnimatedTexture(bool value);

	//void ApplyTexture(std::string spritesheet_name, std::string texture_name);

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	constexpr static Metadata <SpriteComponent, Texture2D * SpriteComponent::*, unsigned SpriteComponent::*,
		unsigned SpriteComponent::*, float SpriteComponent::*, bool SpriteComponent::*> metadata
	{ "Sprite Component", std::make_tuple(&SpriteComponent::m_Tex, &SpriteComponent::m_layer, &SpriteComponent::m_FrameCount,
										   &SpriteComponent::m_timeCount, &SpriteComponent::m_OrthogonalCamera),
		std::make_tuple("m_Tex", "m_layer", "m_FrameCount", "m_timeCount", "m_isOrthogonal") };

	static void AddToScript(sol::state * luaState);


};

bool CompareData(const SpriteComponent & lhs, const SpriteComponent & rhs);