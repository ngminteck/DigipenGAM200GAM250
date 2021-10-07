#pragma once
/******************************************************************************/
/*!
\file   ButtonFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the ButtonFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentFactory.h"
#include "ButtonComponent.h"
#include "SpriteSheetFactory.h"
class ButtonFactory : public ComponentFactory
{
public:
	ButtonFactory(SpriteSheetFactory *);
	~ButtonFactory();

	Component * CreateComponent(const rapidjson::Value& node) const;
	Component * CreateComponent() const;
private:
	SpriteSheetFactory * m_sprite;
};