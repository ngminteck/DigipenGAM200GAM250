#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

bool Entity::GetActive()const
{
	return isActive;
}

void Entity::SetActive(bool value)
{
	isActive = value;
}

bool Entity::GetInvul() const
{
	return isInvul;
}

void Entity::SetInvul(bool value)
{
	isInvul = value;
}

float Entity::GetTimer() const
{
	return timer;
}

void Entity::SetTimer(float value)
{
	timer = value;
}

void Entity::IncTimer()
{
	timer += 2.0f;
}

void Entity::SetDead(bool value)
{
	isDead = value;
}

bool Entity::GetDead()
{
	return isDead;
}
