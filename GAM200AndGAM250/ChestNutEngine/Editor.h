/******************************************************************************/
/*!
\file   Editor.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	Main editor init with docking

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include <iostream>
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include <tchar.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#include "Utils.h"
#include "Factory.h"
#include "Input.h"

#include "ResourceFactoryManager.h"

#include "EditorContentBrowser.h"
#include "EditorWorldOutliner.h"
#include "Inspector.h"
#include "EditorObjectSelector.h"
#include "EditorConsole.h"
#include "CommandManager.h"
#include "PerformanceViewer.h"
//#include "Profiler.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
class Editor
{
public:
	Editor(InputSystem *, ProfileHandler * , ResourceFactoryManager * , EventHandler *, bool &, bool &);
	~Editor();
	void InitEditor(HWND, Camera *);
	void SetupEditorFrame();
	void Update();
	void UpdateCamera();
	void DrawEditor();
	void EndFrame();
	void ShutdownEditor();
	void UpdateEditorViewPort();

	Profiler * GetProfiler() const { return profiler;}

private:

	bool show_demo_window; // testing	
	bool camera_mode;
	
	ResourceFactoryManager * m_ResourceFactoryManager;

	InputSystem * m_input;
	Camera * m_cam;

	Profiler * profiler;
	EditorContentBrowser * editor_content_browser;
	EditorWorldOutliner * editor_world_outliner;
	EditorObjectSelector * editor_object_selector;
	Inspector * editor_object_viewer;
	PerformanceViewer * performance_viewer;
	EditorConsole	 *  console;
	CommandManager	 *  command_manager;
	ProfileHandler  * profile_handler;

	void InitDockingArea();
};