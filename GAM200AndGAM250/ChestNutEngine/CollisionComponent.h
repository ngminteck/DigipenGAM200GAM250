/******************************************************************************/
/*!
\file   CollisionComponent.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of CollisionComponent class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <tuple>
#include <cmath>

#include "Component.h"
#include "Metadata.h"
#include "TransformComponent.h"

class CollisionComponent : public Component
{
private:
	bool isCollidingRight;
	bool isCollidingBottom;
	bool isCollidingLeft;
	bool isCollidingTop;
	bool isColliding;
	bool isCollidingvsTornado;
	bool isCollidingvsCrumbling;
	bool isCollidingvsPit;
	bool isCollidingvsMoving;
	bool isCollidingvsEnemy;
	bool isCollidingvsSwitch;
	bool isCollidingvsCheckpoint;
	bool isCollidingvsRespawn;
	bool isCollidingvsTrigger;
	bool isCollidingvsPopup;
	bool isCollidingvsDIn;
	bool isCollidingvsDOut;
	bool isCollidingvsEndGameBox;
	bool isCollidingvsCollider;

	bool flag;
	bool flagTornado;
	bool flagCrumbling;
	bool flagPit;
	bool flagMoving;
	bool flagEnemy;
	bool flagSwitch;
	bool flagCheckpoint;
	bool flagRespawn;
	bool flagTrigger;
	bool flagPopup;
	bool flagDIn;
	bool flagDOut;
	bool flagEndGameBox;
	bool flagCollider;
	
	bool collidePlayerHitbox;
	bool collidePlayerDashHitbox;
	bool collideBigPunchHitbox;
	bool collideEnemyDashHitbox;
	bool collideEnemyCounterHitbox;
	bool collideEnemyHitbox;
	bool collideRespawnBox;
	bool collideTriggerBox;

	bool playerHitboxFlag;
	bool playerDashHitboxFlag;
	bool bigPunchHitboxFlag;
	bool enemyDashHitboxFlag;
	bool enemyCounterHitboxFlag;
	bool enemyHitboxFlag;

	std::vector<SceneActor*> vecOfPtrToCollidedObject;

public:
	CollisionComponent();
	CollisionComponent(const CollisionComponent& colComp);
	~CollisionComponent();
	Component * Clone() const override;

	void SetColliding(bool val);
	void SetCollidingRight(bool val);
	void SetCollidingBottom(bool val);
	void SetCollidingLeft(bool val);
	void SetCollidingTop(bool val);
	void SetCollidingvsTornado(bool val);
	void SetCollidingvsCrumbling(bool val);
	void SetCollidingvsPit(bool val);
	void SetCollidingvsMoving(bool val);
	void SetCollidingvsEnemy(bool val);
	void SetCollidingvsSwitch(bool val);
	void SetCollidingvsCheckpoint(bool val);
	void SetCollidingvsRespawn(bool val);
	void SetCollidingvsTrigger(bool val);
	void SetCollidingvsPopup(bool val);
	void SetCollidingvsDIn(bool val);
	void SetCollidingvsDOut(bool val);
	void SetCollidingvsEndGameBox(bool val);
	void SetCollidingvsCollider(bool val);

	void SetFlag(bool val);
	void SetFlagTornado(bool val);
	void SetFlagCrumbling(bool val);
	void SetFlagPit(bool val);
	void SetFlagMoving(bool val);
	void SetFlagEnemy(bool val);
	void SetFlagSwitch(bool val);
	void SetFlagCheckpoint(bool val);
	void SetFlagRespawn(bool val);
	void SetFlagTrigger(bool val);
	void SetFlagPopup(bool val);
	void SetFlagDIn(bool val);
	void SetFlagDOut(bool val);
	void SetFlagEndGameBox(bool val);
	void SetFlagCollider(bool val);
	
	void SetCollidePlayerHitbox(bool val);
	void SetCollidePlayerDashHitbox(bool val);
	void SetCollideBigPunchHitbox(bool val);
	void SetCollideEnemyDashHitbox(bool val);
	void SetCollideEnemyCounterHitbox(bool val);
	void SetCollideEnemyHitbox(bool val);
	void SetPlayerHitboxFlag(bool val);
	void SetPlayerDashHitboxFlag(bool val);
	void SetBigPunchHitboxFlag(bool val);
	void SetEnemyDashHitboxFlag(bool val);
	void SetEnemyCounterHitboxFlag(bool val);
	void SetEnemyHitboxFlag(bool val);

	void SetAll(bool val);
	void CheckFlagAndSet(bool val);

	bool GetCollidingRight() const;
	bool GetCollidingBottom() const;
	bool GetCollidingLeft() const;
	bool GetCollidingTop() const;
	bool GetIsColliding() const;
	bool GetCollidingvsTornado() const;
	bool GetCollidingvsCrumbling() const;
	bool GetCollidingvsPit() const;
	bool GetCollidingvsMoving() const;
	bool GetCollidingvsEnemy() const;
	bool GetCollidingvsSwitch() const;
	bool GetCollidingvsCheckpoint() const;
	bool GetCollidingvsRespawn() const;
	bool GetCollidingvsTrigger() const;
	bool GetCollidingvsPopup() const;
	bool GetCollidingvsDIn() const;
	bool GetCollidingvsDOut() const;
	bool GetCollidingvsEndGameBox() const;
	bool GetCollidingvsCollider() const;

	bool GetFlag() const;
	bool GetFlagTornado() const;
	bool GetFlagCrumbling() const;
	bool GetFlagPit() const;
	bool GetFlagMoving() const;
	bool GetFlagEnemy() const;
	bool GetFlagSwitch() const;
	bool GetFlagCheckpoint() const;
	bool GetFlagRespawn() const;
	bool GetFlagTrigger() const;
	bool GetFlagPopup() const;
	bool GetFlagDIn() const;
	bool GetFlagDOut() const;
	bool GetFlagEndGameBox() const;
	bool GetFlagCollider() const;
	
	bool GetCollidePlayerHitbox() const;
	bool GetCollidePlayerDashHitbox() const;
	bool GetCollideBigPunchHitbox() const;
	bool GetCollideEnemyDashHitbox() const;
	bool GetCollideEnemyCounterHitbox() const;
	bool GetCollideEnemyHitbox() const;
	bool GetPlayerHitboxFlag() const;
	bool GetPlayerDashHitboxFlag() const;
	bool GetBigPunchHitboxFlag() const;
	bool GetEnemyDashHitboxFlag() const;
	bool GetEnemyCounterHitboxFlag() const;
	bool GetEnemyHitboxFlag() const;

	std::vector<SceneActor*>& GetVecOfPtrToCollidedObject();
	
	void PushBackCollidedObject(SceneActor* obj);

	bool DetectInRadius(float dist, TransformComponent* transComp1, TransformComponent* transComp2);

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	constexpr static Metadata <CollisionComponent, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*, 
								bool CollisionComponent::*, bool CollisionComponent::*, 
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*, 
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*,
								bool CollisionComponent::*, bool CollisionComponent::*>
								metadata
	{ "CollisionComponent", std::make_tuple(&CollisionComponent::isCollidingRight, 
											&CollisionComponent::isCollidingBottom,
											&CollisionComponent::isCollidingLeft,
											&CollisionComponent::isCollidingTop,
											&CollisionComponent::isCollidingvsTornado,
											&CollisionComponent::isCollidingvsCrumbling,
											&CollisionComponent::isCollidingvsPit,
											&CollisionComponent::isCollidingvsMoving,
											&CollisionComponent::isCollidingvsEnemy,
											&CollisionComponent::isCollidingvsSwitch,
											&CollisionComponent::isCollidingvsCheckpoint,
											&CollisionComponent::isCollidingvsRespawn,
											&CollisionComponent::isCollidingvsTrigger,
											&CollisionComponent::isCollidingvsPopup,
											&CollisionComponent::isCollidingvsDIn,
											&CollisionComponent::isCollidingvsDOut,
											&CollisionComponent::isCollidingvsEndGameBox,
											&CollisionComponent::isCollidingvsCollider,

											&CollisionComponent::collidePlayerHitbox,
											&CollisionComponent::collidePlayerDashHitbox,
											&CollisionComponent::collideBigPunchHitbox,
											&CollisionComponent::collideEnemyDashHitbox,
											&CollisionComponent::collideEnemyCounterHitbox,
											&CollisionComponent::collideEnemyHitbox,

											&CollisionComponent::isColliding), 
							std::make_tuple("isCollidingRight",		
											"isCollidingBottom", 
											"isCollidingLeft", 
											"isCollidingTop",
											"isCollidingvsTornado",
											"isCollidingvsCrumbling",
											"isCollidingvsPit",
											"isCollidingvsMoving",
											"isCollidingvsEnemy",
											"isCollidingvsSwitch",
											"isCollidingvsCheckpoint",
											"isCollidingvsRespawn",
											"isCollidingvsTrigger",
											"isCollidingvsPopup",
											"isCollidingvsDIn",
											"isCollidingvsDOut",
											"isCollidingvsEndGameBox",
											"isCollidingvsCollider",

											"collidePlayerHitbox",
											"collidePlayerDashHitbox",
											"collideBigPunchHitbox",
											"collideEnemyDashHitbox",
											"collideEnemyCounterHitbox",
											"collideEnemyHitbox",

											"isColliding") 
	};

	static void AddToScript(sol::state * luaState);
};

bool CompareData(CollisionComponent * lhs, CollisionComponent * rhs);
