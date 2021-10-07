/******************************************************************************/
/*!
\file   CameraFactory.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file camera factory function

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include "ComponentFactory.h"
#include "rapidjson/document.h"

class CameraFactory : public ComponentFactory
{
public:
	CameraFactory() {}
	~CameraFactory() {}
	Component * CreateComponent(const rapidjson::Value& node) const override;
	Component * CreateComponent() const override;
};