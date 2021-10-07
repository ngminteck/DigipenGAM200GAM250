#include "AISystem.h"

AISystem::AISystem()
{
}

AISystem::~AISystem()
{
}

void AISystem::MessageSend(Event * message) const
{
	message->PrintMessage("AI system");
	std::cout << "AI system received" << std::endl;
}

void AISystem::Update(std::unordered_map<std::string, GameObject *> & obj)
{
	std::unordered_map<ComponentType, Component *> map;
	RigidBodyComponent * spearValk_rbComp = nullptr;
	RigidBodyComponent * player_rbComp = nullptr;
	MovementComponent* moveComp = nullptr;
	ControlComponent* ctlComp = nullptr;
	for (auto itor = obj.begin(); itor != obj.end(); ++itor)
	{
		map = itor->second->GetComponents();
		if (itor->second->GetType() == PLAYER)
			player_rbComp = dynamic_cast<RigidBodyComponent*>(map[ComponentType::RigidBodyComponent]);
		else if(itor->second->GetType() == SPEARVALKYRIE)
			spearValk_rbComp = dynamic_cast<RigidBodyComponent*>(map[ComponentType::RigidBodyComponent]);
		moveComp = dynamic_cast<MovementComponent*>(map[ComponentType::MovementComponent]);
		ctlComp = dynamic_cast<ControlComponent*>(map[ComponentType::ControlComponent]);
		if (itor->second->GetType() == SPEARVALKYRIE && spearValk_rbComp && player_rbComp && moveComp)
			UpdateAIValkyrie(spearValk_rbComp, player_rbComp, moveComp);
	}
}

void AISystem::UpdateAIValkyrie(RigidBodyComponent* valkRB, RigidBodyComponent* playerRB, MovementComponent* moveComp)
{
	Vector2D vel;
	std::cout << "Valkryrie State: " << moveComp->GetState() << std::endl;
	switch (moveComp->GetState())
	{
	case IDLE:
		if (moveComp->GetPrevState() == ATTACK)
		{
			std::cout << "TIRED" << std::endl;
			moveComp->SetTimeCount(moveComp->GetTimeCount() + 1.0f / 60.0f);
			if (moveComp->GetTimeCount() >= 2.0f) // >= 1sec
			{
				moveComp->SetTimeCount(0.0f);
				moveComp->SetPrevState(IDLE);
			}
		}
		else if(AIScript::DetectWithinAxis('x', 1000.0f, valkRB, playerRB))
		{
			//std::cout << valkRB->GetPosition().x - playerRB->GetPosition().x << std::endl;
			std::cout << "TARGET FOUND" << std::endl;
			moveComp->SetState(WALK);
		}
		break;
	case WALK:
		AIScript::MoveTowards(valkRB, playerRB, 250.0f);
		if (AIScript::DetectWithinAxis('x', 50.0f, valkRB, playerRB))
		{
			std::cout << "CHARGE" << std::endl;
			moveComp->SetState(ATTACK);
		}
		break;
	case ATTACK:
		std::cout << "ATTACKING" << std::endl;
		moveComp->SetTimeCount(moveComp->GetTimeCount() + 1.0f / 60.0f);
		if (moveComp->GetTimeCount() >= 1.0f) // >= 1sec
		{
			moveComp->SetTimeCount(0.0f);
			moveComp->SetState(IDLE);
			moveComp->SetPrevState(ATTACK);
		}
		break;
	case FLINCH:
		std::cout << "OUCH" << std::endl;
		moveComp->SetTimeCount(moveComp->GetTimeCount() + 1.0f / 60.0f);
		if (moveComp->GetTimeCount() >= 1.0f) // >= 1sec
		{
			moveComp->SetTimeCount(0.0f);
			moveComp->SetState(IDLE);
		}
		break;
	}
}

