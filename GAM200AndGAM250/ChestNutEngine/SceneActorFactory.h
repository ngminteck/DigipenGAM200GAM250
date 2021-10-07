/******************************************************************************/
/*!
\file   SceneActorFactory.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	the file contain the main scene container, loading and saving and the 
	function of edit in the editor, like select, new ,move, duplicate,
	rename,delete

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <math.h>
#include <algorithm>
#include <list>
#include <windows.h>

#include <new>

#include "SceneActor.h"
#include "PrefabActorFactory.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"
#include "DialogueComponent.h"

#include "EventHandler.h"
#include "SwitchLevelEvent.h"
#include "LoadLevel.h"
#include "PlayLevel.h"
#include "ChangeAudio.h"
#include "LoadGameState.h"
#include "PauseGame.h"

#include "MainWindow.h"
#include "GraphicsSystem.h"

class SceneActorFactory
{
public:
	SceneActorFactory(PrefabActorFactory * prefabactor_fac, MainWindow * window, EventHandler * eventhandle, GraphicsSystem* graphic);
	~SceneActorFactory();

	void InitLoadingScreen();

	void RestartSceneLevel();

	// need reset level, garphic, physic
	void LoadSceneActorFromJsonFile(const char * filepath);

	// need reset level, garphic, physic, may have some change so need
	void SaveSceneActor(const std::string & level_name = "nameless");

	void SaveAllNotPrefabChildPrefabInfo(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer,PrefabActor * childprefabactor);

	void DeleteSceneActor(unsigned);
	void AddSceneActor(unsigned, SceneActor *);

	std::map<unsigned, SceneActor *> & GetSceneActorContainer();
	std::map<unsigned, SceneActor *> * GetLoadingContainer();
	void UnloadSceneActorFactory();

	float GetGlobalCoordinateMinX()const;
	float GetGlobalCoordinateMaxX()const;
	float GetGlobalCoordinateMinY()const;
	float GetGlobalCoordinateMaxY()const;

	bool check_level_size;

	SceneActor * NewSceneActor(unsigned key , SceneActor * added_target = nullptr);
	void CloneSceneActor(SceneActor * clone_object);
	void SceneActorSelectionDuplicate();
	void SceneActorSelectionDelete();
	void SceneActorSelectionMove(SceneActor * dest);
	void ClearWholeSceneActorSelection();
	void SceneActorSelectionRename(std::string name);
	void UpdateSceneActorCriticalInfo(std::string path_to_search, PrefabActor * update_info);
	void UpdateAllSceneActor(PrefabActor * update_info);

	void CheckAllSceneActorIntegrityCheck();
	void SceneActorShiftSelection(SceneActor * last_click);

	unsigned *GetCurrentKeyIDPtr();

	bool SavePrefabIntegrityCheck(SceneActor * actor);

	SceneActor * GetCurrentSceneCamera()const;


	static void AddToScript(sol::state * luaState);
	// switch camera
	void SetCurrentSceneCamera(std::string actor_name, bool lerp=false);
	// get current scene camera location
	Vector3D GetCurrentSceneCameraPosition() const;
	// camera shake
	void ShakeCurrentSceneCamera(float amplitude, float frequency =1.0f, float shake_duration = 1.0f);

	// temp view location, for cutscene 
	void SetCurrentSceneCameraViewPosition(float x, float y, float z = 0.0f, bool back_to_object = true, float timer = 0.0f);
	void SetCurrentSceneCameraViewObject(std::string actor_name, float z = 0.0f, bool back_to_object = true, float timer = 0.0f);
	void SetCurrentSceneCameraViewBackItself();
	//

	void SetCurrentSceneCameraLock();
	void SetCurrentSceneCameraUnLock();

	// set temp offset with lerp , for some part of gameplay need character be either left, right, bottom, top etc
	void SetCurrentSceneAnchorPoint(unsigned pos = 5, float padding_x = 0.0f, float padding_y = 0.0f);
	// set zoom with lerp
	void SetCurrentSceneCameraTempZoom(float zoom, bool back_to_object = true, float timer = 0.0f);

	TransformComponent * progress_walk;
	SpriteComponent* progress_frame;

private:
	PrefabActorFactory * m_PrefabActorFactory;
	EventHandler * m_ev;
	MainWindow * m_win;
	GraphicsSystem * m_graphic;


	float global_coordinate_min_x;
	float global_coordinate_max_x;
	float global_coordinate_min_y;
	float global_coordinate_max_y;

	SceneActor * current_scene_camera;

	std::map<unsigned, std::vector<SpriteComponent*>> render_container;

	unsigned scene_actor_container_key;

	std::map<unsigned, SceneActor *> loading_container;
	std::map<unsigned, SceneActor *> scene_actor_container;

	std::list<unsigned> container_index;

	bool first_actor_map_check;

	void InitGlobalCoordinate(float pos_x, float pos_y, float size_x, float size_y, float rot);
	
	void ReadtLevelSize(float min_x, float max_x, float min_y, float max_y);
	void CalculateLevelSize(float pos_x, float pos_y, float size_x, float size_y, float rot);
	SceneActor * CreateChildSceneActor(const rapidjson::Value& node, SceneActor * parent_actor);
	void SaveChildSceneActor(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer, SceneActor * child_scene_actor);
	void ChildSceneActorSelectionDuplicate(SceneActor * copy_source, SceneActor * dest_source, bool duplicate_container = false);
	void ChildSceneActorSelectionDelete(std::map<unsigned, SceneActor *> & container);
	void AttachAllSelectionToDestSceneActor( SceneActor * dest);
	void AttachAllSelectionToDestSceneActor(SceneActor * check , SceneActor * dest);
	void SceneActorSelectionRename(std::string name, SceneActor * actor);
	void UpdateSceneActorCriticalInfo(std::string path_to_search, PrefabActor * update_info , SceneActor * actor);
	void UpdateSceneActor(PrefabActor * update_info, SceneActor * actor);
	void UpdateSceneActor(PrefabActor * update_info , SceneActor * parent , SceneActor * actor);
	void CheckAllSceneActorIntegrityCheck(SceneActor * actor);
	void PrefabIntegrityCheck(SceneActor * actor);
};