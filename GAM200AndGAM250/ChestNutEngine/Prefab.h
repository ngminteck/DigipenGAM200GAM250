#pragma once
/******************************************************************************/
/*!
\file   Prefab.h

\author ONG, Zhi Hao Jaryl (100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login:  o.zhihaojaryl

\brief
This file contains function and stucts definition for Prefab

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/

/******************************************************************************/
#include <string>
#include "Component.h"
#include <unordered_map>

#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"
class Prefab
{
public:
	Prefab();
	~Prefab();
	void EmptyComponents();
private:
	std::unordered_map<ComponentType, Component *> m_components;
};