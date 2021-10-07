/******************************************************************************/
/*!
\file   ComponentFactory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the ComponentFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H
#include "Component.h"
#include "GameObject.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

class ComponentFactory
{
public:
	ComponentFactory() {};
	virtual ~ComponentFactory() {};
	virtual Component * CreateComponent() const = 0;
	virtual Component * CreateComponent(const rapidjson::Value& node) const = 0;
};

#endif


