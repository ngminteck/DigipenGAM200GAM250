#pragma once
#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Component.h"
#include "TransformComponent.h"
#include "AABBComponent.h"
#include "CollisionComponent.h"
#include "MovementComponent.h"
#include "ScriptComponent.h"
#include "StatsComponent.h"
#include "SpriteComponent.h"
#include "SceneActor.h"
#include "CameraComponent.h"
#include "EventHandler.h"
#include "SceneActorFactory.h"
#include "XBoxInputSystem.h"
#include "AudioSystem.h"

#include <unordered_map>
class InputSystem;
class ScriptManager
{
private:

public:
	ScriptManager(InputSystem* ,XBoxInputSystem*, AudioSystem*);
	~ScriptManager();

	void BindTypes();

	sol::state LState;
};