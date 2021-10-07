/******************************************************************************/
/*!
\file   RigidBodyComponent.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of RigidBodyComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "Vector2D.h"
#include "Matrix3x3.h"
#include "Force.h"
#include "GameObjectType.h"

class RigidBodyComponent : public Component
{

private:
	Vector2D m_vel;
	Vector2D m_accel;
	Vector2D m_direction;
	Force m_force;
	float m_mass;
	float m_gravity;
	bool m_onGround = false;
	bool m_pushRight = false;
	bool m_pushLeft = false;
	bool m_hitCeiling = false;
	bool m_isMoving = false;
	std::string m_objType;
	bool m_isStaticObj;
	bool m_hasGravity;

	float m_startTime;
	float m_duration;
	float m_counter = 0.0f;

	float m_modA;
	float m_modB;
	float m_modC;

public:
	RigidBodyComponent();
	RigidBodyComponent(const RigidBodyComponent& rbComp);
	RigidBodyComponent(Vector2D vel, Vector2D accel, Vector2D dir, float mass, float grav, bool isStatic, float startTime, float duration, float modA, float modB, float modC, std::string objType = "DEFAULT", bool hasGravity = false);
	~RigidBodyComponent();
	Component * Clone() const override;

	void SetVelocity(Vector2D vel);
	void SetAcceleration(Vector2D accel);
	void SetForce(Force f);
	void SetMass(float mass);
	void SetGravity(float grav);
	void SetOnGround(bool val);
	void SetPushRight(bool val);
	void SetPushLeft(bool val);
	void SetHitCeiling(bool val);
	void SetIsMoving(bool val);
	void SetHasGravity(bool val);
	void SetObjType(std::string str);
	void SetStartTime(float val);
	void SetDuration(float val);
	void SetCounter(float val);
	void SetDirection(Vector2D val);
	void SetModA(float val);
	void SetModB(float val);
	void SetModC(float val);

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	Vector2D GetVelocity() const;
	Vector2D GetAcceleration() const;
	Force& GetForce();
	float GetMass() const;
	float GetGravity() const;
	bool GetOnGround() const;
	bool GetPushRight() const;
	bool GetPushLeft() const;
	bool GetHitCeiling() const;
	bool GetIsMoving() const;
	bool GetIsStaticObj() const;
	bool GetHasGravity() const;
	std::string GetObjType() const;
	float GetStartTime() const;
	float GetDuration() const;
	float GetCounter() const;
	Vector2D GetDirection() const;
	float GetModA() const;
	float GetModB() const;
	float GetModC() const;

	constexpr static Metadata <RigidBodyComponent, Vector2D RigidBodyComponent::*, Vector2D RigidBodyComponent::*,
									float RigidBodyComponent::*, float RigidBodyComponent::*, bool RigidBodyComponent::*, bool RigidBodyComponent::*,
									bool RigidBodyComponent::*, bool RigidBodyComponent::*, bool RigidBodyComponent::*, bool RigidBodyComponent::*, bool RigidBodyComponent::*,
								    float RigidBodyComponent::*, float RigidBodyComponent::*, float RigidBodyComponent::*, std::string RigidBodyComponent::*, 
									Vector2D RigidBodyComponent::*, float RigidBodyComponent::*, float RigidBodyComponent::*, float RigidBodyComponent::*> metadata
	{ "RigidBodyComponent", std::make_tuple(&RigidBodyComponent::m_vel, &RigidBodyComponent::m_accel, 
											&RigidBodyComponent::m_mass, &RigidBodyComponent::m_gravity, &RigidBodyComponent::m_onGround, 
											&RigidBodyComponent::m_pushRight, &RigidBodyComponent::m_pushLeft, &RigidBodyComponent::m_hitCeiling, 
											&RigidBodyComponent::m_isMoving, &RigidBodyComponent::m_isStaticObj, &RigidBodyComponent::m_hasGravity, 
											&RigidBodyComponent::m_startTime, &RigidBodyComponent::m_duration, &RigidBodyComponent::m_counter, &RigidBodyComponent::m_objType,
											&RigidBodyComponent::m_direction, &RigidBodyComponent::m_modA, &RigidBodyComponent::m_modB, &RigidBodyComponent::m_modC),
							std::make_tuple("m_vel", "m_accel", "m_mass", "m_gravity", "m_onGround", 
											"m_pushRight", "m_pushLeft", "m_hitCeiling", "m_isMoving", "m_isStaticObj", "m_hasGravity", "m_startTime",
											"m_duration", "m_counter", "m_objType", "m_direction", "m_modA", "m_modB", "m_modC")
	};
	static void AddToScript(sol::state * luaState);
};

bool CompareData(const RigidBodyComponent & lhs, const RigidBodyComponent & rhs);