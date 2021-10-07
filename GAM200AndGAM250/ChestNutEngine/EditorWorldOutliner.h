/******************************************************************************/
/*!
\file   EditorWorldOutliner.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	the world outliner

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include <filesystem>
#include <sstream>
#include "imgui.h"
#include "imgui_internal.h"
#include "Factory.h"
#include "EditorConsole.h"
#include "Camera.h"
#include "CommandManager.h"
#include "SwitchLevelEvent.h"
#include "PauseGame.h"
#include "DeleteActor.h"
#include "ResourceFactoryManager.h"
#include "LoadLevel.h"
#include "PlayLevel.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


class EditorWorldOutliner
{
public:
	EditorWorldOutliner(EditorConsole *, ResourceFactoryManager * , EventHandler *, CommandManager *, bool & pause);
	~EditorWorldOutliner();
	void Draw();
	
	Camera * m_cam;
private:
	ResourceFactoryManager * m_ResourceFactoryManager;
	EditorConsole * m_console;
	EventHandler * m_ev;
	Profiler * m_profiler;
	bool & m_pause;

	std::string search;
	std::string levelName;
	std::string new_name;
	std::string levelfilepath;

	unsigned prefab_actor_key_that_required_update;
	bool deleting;
	bool moving;
	CommandManager * m_cmd;
	SceneActor * dest;

	void MainMenu();
	void SortByName();
	void ObjectBranch();
	void ChildObjectBranch(std::map<unsigned, SceneActor *> child_object);

};