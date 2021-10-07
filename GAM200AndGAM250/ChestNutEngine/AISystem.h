#pragma once
#include "System.h"
#include "GameObject.h"
#include "AIScript.h"

class AISystem : System
{
public:
	AISystem();
	~AISystem();
	void MessageSend(Event *) const;
	void Update(std::unordered_map<std::string , GameObject *> &);
	void UpdateAIValkyrie(RigidBodyComponent* valkRB, RigidBodyComponent* playerRB, MovementComponent* moveComp);

private:
	std::unordered_map<ComponentType, Component*> components;
};