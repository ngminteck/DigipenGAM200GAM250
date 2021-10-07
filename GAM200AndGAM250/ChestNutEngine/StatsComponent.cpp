/******************************************************************************/
/*!
\file   HPComponent.cpp

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the function for the HPComponent class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "StatsComponent.h"
#include <iostream>

#include <typeinfo>
StatsComponent::StatsComponent()
	:Component{ ComponentType::StatsComponent }
{ }

StatsComponent::StatsComponent(float hp, float mp, std::string charac, bool isDead, bool invul, float invulTimer)
	: Component( ComponentType::StatsComponent ),
	m_HP(hp),
	m_MP(mp),
	m_char(charac),
	m_isDead(isDead),
	m_invul(invul),
	m_invulTimer(invulTimer)
{ }

StatsComponent::~StatsComponent()
{ }

Component * StatsComponent::Clone() const
{
	return new StatsComponent(*this);
}

void StatsComponent::SetHP(float val)
{
	m_HP = val;
	if (m_HP < 0.0f)
	{
		m_HP = 0.0f;
		m_isDead = true;
	}
}

void StatsComponent::SetMP(float val)
{
	m_MP = val;
}

void StatsComponent::SetChar(std::string str)
{
	m_char = str;
}

void StatsComponent::SetDead(bool val)
{
	m_isDead = val;
}

void StatsComponent::SetInvul(bool val)
{
	m_invul = val;
}

void StatsComponent::SetInvulTimer(float val)
{
	m_invulTimer = val;
}

void StatsComponent::SetRespawnPos(Vector2D pos)
{
	m_respawnPos = pos;
}

float StatsComponent::GetHP() const
{
	return m_HP;
}

float StatsComponent::GetMP() const
{
	return m_MP;
}

std::string StatsComponent::GetChar() const
{
	return m_char;
}

bool StatsComponent::GetDead() const
{
	return m_isDead;
}

bool StatsComponent::GetInvul() const
{
	return m_invul;
}

float StatsComponent::GetInvulTimer() const
{
	return m_invulTimer;
}

Vector2D StatsComponent::GetRespawnPos() const
{
	return m_respawnPos;
}

void StatsComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("StatsComponent");
	auto map = metadata.reflect<TypeErasedObj>(*this);
	for (auto elem : map)
	{
		AutoWrite(writer, elem.first, elem.second.GetValue());
	}
}

void StatsComponent::RegisterCooldown(const std::string name, const float& value, const float& timer, const bool & state)
{
	if (name == "" || timer < 0.0f)
	{
		std::cout << "Register cooldown fail, name or value is wrong." << std::endl;
	}
	else
		m_cooldownVec.push_back(std::make_tuple(name, value, timer, state));
}

void StatsComponent::UpdateCooldown(const float& value)
{
	for (auto& e : m_cooldownVec)
	{
		if (std::get<3>(e))
		{
			std::get<2>(e) -= value;
			if(std::get<2>(e) < 0)
				std::get<2>(e) = 0.0f;
		}
	}
}

void StatsComponent::ActivateCooldown(const std::string name, bool state)
{
	for (auto& e : m_cooldownVec)
	{
		if (std::get<0>(e) == name)
			std::get<3>(e) = state;
	}
}

void StatsComponent::ResetCooldown(const std::string name)
{
	for (auto& e : m_cooldownVec)
		if (std::get<0>(e) == name)
			std::get<2>(e) = std::get<1>(e);
}

void StatsComponent::SetCooldown(const std::string & name, float value)
{
	for (auto& e : m_cooldownVec)
		if (std::get<0>(e) == name)
			std::get<1>(e) = value;
}

float StatsComponent::GetCooldown(const std::string& name) const
{
	for (const auto& e : m_cooldownVec)
		if (std::get<0>(e) == name)
			return std::get<2>(e);
	return 0.0f;
}

bool StatsComponent::IsActive(const std::string & name) const
{
	for (const auto& e : m_cooldownVec)
		if (std::get<0>(e) == name)
			return std::get<3>(e);
	return false;
}

void StatsComponent::PrintAllCooldown() const
{
	for (const auto& e : m_cooldownVec)
		std::cout << std::get<0>(e) << ": " << std::get<2>(e) << std::endl;
}

void StatsComponent::AddToScript(sol::state* luaState)
{
	luaState->new_usertype<StatsComponent>("StatsComponent",
		"SetHP", &StatsComponent::SetHP,
		"SetMP", &StatsComponent::SetMP,
		"SetDead", &StatsComponent::SetDead,
		"GetHP", &StatsComponent::GetHP,
		"GetMP", &StatsComponent::GetMP,
		"GetDead", &StatsComponent::GetDead,
		"GetInvul", &StatsComponent::GetInvul,
		"GetInvulTimer", &StatsComponent::GetInvulTimer,
		"SetInvul", &StatsComponent::SetInvul,
		"SetInvulTimer", &StatsComponent::SetInvulTimer,
		"RegisterCooldown", &StatsComponent::RegisterCooldown,
		"ActivateCooldown", &StatsComponent::ActivateCooldown,
		"PrintAllCooldown", &StatsComponent::PrintAllCooldown,
		"GetCooldown", &StatsComponent::GetCooldown,
		"IsActive", &StatsComponent::IsActive,
		"SetCooldown", &StatsComponent::SetCooldown,
		"ResetCooldown", &StatsComponent::ResetCooldown,
		"GetRespawnPos", &StatsComponent::GetRespawnPos
		);
}

bool CompareData(const StatsComponent & lhs, const StatsComponent & rhs)
{
	UNREFERENCED_PARAMETER(lhs);
	UNREFERENCED_PARAMETER(rhs);
	return true;
}

