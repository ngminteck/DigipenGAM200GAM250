/******************************************************************************/
/*!
\file   Random.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains random function

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Random.h"


int RandInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

float RandFloat(float min, float max)
{
	return rand() * (max - min) / RAND_MAX + min;
}