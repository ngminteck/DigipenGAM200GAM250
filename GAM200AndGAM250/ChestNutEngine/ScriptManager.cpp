#include "ScriptManager.h"
#include "Input.h"
ScriptManager::ScriptManager(InputSystem* input, XBoxInputSystem* xinput, AudioSystem* audio)
	: LState{}
{
	LState.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::math,
						   sol::lib::string, sol::lib::io,sol::lib::table);
	BindTypes();
	LState.script_file("Resource/Scripts/GlobalScripts.lua");
	
	sol::function Start;
	Start = LState["Start"];
	auto result = Start(input, xinput, audio);
}

void ScriptManager::BindTypes()
{
	Vector2D::AddToScript(&LState);
	Vector3D::AddToScript(&LState);
	PrefabActor::AddToScript(&LState);
	SceneActor::AddToScript(&LState);
	Component::AddToScript(&LState);
	AABBComponent::AddToScript(&LState);
	CollisionComponent::AddToScript(&LState);	
	MovementComponent::AddToScript(&LState);
	RigidBodyComponent::AddToScript(&LState);
	StatsComponent::AddToScript(&LState);
	SpriteComponent::AddToScript(&LState);
	TransformComponent::AddToScript(&LState);
	InputSystem::AddToScript(&LState);
	Camera::AddToScript(&LState);
	CameraComponent::AddToScript(&LState);
    SceneActorFactory::AddToScript(&LState);
	XBoxInputSystem::AddToScript(&LState);
	AudioSystem::AddToScript(&LState);
	//EventHandler::AddToScript(&LState);
}

ScriptManager::~ScriptManager()
{
}