/******************************************************************************/
/*!
\file   Force.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of force.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Force.h"


Force::Force(Vector2D force , float lifetime, float age, bool isActive)
	:m_resultForce(force),
	m_lifetime(lifetime),
	m_age(age),
	m_isActive(isActive)
{
}

Force::~Force()
{ }

void Force::AddForce(Force f)
{
	m_resultForce = m_resultForce + f.m_resultForce;
	m_lifetime = f.m_lifetime;
	m_age = f.m_age;
}

void Force::RemoveForce()
{
	m_resultForce = { 0.0f,0.0f };
}

void Force::ActivateForce(bool val)
{
	m_isActive = val;
}

void Force::SetLifetime(float lifetime)
{ 
	m_lifetime = lifetime;
}	

void Force::SetResultForce(Vector2D force)
{
	m_resultForce = force;
}

void Force::ValidateAge(float dt)
{
	if(m_isActive && m_age < m_lifetime)
		m_age+=dt;
	if (m_age >= m_lifetime)
		RemoveForce();
}

Vector2D Force::GetResultForce() const
{
	return m_resultForce;
}

bool Force::GetIsActive() const
{
	return m_isActive;
}

