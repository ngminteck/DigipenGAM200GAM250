/******************************************************************************/
/*!
\file  Component.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\author Ng Min Teck(50%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains function and structs definition for Component and auto writer

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Component.h"

Component::Component(ComponentType id)
	:m_CompId{id}
{
	
}

Component::Component(const Component & comp)
	:m_CompId(comp.m_CompId)
{
}

Component::~Component()
{
}

ComponentType Component::GetComponentType() const
{
	return m_CompId;
}

void  Component::AutoWrite(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer, std::string key, std::any value)
{
	if (value.type() == typeid(int *))
	{
		writer.Key(key.c_str());
		writer.Int(*(std::any_cast<int *>(value)));
	}
	else if (value.type() == typeid(unsigned *))
	{
		writer.Key(key.c_str());
		writer.Int(*(std::any_cast<unsigned *>(value)));
	}
	else if (value.type() == typeid(float *))
	{
		writer.Key(key.c_str());
		writer.Double(*(std::any_cast<float *>(value)));
	}
	else if (value.type() == typeid(double *))
	{
		writer.Key(key.c_str());
		writer.Double(*(std::any_cast<double *>(value)));
	}
	else if (value.type() == typeid(bool *))
	{
		writer.Key(key.c_str());
		writer.Bool(*(std::any_cast<bool *>(value)));
	}
	else if (value.type() == typeid(std::string *))
	{
		writer.Key(key.c_str());
		writer.String((*(std::any_cast<std::string *>(value))).c_str());
	}
	else if (value.type() == typeid(Vector2D *))
	{
		std::stringstream x;
		std::stringstream y;

		x.str(std::string());
		y.str(std::string());

		x << key << ".x";
		y << key << ".y";

		writer.Key(x.str().c_str());
		writer.Double((*(std::any_cast<Vector2D *>(value))).x);
		writer.Key(y.str().c_str());
		writer.Double((*(std::any_cast<Vector2D *>(value))).y);

	}
	else if (value.type() == typeid(Vector3D *))
	{
		std::stringstream x;
		std::stringstream y;
		std::stringstream z;

		x.str(std::string());
		y.str(std::string());
		z.str(std::string());

		x << key << ".x";
		y << key << ".y";
		z << key << ".z";

		writer.Key(x.str().c_str());
		writer.Double((*(std::any_cast<Vector3D *>(value))).x);
		writer.Key(y.str().c_str());
		writer.Double((*(std::any_cast<Vector3D *>(value))).y);
		writer.Key(z.str().c_str());
		writer.Double((*(std::any_cast<Vector3D *>(value))).z);
	}
	/*else if (value.type() == typeid(glm::vec3**))
	{
		std::stringstream x;
		std::stringstream y;
		std::stringstream z;

		x.str(std::string());
		y.str(std::string());
		z.str(std::string());

		x << key << ".x";
		y << key << ".y";
		z << key << ".z";

		writer.Key(x.str().c_str());
		writer.Double((*(std::any_cast<Vector3D *>(value))).x);
		writer.Key(y.str().c_str());
		writer.Double((*(std::any_cast<Vector3D *>(value))).y);
		writer.Key(z.str().c_str());
		writer.Double((*(std::any_cast<Vector3D *>(value))).z);
	}*/
	//else if (value.type() == typeid(State *))
	//{
	//	writer.Key(key.c_str());
	//	writer.Int(*(std::any_cast<State *>(value)));
	//}
	else if (value.type() == typeid(Direction *))
	{
		writer.Key(key.c_str());
		writer.Int(*(std::any_cast<Direction *>(value)));
	}
	else if (value.type() == typeid(Rect *))
	{
		std::stringstream x;
		std::stringstream y;
		for (auto it = (*(std::any_cast<Rect *>(value))).m_vertices.begin(); it != (*(std::any_cast<Rect *>(value))).m_vertices.end(); ++it)
		{
			x.str(std::string());
			y.str(std::string());

			x << key << "." << it - (*(std::any_cast<Rect *>(value))).m_vertices.begin() << ".x";
			y << key << "." << it - (*(std::any_cast<Rect *>(value))).m_vertices.begin() << ".y";

			writer.Key(x.str().c_str());
			writer.Double((*it).x);
			writer.Key(y.str().c_str());
			writer.Double((*it).y);

		}

	}

	else
	{
		std::cout << key << " cannot be identified" << std::endl;
	}



}

void Component::AddToScript(sol::state* luaState)
{
	luaState->new_enum("ComponentType",
		"UnknownComponent"	, ComponentType::UnknownComponent,
		"Transform", ComponentType::TransformComponent,
		"Sprite"	, ComponentType::SpriteComponent,
		"Movement"	, ComponentType::MovementComponent,
		"Collision", ComponentType::CollisionComponent,
		"RigidBody", ComponentType::RigidBodyComponent,
		"Stats"	, ComponentType::StatsComponent,
		"AABB"		, ComponentType::AABBComponent,
		"Script"	, ComponentType::ScriptComponent,
		"Audio"	, ComponentType::AudioComponent);
	luaState->new_usertype	<Component> ("Component",
		"GetComponentType",&Component::GetComponentType
		);
}

std::string checkComponentType(ComponentType comptype)
{
	switch (comptype)
	{
	case ComponentType::TransformComponent: return "TransformComponent";
	case ComponentType::SpriteComponent:  return "SpriteComponent";
	case ComponentType::MovementComponent:  return "MovementComponent";
	case ComponentType::CollisionComponent:  return "CollisionComponent";
	case ComponentType::RigidBodyComponent:  return "RigidBodyComponent";
	case ComponentType::StatsComponent:  return "StatsComponent";
	case ComponentType::AABBComponent:  return "AABBComponent";
	case ComponentType::ScriptComponent:  return "ScriptComponent";
	case ComponentType::AudioComponent:  return "AudioComponent";
	case ComponentType::ButtonComponent:  return "ButtonComponent";
	case ComponentType::CameraComponent:  return "CameraComponent";
	default: return "UnknownComponent";
	}
}