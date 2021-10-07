#pragma once
#include "Command.h"
#include "SceneActorFactory.h"
#include "EventHandler.h"
#include "SwitchLevelEvent.h"
#include <unordered_map>

struct DeleteActor : Command
{
	DeleteActor(SceneActorFactory * fac, unsigned id, SceneActor * obj, EventHandler * ev, Camera * cam)
		:m_fac{ fac }, m_id{ id }, m_obj{ new SceneActor{*obj,false} }, m_ev{ ev }, m_cam{ cam } {}

	~DeleteActor()
	{
		delete m_obj;
	}

	void ExecuteRedo() override
	{
		if (m_cam->GetGameObject() == m_fac->GetSceneActorContainer().at(m_id))
			m_cam->UnlatchObject();
		std::cout << "Size is first: " <<  m_fac->GetSceneActorContainer().size() << std::endl;
		m_fac->DeleteSceneActor(m_id);

		std::cout << "Size is now: " << m_fac->GetSceneActorContainer().size() << std::endl;
	} 

	void ExecuteUndo() override
	{
		m_fac->AddSceneActor(m_id, m_obj);

	}

	unsigned m_id;
	SceneActor * m_obj;
	SceneActorFactory * m_fac;
	EventHandler * m_ev;
	Camera * m_cam;
};