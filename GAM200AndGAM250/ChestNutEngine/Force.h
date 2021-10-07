/******************************************************************************/
/*!
\file   Force.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Force.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Vector2D.h"

class Force 
{
public:
	Force(Vector2D force = Vector2D{ 0.0f, 0.0f }, float lifetime = 1.0f, float age = 0.0f, bool isActive = false);
	~Force();

	void AddForce(Force f);
	void RemoveForce();

	void ActivateForce(bool);
	void SetLifetime(float lifetime);
	void SetResultForce(Vector2D force);
	void ValidateAge(float dt);

	Vector2D GetResultForce() const;
	bool GetIsActive() const;
private:
	Vector2D m_resultForce;
	float m_lifetime; // how many frames it will go through to die
	float m_age;    // how many frames it has went through alredy
	bool m_isActive;
};