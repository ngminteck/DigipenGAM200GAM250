/******************************************************************************/
/*!
\file   RigidBodyComponent.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementaiton of RigidBodyComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent()
	:Component{ ComponentType::RigidBodyComponent }, 
	m_vel{ 0.0f, 0.0f },
	m_accel(0.0f, 0.0f),
	m_direction(0.0f, 0.0f),
	m_mass(1.0f),
	m_gravity(0.0f),
	m_onGround(false),
	m_pushRight(false),
	m_pushLeft(false),
	m_hitCeiling(false),
	m_isMoving(false),
	m_startTime(0.0f),
	m_duration(0.0f),
	m_objType("Default"),
	m_isStaticObj(true),
	m_hasGravity(false)
{
	
}

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent & rbComp)
	:Component(rbComp),
	m_vel(rbComp.m_vel),
	m_accel(rbComp.m_accel),
	m_direction(rbComp.m_direction),
	m_mass(rbComp.m_mass),
	m_gravity(rbComp.m_gravity),
	m_onGround(rbComp.m_onGround),
	m_pushRight(rbComp.m_pushRight),
	m_pushLeft(rbComp.m_pushLeft),
	m_hitCeiling(rbComp.m_hitCeiling),
	m_isMoving(rbComp.m_isMoving),
	m_startTime(rbComp.m_startTime),
	m_duration(rbComp.m_duration),
	m_objType(rbComp.m_objType),
	m_isStaticObj(rbComp.m_isStaticObj),
	m_hasGravity(rbComp.m_hasGravity),
	m_modA(rbComp.m_modA),
	m_modB(rbComp.m_modB)
	
{ }

RigidBodyComponent::RigidBodyComponent(Vector2D vel, Vector2D accel, Vector2D dir, float mass, float grav, 
									bool isStatic, float startTime, float duration, float modA, float modB, float modC,
									std::string objType, bool hasGravity)
	: Component{ ComponentType::RigidBodyComponent }, 
	m_vel{ vel },
	m_accel{ accel }, 
	m_direction{ dir },
	m_mass{ mass },
	m_gravity{ grav },
	m_isStaticObj{ isStatic },
	m_startTime{ startTime },
	m_duration{ duration },
	m_modA{ modA },
	m_modB{ modB },
	m_modC{ modC },
	m_objType{ objType },
	m_hasGravity{ hasGravity }
{
}

RigidBodyComponent::~RigidBodyComponent()
{

}

Component * RigidBodyComponent::Clone() const
{
	return new RigidBodyComponent(*this);
}

void RigidBodyComponent::SetVelocity(Vector2D vel)
{
	m_vel = vel;
}

void RigidBodyComponent::SetAcceleration(Vector2D accel)
{
	m_accel = accel;
}

void RigidBodyComponent::SetForce(Force f)
{
	m_force = f;
}

void RigidBodyComponent::SetMass(float mass)
{
	m_mass = mass;
}

void RigidBodyComponent::SetGravity(float grav)
{
	m_gravity = grav;
}

void RigidBodyComponent::SetOnGround(bool val)
{
	m_onGround = val;
}

void RigidBodyComponent::SetPushRight(bool val)
{
	m_pushRight = val;
}

void RigidBodyComponent::SetPushLeft(bool val)
{
	m_pushLeft = val;
}

void RigidBodyComponent::SetHitCeiling(bool val)
{
	m_hitCeiling = val;
}

void RigidBodyComponent::SetIsMoving(bool val)
{
	m_isMoving = val;
}

void RigidBodyComponent::SetHasGravity(bool val)
{
	m_hasGravity = val;
}

void RigidBodyComponent::SetObjType(std::string str)
{
	m_objType = str;
}

void RigidBodyComponent::SetStartTime(float val)
{
	m_startTime = val;
}

void RigidBodyComponent::SetDuration(float val)
{
	m_duration = val;
}

void RigidBodyComponent::SetCounter(float val)
{
	m_counter = val;
}

void RigidBodyComponent::SetDirection(Vector2D val)
{
	m_direction = val;
}

void RigidBodyComponent::SetModA(float val)
{
	m_modA = val;
}

void RigidBodyComponent::SetModB(float val)
{
	m_modB = val;
}

void RigidBodyComponent::SetModC(float val)
{
	m_modC = val;
}

Vector2D RigidBodyComponent::GetVelocity() const
{
	return m_vel;
}

Vector2D RigidBodyComponent::GetAcceleration() const
{
	return m_accel;
}

Force& RigidBodyComponent::GetForce()
{
	return m_force;
}

float RigidBodyComponent::GetMass() const
{
	return m_mass;
}

float RigidBodyComponent::GetGravity() const
{
	return m_gravity;
}

bool RigidBodyComponent::GetOnGround() const
{
	return m_onGround;
}

bool RigidBodyComponent::GetPushRight() const
{
	return m_pushRight;
}

bool RigidBodyComponent::GetPushLeft() const
{
	return m_pushLeft;
}

bool RigidBodyComponent::GetHitCeiling() const
{
	return m_hitCeiling;
}

bool RigidBodyComponent::GetIsMoving() const
{
	return m_isMoving;
}

bool RigidBodyComponent::GetIsStaticObj() const
{
	return m_isStaticObj;
}

bool RigidBodyComponent::GetHasGravity() const
{
	return m_hasGravity;
}

std::string RigidBodyComponent::GetObjType() const
{
	return m_objType;
}

float RigidBodyComponent::GetStartTime() const
{
	return m_startTime;
}

float RigidBodyComponent::GetDuration() const
{
	return m_duration;
}

float RigidBodyComponent::GetCounter() const
{
	return m_counter;
}

Vector2D RigidBodyComponent::GetDirection() const
{
	return m_direction;
}

float RigidBodyComponent::GetModA() const
{
	return m_modA;
}

float RigidBodyComponent::GetModB() const
{
	return m_modB;
}

float RigidBodyComponent::GetModC() const
{
	return m_modC;
}

void RigidBodyComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("RigidBodyComponent");
	auto map = metadata.reflect<TypeErasedObj>(*this);
	for (auto elem : map)
	{
		AutoWrite(writer, elem.first, elem.second.GetValue());
	}

}
void RigidBodyComponent::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<Force>("Force",
		sol::constructors<Force(), Force(Vector2D, float, float, bool)>(),
		"AddForce", &Force::AddForce,
		"RemoveForce", &Force::RemoveForce,
		"ActivateForce", &Force::ActivateForce,
		"SetLifeTime", &Force::SetLifetime,
		"SetResultantForce", &Force::SetResultForce,
		"ValidateAge", &Force::ValidateAge,
		"GetResultantForce", &Force::GetResultForce,
		"GetIsActive", &Force::GetIsActive
		);


	luaState->new_usertype<RigidBodyComponent>("RigidBodyComponent",
		// Member functions
		"SetVel", &RigidBodyComponent::SetVelocity,
		"SetAccel", &RigidBodyComponent::SetAcceleration,
		"SetDir", &RigidBodyComponent::SetDirection,
		"SetForce", &RigidBodyComponent::SetForce,
		"SetMass", &RigidBodyComponent::SetMass,
		"SetGravity", &RigidBodyComponent::SetGravity,
		"SetOnGround", &RigidBodyComponent::SetOnGround,
		"SetPushRight", &RigidBodyComponent::SetPushRight,
		"SetPushLeft", &RigidBodyComponent::SetPushLeft,
		"SetHitCeiling", &RigidBodyComponent::SetHitCeiling,
		"SetIsMoving", &RigidBodyComponent::SetIsMoving,
		"SetHasGravity",&RigidBodyComponent::SetHasGravity,
		"SetObjType", &RigidBodyComponent::SetObjType,
		"SetStartTime", &RigidBodyComponent::SetStartTime,
		"SetDuration", &RigidBodyComponent::SetDuration,
		"SetCounter", &RigidBodyComponent::SetCounter,
		"SetModA", &RigidBodyComponent::SetModA,
		"SetModB", &RigidBodyComponent::SetModB,
		"SetModC", &RigidBodyComponent::SetModC,

		"GetVel", &RigidBodyComponent::GetVelocity,
		"GetAccel", &RigidBodyComponent::GetAcceleration,
		"GetDir", &RigidBodyComponent::GetDirection,
		"GetForce", &RigidBodyComponent::GetForce,
		"GetMass", &RigidBodyComponent::GetMass,
		"GetGravity", &RigidBodyComponent::GetGravity,
		"GetOnGround", &RigidBodyComponent::GetOnGround,
		"GetPushRight", &RigidBodyComponent::GetPushRight,
		"GetPushLeft", &RigidBodyComponent::GetPushLeft,
		"GetHitCeiling", &RigidBodyComponent::GetHitCeiling,
		"GetIsMoving", &RigidBodyComponent::GetIsMoving,
		"GetIsStaticObject", &RigidBodyComponent::GetIsStaticObj,
		"GetHasGravity", &RigidBodyComponent::GetHasGravity,
		"GetObjType", &RigidBodyComponent::GetObjType,
		"GetStartTime", &RigidBodyComponent::GetStartTime,
		"GetDuration", &RigidBodyComponent::GetDuration,
		"GetCounter", &RigidBodyComponent::GetCounter,
		"GetModA", &RigidBodyComponent::GetModA,
		"GetModB", &RigidBodyComponent::GetModB,
		"GetModC", &RigidBodyComponent::GetModC,

		// Variables
		sol::base_classes, sol::bases<Component>()
		);
}

bool CompareData(const RigidBodyComponent & lhs, const RigidBodyComponent & rhs)
{
	if (lhs.GetGravity() != rhs.GetGravity())
		return false;
	if (lhs.GetHitCeiling() != rhs.GetHitCeiling())
		return false;
	if (lhs.GetIsStaticObj() != rhs.GetIsStaticObj())
		return false;
	if (lhs.GetMass() != rhs.GetMass())
		return false;
	if (lhs.GetHasGravity() != rhs.GetHasGravity())
		return false;

	return true;
}