#pragma once
#include "SceneActor.h"
#include "SpriteComponent.h"
#include "RestartLevel.h"
#include "EventHandler.h"
struct TransitionSystem
{
	TransitionSystem(bool &, EventHandler *);
	~TransitionSystem();
	void Initialize(std::map<unsigned, SceneActor *>& );
	void Restart(RestartLevel *);
	bool & m_transition;
	SceneActor * m_ftb; //Fade to black.
	SpriteComponent * m_ftb_sprite;
	EventHandler * m_ev;
};