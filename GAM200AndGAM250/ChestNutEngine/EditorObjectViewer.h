#pragma once
/******************************************************************************/
/*!
\file  EditorObjectViewer.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EditorObjectViewer class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "imgui.h"
#include "imgui_stl.h"
#include <iostream>
#include <string>
#include "ResourceFactoryManager.h"

#include "MovementComponent.h"
#include <cstring>
#include <stdlib.h> //atoi
#include <any>
#include <sstream>
#include "Camera.h"
#include "EditorConsole.h"
#include "CommandManager.h"
#include "ValueChange.h"
#include "EventHandler.h"
#include <filesystem>
#include "LoadLevel.h"
#include "SwitchLevelEvent.h"
#include "ToggleGravity.h"
#include "ChangeAudio.h"



class EditorObjectViewer
{
public:
	EditorObjectViewer(EditorConsole *, ResourceFactoryManager *, CommandManager *, EventHandler *, bool &);
	~EditorObjectViewer();

	//Core functions
	std::string GetComponentName(ComponentType);
	void PrintComponentDataAux(const std::string &, const std::string &, TypeErasedObj &);
	template<typename T> void PrintComponentData(const std::string &, Component *);
	void PrintComponent(const std::string &, Component *);
	void PrintQuickAccess(SceneActor *, unsigned);
	void PrintCameraData();
	void Draw();
	Camera  * m_cam;
	Profiler * m_profiler;
	
private:

	ResourceFactoryManager * m_ResourceFactoryManager;
	EditorConsole * m_console;
	CommandManager * m_cmd;
	EventHandler   * m_ev;
	bool & m_gravity; //Gravity status of physics system
	void DrawChildInspector(unsigned id, SceneActor * obj);
};


