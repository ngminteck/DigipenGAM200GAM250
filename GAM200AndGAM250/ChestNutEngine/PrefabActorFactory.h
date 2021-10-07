/******************************************************************************/
/*!
\file   PrefabActorFactory.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
		the file contain the main prefab container, loading and saving, checking
	prefab integrity and the function of editing the prefab in editor, 
	like select,move,duplicate,rename,delete,new

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include <filesystem>

#include "PrefabActor.h"
#include "ResourceFactory.h"
#include "StatsFactory.h"
#include "RigidBodyFactory.h"
#include "SpriteFactory.h"
#include "CollisionFactory.h"
#include "AABBFactory.h"
#include "TransformFactory.h"
#include "MovementFactory.h"
#include "ShaderFactory.h"
#include "ScriptFactory.h"
#include "AudioFactory.h"
#include "ButtonFactory.h"
#include "CameraFactory.h"
#include "DialogueFactory.h"
#include <sol.hpp>
#include "CheckpointFactory.h"

#include "MainWindow.h"
#include "GraphicsSystem.h"

struct CheckpointFactory;

class PrefabActorFactory
{
public:
	PrefabActorFactory(SpriteSheetFactory * spritesheet_fac, sol::state*, MainWindow *win, GraphicsSystem* graphic );
	~PrefabActorFactory();

	void InitComponentFactoryAndConstructDefaultPrefab();
	Component* InitComponent(std::string component, const rapidjson::Value& node);
	Component* InitDefaultComponent(std::string component);
	void LoadPrefabActorFromJsonFile(const char * filepath);
	void ScanPrefabActorDirectory();
	void SaveAllPrefabActor();
	void SavePrefabActor(PrefabActor * actor);
	std::unordered_map<std::string, ComponentFactory*> GetFactories() const;
	std::unordered_map<unsigned, PrefabActor *> & GetPrefabActorContainer();
	std::unordered_map<std::string, std::string> & GetDefaultPrefabActorContainer();
	std::unordered_map<unsigned, std::string> & GetObjectTypeContainer();
	std::unordered_map<ComponentType, std::string> & GetComponentTypeContainer();
	unsigned NewPrefab(unsigned index);
	unsigned DuplicatePrefab(PrefabActor * duplicated_actor);
	void DeletePrefab(unsigned index);
	bool PrefabCheck(PrefabActor * lhs, PrefabActor * rhs, bool diff_prefab_name = false);
	unsigned FindAnySimilarPrefabData(PrefabActor * prefab_actor);
	void DeletePrefabFile(std::string path);

	void PrefabActorShiftSelection(PrefabActor * last_click);
	void PrefabActorSelectionDelete(PrefabActor * prefab_actor);

	void SetDefaultPrefabAttribute(PrefabActor * prefab_actor);

	void AutoCreatePrefab();

	void InitDigipenLogoAndTeamLogo();

	SpriteSheetFactory* m_SpriteSheetFactory;
	PrefabActor * background_logo;
	PrefabActor * digipen_logo;
	PrefabActor * team_logo;

private:
	sol::state* LuaState;

	MainWindow * m_win;
	GraphicsSystem * m_graphic;

	unsigned prefab_actor_container_key;
	std::unordered_map<unsigned, PrefabActor *> prefab_actor_container;
	
	std::unordered_map<std::string, std::string> default_prefab_actor_container;
	std::unordered_map<unsigned, std::string> ObjectTypeList;
	std::unordered_map<ComponentType, std::string> ComponentTypeList;


	// all ComponentFactory
	std::unordered_map<std::string, ComponentFactory*> m_objMap;

	PrefabActor * CreateChildPrefabActor(const rapidjson::Value& node, PrefabActor * parent_actor);
	void SaveChildPrefabActor(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer, PrefabActor * child_prefab_actor);
	void ConstructDefaultPrefabActor();
	bool ComponentCheck(PrefabActor * lhs, PrefabActor * rhs);

};