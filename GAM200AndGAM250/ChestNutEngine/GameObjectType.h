/******************************************************************************/
/*!
\file   GameObjectType.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the type of objects.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once


enum ObjectType : int
{
	DEFAULT = 0,
	PLAYER = 1,
	ENEMY = 2,
	PLATFORM = 3,
	BACKGROUND = 4,
	CAMERA = 5,
	CONTROLLER = 6,
	LIGHT = 7,
	PARTICLE = 8,
	RAGDOLL = 9,
	OTHER = 100
};