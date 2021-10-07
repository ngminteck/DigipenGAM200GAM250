/******************************************************************************/
/*!
\file   AnimationSystem.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains declaration for AnimationSystem class functions and members

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "System.h"
#include "SceneActor.h"
#include "PauseGame.h"

class AnimationSystem : public System
{
public:
	AnimationSystem(EventHandler *, bool &, bool &);
	~AnimationSystem();

	void Update(std::vector<GraphicsContainer>* obj, bool reverse = false);
	std::string GetAnimationKey(MovementComponent*);
	Profiler profiler;
private:
	EventHandler * m_ev; //Pointer to the game engine's event handler.
	bool & pause;
	bool & dialogue;
};