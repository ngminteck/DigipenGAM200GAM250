#pragma once
/******************************************************************************/
/*!
\file   TransformFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the ComponentFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ComponentFactory.h"
#include "rapidjson/document.h"

class TransformFactory : public ComponentFactory
{
public: 
	 TransformFactory() {}
	~TransformFactory() {}
    Component * CreateComponent(const rapidjson::Value& node) const override;
	Component * CreateComponent() const override;
};
