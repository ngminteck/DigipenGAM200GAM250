/******************************************************************************/
/*!
\file   SpriteFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the SpriteFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentFactory.h"
#include "SpriteComponent.h"
#include "SpriteSheetFactory.h"
#include "MeshFactory.h"
class SpriteFactory : public ComponentFactory
{
private:
	SpriteSheetFactory * m_SpriteSheetFactory;
	//MeshFactory		*m_MeshFactory;
public:
	SpriteFactory(SpriteSheetFactory* spritesheet_fac);
	~SpriteFactory();

	void Initialize();

	Component * CreateComponent(const rapidjson::Value& node) const;
	Component * CreateComponent(std::string texture , std::string anime, bool bool_value) const;
	Component * CreateComponent() const;

	
};