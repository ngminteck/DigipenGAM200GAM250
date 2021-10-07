#pragma once
#include "System.h"
#include "EventHandler.h"
#include "CollisionComponent.h"
#include "LoadLevel.h"
#include "LoadGameState.h"
#include "SceneActorFactory.h"
#include "RestartLevel.h"
#include "SpriteComponent.h"
class CheckpointSystem
{
public:
	CheckpointSystem(EventHandler *, SceneActorFactory *);
	~CheckpointSystem();

	void Update();
	void Initialize();
	void Reinitialize(LoadLevel *);

	void RespawnPlayer(RestartLevel *);

	Vector2D m_checkpoint;


private:
	EventHandler * m_ev;
	SceneActorFactory * m_fac;

	CollisionComponent * m_col;
	CheckpointComponent * m_cpt;

	TransformComponent * m_player_t;
	CollisionComponent * m_player_c;
	StatsComponent * m_player_s;
	MovementComponent * m_player_m;
	SpriteComponent * m_player_sp;

	SceneActor * m_damage;

};