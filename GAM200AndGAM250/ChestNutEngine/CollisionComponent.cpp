/******************************************************************************/
/*!
\file   CollisionComponent.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of CollisionComponent

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "CollisionComponent.h"
#include "SceneActor.h"

CollisionComponent::CollisionComponent()
	:Component{ ComponentType::CollisionComponent },
	isColliding{false},
	isCollidingRight{ false },
	isCollidingBottom{false}, 
	isCollidingLeft{false},
	isCollidingTop{false},
	isCollidingvsTornado{ false }, 
	isCollidingvsCrumbling{ false },
	isCollidingvsPit{ false }, 
	isCollidingvsMoving{ false },
	isCollidingvsSwitch{ false },
	isCollidingvsCheckpoint{ false },
	isCollidingvsRespawn{ false },
	flag{ false },
	flagTornado{ false }, 
	flagPit{ false },
	flagMoving{ false },
	flagSwitch{ false },
	flagCheckpoint{ false },
	flagRespawn{ false },
	collidePlayerHitbox{ false },
	collidePlayerDashHitbox{ false },
	collideBigPunchHitbox{ false },
	collideEnemyDashHitbox{ false },
	collideEnemyCounterHitbox{ false },
	collideEnemyHitbox{ false },
	playerHitboxFlag{ false },
	playerDashHitboxFlag{ false },
	bigPunchHitboxFlag{ false },
	enemyDashHitboxFlag{ false },
	enemyCounterHitboxFlag{ false },
	enemyHitboxFlag{ false }
{

}

CollisionComponent::CollisionComponent(const CollisionComponent & colComp)
	: Component(colComp),
	isCollidingRight(colComp.isCollidingRight),
	isCollidingBottom(colComp.isCollidingBottom),
	isCollidingLeft(colComp.isCollidingLeft),
	isCollidingTop(colComp.isCollidingTop),
	isColliding(colComp.isColliding),
	isCollidingvsTornado(colComp.isCollidingvsTornado),
	isCollidingvsCrumbling(colComp.isCollidingvsCrumbling),
	isCollidingvsPit(colComp.isCollidingvsPit),
	isCollidingvsMoving(colComp.isCollidingvsMoving),
	isCollidingvsSwitch(colComp.isCollidingvsSwitch),
	isCollidingvsCheckpoint(colComp.isCollidingvsCheckpoint),
	isCollidingvsRespawn(colComp.isCollidingvsRespawn),
	isCollidingvsTrigger(colComp.isCollidingvsTrigger),

	flag(colComp.flag),
	flagTornado(colComp.flagTornado),
	flagPit(colComp.flagPit),
	flagMoving(colComp.flagMoving),
	flagSwitch(colComp.flagSwitch),
	flagCheckpoint(colComp.flagCheckpoint),
	flagRespawn(colComp.flagRespawn),
	flagTrigger(colComp.flagTrigger),

	collidePlayerHitbox(colComp.collidePlayerHitbox),
	collidePlayerDashHitbox(colComp.collidePlayerDashHitbox),
	collideBigPunchHitbox(colComp.collideBigPunchHitbox),
	collideEnemyDashHitbox(colComp.collideEnemyDashHitbox),
	collideEnemyCounterHitbox(colComp.collideEnemyCounterHitbox),
	collideEnemyHitbox(colComp.collideEnemyHitbox),
	playerHitboxFlag(colComp.playerHitboxFlag),
	playerDashHitboxFlag(colComp.playerDashHitboxFlag),
	bigPunchHitboxFlag(colComp.bigPunchHitboxFlag),
	enemyDashHitboxFlag(colComp.enemyDashHitboxFlag),
	enemyCounterHitboxFlag(colComp.enemyCounterHitboxFlag),
	enemyHitboxFlag(colComp.enemyHitboxFlag)
{
	
}

CollisionComponent::~CollisionComponent()
{
}

Component * CollisionComponent::Clone() const
{
	return new CollisionComponent(*this);
}

bool CollisionComponent::GetCollidingRight() const
{
	return isCollidingRight;
}

bool CollisionComponent::GetCollidingBottom() const
{
	return isCollidingBottom;
}

bool CollisionComponent::GetCollidingLeft() const
{
	return isCollidingLeft;
}

bool CollisionComponent::GetCollidingTop() const
{
	return isCollidingTop;
}

bool CollisionComponent::GetIsColliding() const
{
	return isColliding;
}

bool CollisionComponent::GetCollidingvsTornado() const
{
	return isCollidingvsTornado;
}

bool CollisionComponent::GetCollidingvsCrumbling() const
{
	return isCollidingvsCrumbling;
}

bool CollisionComponent::GetCollidingvsPit() const
{
	return isCollidingvsPit;
}

bool CollisionComponent::GetCollidingvsMoving() const
{
	return isCollidingvsMoving;
}

bool CollisionComponent::GetCollidingvsEnemy() const
{
	return isCollidingvsEnemy;
}

bool CollisionComponent::GetCollidingvsSwitch() const
{
	return isCollidingvsSwitch;
}

bool CollisionComponent::GetCollidingvsCheckpoint() const
{
	return isCollidingvsCheckpoint;
}

bool CollisionComponent::GetCollidingvsRespawn() const
{
	return isCollidingvsRespawn;
}

bool CollisionComponent::GetCollidingvsTrigger() const
{
	return isCollidingvsTrigger;
}

bool CollisionComponent::GetCollidingvsPopup() const
{
	return isCollidingvsPopup;
}

bool CollisionComponent::GetCollidingvsDIn() const
{
	return isCollidingvsDIn;
}

bool CollisionComponent::GetCollidingvsDOut() const
{
	return isCollidingvsDOut;
}

bool CollisionComponent::GetCollidingvsEndGameBox() const
{
	return isCollidingvsEndGameBox;
}

bool CollisionComponent::GetCollidingvsCollider() const
{
	return isCollidingvsCollider;
}

bool CollisionComponent::GetFlag() const
{
	return flag;
}

bool CollisionComponent::GetFlagTornado() const
{
	return flagTornado;
}

bool CollisionComponent::GetFlagCrumbling() const
{
	return flagCrumbling;
}

bool CollisionComponent::GetFlagPit() const
{
	return flagPit;
}

bool CollisionComponent::GetFlagMoving() const
{
	return flagMoving;
}

bool CollisionComponent::GetFlagEnemy() const
{
	return flagEnemy;
}

bool CollisionComponent::GetFlagSwitch() const
{
	return flagSwitch;
}

bool CollisionComponent::GetFlagCheckpoint() const
{
	return flagCheckpoint;
}

bool CollisionComponent::GetFlagRespawn() const
{
	return flagRespawn;
}

bool CollisionComponent::GetFlagTrigger() const
{
	return flagTrigger;
}

bool CollisionComponent::GetFlagPopup() const
{
	return flagPopup;
}

bool CollisionComponent::GetFlagDIn() const
{
	return flagDIn;
}

bool CollisionComponent::GetFlagDOut() const
{
	return flagDOut;
}

bool CollisionComponent::GetFlagEndGameBox() const
{
	return flagEndGameBox;
}

bool CollisionComponent::GetFlagCollider() const
{
	return flagCollider;
}

bool CollisionComponent::GetCollidePlayerHitbox() const
{
	return collidePlayerHitbox;
}

bool CollisionComponent::GetCollidePlayerDashHitbox() const
{
	return collidePlayerDashHitbox;
}

bool CollisionComponent::GetCollideBigPunchHitbox() const
{
	return collideBigPunchHitbox;
}

bool CollisionComponent::GetCollideEnemyDashHitbox() const
{
	return collideEnemyDashHitbox;
}

bool CollisionComponent::GetCollideEnemyCounterHitbox() const
{
	return collideEnemyCounterHitbox;
}

bool CollisionComponent::GetCollideEnemyHitbox() const
{
	return collideEnemyHitbox;
}

bool CollisionComponent::GetPlayerHitboxFlag() const
{
	return playerHitboxFlag;
}

bool CollisionComponent::GetPlayerDashHitboxFlag() const
{
	return playerDashHitboxFlag;
}

bool CollisionComponent::GetBigPunchHitboxFlag() const
{
	return bigPunchHitboxFlag;
}

bool CollisionComponent::GetEnemyDashHitboxFlag() const
{
	return enemyDashHitboxFlag;
}

bool CollisionComponent::GetEnemyCounterHitboxFlag() const
{
	return enemyCounterHitboxFlag;
}

bool CollisionComponent::GetEnemyHitboxFlag() const
{
	return enemyHitboxFlag;
}

std::vector<SceneActor*>& CollisionComponent::GetVecOfPtrToCollidedObject()
{
	return vecOfPtrToCollidedObject;
}

void CollisionComponent::PushBackCollidedObject(SceneActor * obj)
{
	vecOfPtrToCollidedObject.push_back(obj);
}


bool CollisionComponent::DetectInRadius(float squareddist, TransformComponent * transComp1, TransformComponent * transComp2)
{
	squareddist = abs(squareddist);
	Vector2D displacement = transComp2->GetPosition() - transComp1->GetPosition();
	float displacement2 = Vector2DLengthSquared(displacement);
	if (displacement2 <= squareddist)
		return true;
	else
		return false;
}

void CollisionComponent::SetColliding(bool val)
{
	isColliding = val;
}

void CollisionComponent::SetCollidingRight(bool val)
{
	isCollidingRight = val;
}

void CollisionComponent::SetCollidingBottom(bool val)
{
	isCollidingBottom = val;
}

void CollisionComponent::SetCollidingLeft(bool val)
{
	isCollidingLeft = val;
}

void CollisionComponent::SetCollidingTop(bool val)
{
	isCollidingTop = val;
}
void CollisionComponent::SetCollidingvsTornado(bool val)
{
	isCollidingvsTornado = val;
}
void CollisionComponent::SetCollidingvsCrumbling(bool val)
{
	isCollidingvsCrumbling = val;
}
void CollisionComponent::SetCollidingvsPit(bool val)
{
	isCollidingvsPit = val;
}
void CollisionComponent::SetCollidingvsMoving(bool val)
{
	isCollidingvsMoving = val;
}
void CollisionComponent::SetCollidingvsEnemy(bool val)
{
	isCollidingvsEnemy = val;
}
void CollisionComponent::SetCollidingvsSwitch(bool val)
{
	isCollidingvsSwitch = val;
}
void CollisionComponent::SetCollidingvsCheckpoint(bool val)
{
	isCollidingvsCheckpoint = val;
}
void CollisionComponent::SetCollidingvsRespawn(bool val)
{
	isCollidingvsRespawn = val;
}
void CollisionComponent::SetCollidingvsTrigger(bool val)
{
	isCollidingvsTrigger = val;
}
void CollisionComponent::SetCollidingvsPopup(bool val)
{
	isCollidingvsPopup = val;
}
void CollisionComponent::SetCollidingvsDIn(bool val)
{
	isCollidingvsDIn = val;
}
void CollisionComponent::SetCollidingvsDOut(bool val)
{
	isCollidingvsDOut = val;
}
void CollisionComponent::SetCollidingvsEndGameBox(bool val)
{
	isCollidingvsEndGameBox = val;
}
void CollisionComponent::SetCollidingvsCollider(bool val)
{
	isCollidingvsCollider = val;
}
void CollisionComponent::SetFlag(bool val)
{
	flag = val;
}
void CollisionComponent::SetFlagTornado(bool val)
{
	flagTornado = val;
}
void CollisionComponent::SetFlagCrumbling(bool val)
{
	flagCrumbling = val;
}
void CollisionComponent::SetFlagPit(bool val)
{
	flagPit = val;
}
void CollisionComponent::SetFlagMoving(bool val)
{
	flagMoving = val;
}
void CollisionComponent::SetFlagEnemy(bool val)
{
	flagEnemy = val;
}
void CollisionComponent::SetFlagSwitch(bool val)
{
	flagSwitch = val;
}
void CollisionComponent::SetFlagCheckpoint(bool val)
{
	flagCheckpoint = val;
}
void CollisionComponent::SetFlagRespawn(bool val)
{
	flagRespawn = val;
}
void CollisionComponent::SetFlagTrigger(bool val)
{
	flagTrigger = val;
}
void CollisionComponent::SetFlagPopup(bool val)
{
	flagPopup = val;
}
void CollisionComponent::SetFlagDIn(bool val)
{
	flagDIn = val;
}
void CollisionComponent::SetFlagDOut(bool val)
{
	flagDOut = val;
}
void CollisionComponent::SetFlagEndGameBox(bool val)
{
	flagEndGameBox = val;
}
void CollisionComponent::SetFlagCollider(bool val)
{
	flagCollider = val;
}
void CollisionComponent::SetCollidePlayerHitbox(bool val)
{
	collidePlayerHitbox = val;
}
void CollisionComponent::SetCollidePlayerDashHitbox(bool val)
{
	collidePlayerDashHitbox = val;
}
void CollisionComponent::SetCollideBigPunchHitbox(bool val)
{
	collideBigPunchHitbox = val;
}
void CollisionComponent::SetCollideEnemyDashHitbox(bool val)
{
	collideEnemyDashHitbox = val;
}
void CollisionComponent::SetCollideEnemyCounterHitbox(bool val)
{
	collideEnemyCounterHitbox = val;
}
void CollisionComponent::SetCollideEnemyHitbox(bool val)
{
	collideEnemyHitbox = val;
}
void CollisionComponent::SetPlayerHitboxFlag(bool val)
{
	playerHitboxFlag = val;
}
void CollisionComponent::SetPlayerDashHitboxFlag(bool val)
{
	playerDashHitboxFlag = val;
}
void CollisionComponent::SetBigPunchHitboxFlag(bool val)
{
	bigPunchHitboxFlag = val;
}
void CollisionComponent::SetEnemyDashHitboxFlag(bool val)
{
	enemyDashHitboxFlag = val;
}
void CollisionComponent::SetEnemyCounterHitboxFlag(bool val)
{
	enemyCounterHitboxFlag = val;
}
void CollisionComponent::SetEnemyHitboxFlag(bool val)
{
	enemyHitboxFlag = val;
}
void CollisionComponent::SetAll(bool val)
{
	isCollidingRight = val;
	isCollidingBottom = val;
	isCollidingLeft = val;
	isCollidingTop = val;
	isColliding = val;
	isCollidingvsTornado = val;
	isCollidingvsCrumbling = val;
	isCollidingvsPit = val;
	isCollidingvsMoving = val;
	isCollidingvsEnemy = val;
	//isCollidingvsSwitch = val;
	isCollidingvsCheckpoint = val;
	isCollidingvsRespawn = val;
	isCollidingvsTrigger = val;
	isCollidingvsPopup= val;
	isCollidingvsDIn = val;
	isCollidingvsDOut = val;
	isCollidingvsEndGameBox = val;
	isCollidingvsCollider = val;

	flag = val;
	flagTornado = val;
	flagCrumbling = val;
	flagPit = val;
	flagMoving = val;
	flagEnemy = val;
	flagSwitch = val;
	flagCheckpoint = val;
	flagRespawn = val;
	flagTrigger = val;
	flagPopup = val;
	flagDIn = val;
	flagDOut = val;
	flagEndGameBox = val;
	flagCollider = val;

	collidePlayerHitbox = val;
	collidePlayerDashHitbox = val;
	collideBigPunchHitbox = val;
	collideEnemyDashHitbox = val;
	collideEnemyCounterHitbox = val;
	collideEnemyHitbox = val;

	playerHitboxFlag = val;
	playerDashHitboxFlag = val;
	bigPunchHitboxFlag = val;
	enemyDashHitboxFlag = val;
	enemyCounterHitboxFlag = val;
	enemyHitboxFlag = val;

	if (!val)
		vecOfPtrToCollidedObject.clear();
}
void CollisionComponent::CheckFlagAndSet(bool val)
{
	if (GetFlagCollider())
		SetCollidingvsCollider(val);
	if (GetFlagEndGameBox())
		SetCollidingvsEndGameBox(val);
	if (GetFlagDOut())
		SetCollidingvsDOut(val);
	if (GetFlagDIn())
		SetCollidingvsDIn(val);
	if (GetFlagPopup())
		SetCollidingvsPopup(val);
	if (GetFlagTrigger())
		SetCollidingvsTrigger(val);
	if (GetFlagRespawn())
		SetCollidingvsRespawn(val);
	if (GetFlagTornado())
		SetCollidingvsTornado(val);
	if (GetFlagCrumbling())
		SetCollidingvsCrumbling(val);
	if (GetFlagPit())
		SetCollidingvsPit(val);
	if (GetFlagMoving())
		SetCollidingvsMoving(val);
	if (GetFlagEnemy())
		SetCollidingvsEnemy(val);
	if (GetFlagSwitch())
		SetCollidingvsSwitch(val);
	if (GetFlagCheckpoint())
		SetCollidingvsCheckpoint(val);
	if (GetFlag())
		SetColliding(val);

	if (GetPlayerHitboxFlag())
		SetCollidePlayerHitbox(val);
	if (GetPlayerDashHitboxFlag())
		SetCollidePlayerDashHitbox(val);
	if (GetBigPunchHitboxFlag())
		SetCollideBigPunchHitbox(val);
	if (GetEnemyDashHitboxFlag())
		SetCollideEnemyDashHitbox(val);
	if (GetEnemyCounterHitboxFlag())
		SetCollideEnemyCounterHitbox(val);
	if (GetEnemyHitboxFlag())
		SetCollideEnemyHitbox(val);
}
void CollisionComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("CollisionComponent");
	auto map = metadata.reflect<TypeErasedObj>(*this);
	for (auto elem : map)
	{
		AutoWrite(writer, elem.first, elem.second.GetValue());
	}
}
void CollisionComponent::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<CollisionComponent>("ColComponent",
		// Member functions
		"GetColRight", &CollisionComponent::GetCollidingRight,
		"GetColBottom", &CollisionComponent::GetCollidingBottom,
		"GetColLeft", &CollisionComponent::GetCollidingLeft,
		"GetColTop", &CollisionComponent::GetCollidingTop,
		"GetColCrumble", &CollisionComponent::GetCollidingvsCrumbling,
		"GetColPitfall", &CollisionComponent::GetCollidingvsPit,
		"GetColTornado", &CollisionComponent::GetCollidingvsTornado,
		"GetColMoving", &CollisionComponent::GetCollidingvsMoving,
		"GetColEnemy", &CollisionComponent::GetCollidingvsEnemy,
		"GetColSwitch", &CollisionComponent::GetCollidingvsSwitch,
		"GetColCheckpoint", &CollisionComponent::GetCollidingvsCheckpoint,
		"GetColTrigger", &CollisionComponent::GetCollidingvsTrigger,
		"GetColPopup", &CollisionComponent::GetCollidingvsPopup,
		"GetColDIn", &CollisionComponent::GetCollidingvsDIn,
		"GetColDOut", &CollisionComponent::GetCollidingvsDOut,
		"GetColEndGameBox", &CollisionComponent::GetCollidingvsEndGameBox,
		"GetColCollider", &CollisionComponent::GetCollidingvsCollider,

		"GetColPlayerHitbox", &CollisionComponent::GetCollidePlayerHitbox,
		"GetColPlayerDashHitbox", &CollisionComponent::GetCollidePlayerDashHitbox,
		"GetColBigPunchHitbox", &CollisionComponent::GetCollideBigPunchHitbox,
		"GetColEnemyDashHitbox", &CollisionComponent::GetCollideEnemyDashHitbox,
		"GetColEnemyCounterHitbox", &CollisionComponent::GetCollideEnemyCounterHitbox,
		"GetColEnemyHitbox", &CollisionComponent::GetCollideEnemyHitbox,

		"GetIsCol", &CollisionComponent::GetIsColliding,
		"DetectInRadius", &CollisionComponent::DetectInRadius,

		"GetCollidedObject", &CollisionComponent::GetVecOfPtrToCollidedObject,

		sol::base_classes	,sol::bases<Component>()
		);

		
}

bool CompareData(CollisionComponent * lhs, CollisionComponent * rhs)
{
	UNREFERENCED_PARAMETER(lhs);
	UNREFERENCED_PARAMETER(rhs);
	/*if (lhs && rhs)
	{
		if (lhs->GetCollidingTop() != rhs->GetCollidingTop())
			return false;
		if (lhs->GetCollidingBottom() != rhs->GetCollidingBottom())
			return false;
		if (lhs->GetCollidingLeft() != rhs->GetCollidingLeft())
			return false;
		if (lhs->GetCollidingRight() != rhs->GetCollidingRight())
			return false;
	}*/
	return true;
}