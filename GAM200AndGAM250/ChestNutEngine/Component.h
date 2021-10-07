#pragma once
/******************************************************************************/
/*!
\file  Component.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and stucts definition for Component class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/*****************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H
#include <sstream>
#include "Metadata.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <memory>

#include "State.h"
#include "Shapes.h"

enum class ComponentType :int
{
	UnknownComponent    = 0,
	TransformComponent	= 1,
	SpriteComponent		= 2,
	MovementComponent	= 3,
	CollisionComponent	= 4,
	RigidBodyComponent	= 5,
	StatsComponent      = 6,
	AABBComponent		= 7,
	ScriptComponent		= 12,
	AudioComponent		= 13,
	ButtonComponent     = 14,
	CameraComponent     = 15,
	VolumeComponent     = 16,
	CheckpointComponent = 17,
	DialogueComponent   = 18
};

class Component
{
private:
	ComponentType m_CompId;
	
public:
	Component(ComponentType id);
	Component(const Component&);
	virtual ~Component();
	virtual void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer) = 0;
	virtual Component * Clone() const = 0;

	ComponentType GetComponentType() const;

	void AutoWrite(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer, std::string key, std::any value );
	
	static void AddToScript(sol::state * luaState);
};

std::string checkComponentType(ComponentType comptype);

#endif