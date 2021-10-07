#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(EventHandler * ev, bool & p, bool & d)
	:m_ev{ ev }, profiler{"Animation System"}, pause{p}, dialogue{d}
{

}

AnimationSystem::~AnimationSystem()
{

}

void AnimationSystem::Update(std::vector<GraphicsContainer>* obj, bool order)
{
	profiler.UpdateStart();
	if (!pause || dialogue)
	{
		std::unordered_map<ComponentType, Component*> * comp;
		SpriteComponent  * sprite;
		MovementComponent* movement;
		for (auto& game_obj : *obj)
		{
			if (game_obj.scene_actor->GetActive())
			{
				sprite = nullptr;
				movement = nullptr;
				comp = game_obj.prefab_actor->GetComponentsPtr();

				if ((*comp).find(ComponentType::SpriteComponent) != (*comp).end())
					sprite = dynamic_cast<SpriteComponent*>((*comp)[ComponentType::SpriteComponent]);
				if ((*comp).find(ComponentType::MovementComponent) != (*comp).end())
					movement = dynamic_cast<MovementComponent*>((*comp)[ComponentType::MovementComponent]);

				if (!sprite || !(*sprite->GetAnimatedTexturePtr()))
					continue;

				if (movement&& sprite->GetTexture())
				{
					if (movement->GetPrevStateLogic() != movement->GetStateLogic() ||
						movement->GetPrevDirection() != movement->GetDirection())
					{
						movement->SetPrevStateLogic(movement->GetStateLogic());
						movement->SetPrevDirection(movement->GetDirection());
						sprite->SetTimeCount(0);
						sprite->SetFrameCount(0);
						//std::string dirState = GetAnimationKey(movement);
						//sprite->SetSprite(dirState);
						//sprite->SetMesh(dirState);
					}
				}
				if (*sprite->GetAnimatedTexturePtr())
				{
					if (order == false)
					{
						sprite->SetTimeCount(sprite->GetTimeCount() + 0.016666f);
						if (sprite->GetTimeCount() > sprite->GetAnimation()->GetFrameInterval(sprite->GetFrameCount()))
						{
							sprite->SetTimeCount(sprite->GetTimeCount() - sprite->GetAnimation()->GetFrameInterval(sprite->GetFrameCount()));
							sprite->SetFrameCount(sprite->GetFrameCount() + 1);
							if (sprite->GetAnimation()->GetNumFrames() <= sprite->GetFrameCount())
								sprite->SetFrameCount(0);
						}
					}
					else
					{
						sprite->SetTimeCount(sprite->GetTimeCount() + 0.016666f);
						if (sprite->GetTimeCount() > sprite->GetAnimation()->GetFrameInterval(sprite->GetFrameCount()))
						{
							sprite->SetTimeCount(sprite->GetTimeCount() - sprite->GetAnimation()->GetFrameInterval(sprite->GetFrameCount()));
							sprite->SetFrameCount(sprite->GetFrameCount() - 1);
							if (sprite->GetFrameCount() < 0)
								sprite->SetFrameCount(sprite->GetAnimation()->GetNumFrames());
						}
					}
				}
			}
		}
	}
	profiler.UpdateEnd();
}

std::string AnimationSystem::GetAnimationKey(MovementComponent* movecomp)
{
	std::string retVal;
	switch (movecomp->GetDirection())
	{
	case LEFT:
		retVal = "LEFT_";
		break;
	case RIGHT:
		retVal = "RIGHT_";
		break;
	}
	retVal += movecomp->GetStateLogic();
	return retVal;
}
