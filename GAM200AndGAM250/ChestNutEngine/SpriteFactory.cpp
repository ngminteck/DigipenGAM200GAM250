/******************************************************************************/
/*!
\file   SpriteFactory.cpp

\author WONG, Zhi Jun Justin(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains the prototypes for the RigidBodyFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SpriteFactory.h"

SpriteFactory::SpriteFactory(SpriteSheetFactory* spritesheet_fac)
:m_SpriteSheetFactory{ spritesheet_fac }
{
}	
SpriteFactory::~SpriteFactory()
{
	//delete m_MeshFactory;
}
void SpriteFactory::Initialize()
{
	//m_MeshFactory->LoadFromFile("Resource/Json/ResourceData/mesh.json");
}

Component * SpriteFactory::CreateComponent(const rapidjson::Value& node) const
{
	if (node.HasMember("Sprite_Name") && node.HasMember("Texture_Name") && node.HasMember("Animated_Texture") && node.HasMember("Layer"))
	{
		unsigned key_result = GetSpriteKeyBySpriteName(m_SpriteSheetFactory->GetContainer(), node["Sprite_Name"].GetString());
		auto tex = m_SpriteSheetFactory->GetContainer()->find(key_result)->second;
		auto animation = tex->GetAnimationData(node["Texture_Name"].GetString());

		bool oth_cam = false;

		if (node.HasMember("Orthogonal_Camera"))
		{
			oth_cam = node["Orthogonal_Camera"].GetBool();
		}

		return new SpriteComponent{ tex ,animation ,node["Sprite_Name"].GetString() , node["Texture_Name"].GetString(),oth_cam, node["Layer"].GetUint() ,node["Animated_Texture"].GetBool() };
	}
	else
	{
		auto tex = m_SpriteSheetFactory->GetContainer()->find(0)->second;
		auto animation = tex->GetAnimationData("CIRCLE");
		return new SpriteComponent{ tex ,animation ,tex->GetTextureName() ,"CIRCLE",false,2, false };
	}
}

Component * SpriteFactory::CreateComponent(std::string texture, std::string anim, bool bool_value) const
{
	unsigned key_result = GetSpriteKeyBySpriteName(m_SpriteSheetFactory->GetContainer(), texture);
	auto tex = m_SpriteSheetFactory->GetContainer()->find(key_result)->second;
	auto animation = tex->GetAnimationData(anim);

	return new SpriteComponent{ tex ,animation , texture, anim ,false,2,  bool_value };
}

// this u can't have default sprite component, cause null is not accepted
Component * SpriteFactory::CreateComponent() const 
{ 
	return new SpriteComponent{};
}
