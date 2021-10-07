#include "TransitionSystem.h"

TransitionSystem::TransitionSystem(bool & transition, EventHandler * ev)
	:m_transition{transition}, m_ev{ev}
{
	m_ev->SubscribeEvent(this, &TransitionSystem::Restart);
}

TransitionSystem::~TransitionSystem()
{
	
}

void TransitionSystem::Initialize(std::map<unsigned, SceneActor *> & fac)
{
	m_ftb = nullptr;
	m_ftb_sprite = nullptr;

	for (auto & obj : fac)
	{
		if (obj.second->GetSceneActorName() == "FadeToBlack")
		{
			Component * comp = obj.second->GetComponent(ComponentType::SpriteComponent);
			if (comp != nullptr)
			{
				m_ftb = obj.second;
				m_ftb_sprite = dynamic_cast<SpriteComponent *>(comp);
				m_ftb->SetOnlyActive(false);
				std::cout << "Fade-to-black initialized." << std::endl;
			}
		}
	}
}

void TransitionSystem::Restart(RestartLevel *)
{
	if (m_ftb)
	{
		m_ftb->SetOnlyActive(false);
		SpriteComponent * sprite = dynamic_cast<SpriteComponent *>(m_ftb->GetComponent(ComponentType::SpriteComponent));

		if (sprite)
			sprite->SetFrameCount(0);

		std::cout << "Fade to black off." << std::endl;
	}
}

