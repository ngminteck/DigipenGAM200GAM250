#include "CheckpointSystem.h"

CheckpointSystem::CheckpointSystem(EventHandler * ev, SceneActorFactory * fac)
	:m_ev{ev}, m_fac{fac}
{
	m_ev->SubscribeEvent(this, &CheckpointSystem::Reinitialize);
	m_ev->SubscribeEvent(this, &CheckpointSystem::RespawnPlayer);
}

CheckpointSystem::~CheckpointSystem()
{
}

void CheckpointSystem::Update()
{
	if (m_col)
	{
		if (m_col->GetCollidingvsEndGameBox())
		{
			std::cout << "ACTIVATE" << std::endl;
			m_ev->PublishEvent(new LoadGameState{ m_cpt->m_path });
		}
	}

	if (m_player_c)
	{
		if (m_player_c->GetCollidingvsRespawn())
		{
			std::cout << "Checkpoint updated.";
			m_checkpoint = m_player_t->GetPosition();
			auto vec_obj = m_player_c->GetVecOfPtrToCollidedObject();
			for (auto& e : vec_obj)
			{
				if (e->GetPrefabActor()->GetComponent<RigidBodyComponent>()->GetObjType() == "RESPAWNBOX" && e->GetPrefabActor()->GetComponent<SpriteComponent>()->GetSpriteName() == "Existence")
					e->SetOnlyActive(false);
			}
		}
	}
}

void CheckpointSystem::Initialize()
{
	for (auto & elem : m_fac->GetSceneActorContainer())
	{
		auto comps = elem.second->GetPrefabActor()->GetComponents();
		if (elem.second->GetSceneActorName() == "Surtur")
		{
			if (comps.find(ComponentType::CollisionComponent) != comps.end()
				&& comps.find(ComponentType::TransformComponent) != comps.end()
				&& comps.find(ComponentType::StatsComponent) != comps.end()
					&& comps.find(ComponentType::MovementComponent) != comps.end())
			{
				m_player_t = dynamic_cast<TransformComponent *>(comps[ComponentType::TransformComponent]);
				m_player_c = dynamic_cast<CollisionComponent *>(comps[ComponentType::CollisionComponent]);
				m_player_s = dynamic_cast<StatsComponent *>(comps[ComponentType::StatsComponent]);
				m_player_m = dynamic_cast<MovementComponent *>(comps[ComponentType::MovementComponent]);
				m_player_sp = dynamic_cast<SpriteComponent *>(comps[ComponentType::SpriteComponent]);
				m_checkpoint = m_player_t->GetPosition();
				std::cout << "CheckpointSystem.cpp: Surtur initialized. " << std::endl;
			}

			m_damage = elem.second->GetChildByName("PlayerDamageFX");

			if(m_damage)
				std::cout << "CheckpointSystem.cpp: Surtur's damage fx initialized. " << std::endl;


		}

		
		if(comps.find(ComponentType::CollisionComponent) != comps.end()
			&& comps.find(ComponentType::CheckpointComponent) != comps.end())
		{
			m_col = dynamic_cast<CollisionComponent *>(comps[ComponentType::CollisionComponent]);
			m_cpt = dynamic_cast<CheckpointComponent *>(comps[ComponentType::CheckpointComponent]);
			break;
		}
	}
}


void CheckpointSystem::Reinitialize(LoadLevel *)
{
	m_col = nullptr;
	m_cpt = nullptr;
	m_player_t = nullptr;
	m_player_c = nullptr;
	m_player_s = nullptr;
	m_player_m = nullptr;
	Initialize();
}

void CheckpointSystem::RespawnPlayer(RestartLevel *)
{
	//Set the player hp to max
	if (m_player_t)
	{
		m_player_m->SetStateLogic("IDLE");
		m_player_sp->SetSprite("RIGHT_IDLE");
		m_player_s->SetHP(5);
		m_player_t->SetPosition(m_checkpoint);
		m_damage->SetActive(false);
		std::cout << "respawn player";
	}
}


