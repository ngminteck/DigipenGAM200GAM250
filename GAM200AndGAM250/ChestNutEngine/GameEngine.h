/******************************************************************************/
/*!
\file   GameEngine.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for GameEngine

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include "MainWindow.h"
#include "Input.h"
#include "GraphicsSystem.h"
#include "FrameController.h"

#include "Event.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "LogicSystem.h"
#include "AnimationSystem.h"
#include "AudioSystem.h"
#include "CheckpointSystem.h"
#include "TransitionSystem.h"
#include "DialogueSystem.h"
#include "Factory.h"
#include "TextureFactory.h"
#include "MeshFactory.h"
#include "ButtonSystem.h"
#include "Editor.h"
#include "EventHandler.h"
#include "ProfileHandler.h"
#include "ResourceFactoryManager.h"
#include "ScriptManager.h"
#include "GraphicsPreprocess.h"
#include "LoadGameState.h"
#include "XBoxInputSystem.h"
#include "ActivateFade.h"
#include <vector>

#include <future>
#include <thread>
#include <chrono>
#include <tchar.h>
#include <strsafe.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

class GameEngine
{
public:
	GameEngine(WNDPROC);							// Contrustor
	~GameEngine();									// Destructor

	bool Initialize();								// Initialize the engine (`create the window)
	void Update(MSG& msg, bool active);				// Update the window

	//Graphics Functions
	void ResizeWindow(int, int);					// Resize the window

    //Window Functions
	void ToggleFullScreenMode();					// Toggle between fullscreen and window mode
	void DestroyWindow();							// Close the rendering window

	//Input Functions
	bool InputCheckCurr(unsigned char key);			// Check If Current Input
	bool InputCheckPrev(unsigned char key);			// Check If Previously Input
	bool InputCheckTriggered(unsigned char key);	// Check If Recently Triggered
	bool InputCheckReleased(unsigned char key);		// Check If Released
	void InputShowCursor(bool input);				// To Show or Hide Cursor
	
	//FrameController Functions
	void StartFrame();								// Capture start of frame time
	void EndFrame();								// Limit FPS

	//Misc
	void Pause(PauseGame *);
	void ReinitializeEngine(LoadLevel *);

	void Restart(PlayerDies *);
	void LoadNewLevel(LoadGameState *);
	void Exit(QuitGame *);

	void FadeIn();
	void FadeOut();

	void LoadingSceneRender();

	void ActivateFadeTransition(ActivateFade *);

	//Getters
	ResourceFactoryManager * GetResourceManager();
	GraphicsSystem          * GetGraphicsSystem();

	std::string levelPath;
	TransitionSystem * m_transition;
private:
	//Member Systems
	FrameController	* m_GameTimer;					// Frame Rate Controller
	MainWindow		* m_mainWindow;					// Game Window
	InputSystem		* m_inputSystem;				// Input System
	GraphicsSystem	* m_graphicsSystem;				// Graphics System
	HealthSystem	* m_HealthSystem;				// Health System		
	PhysicsSystem	* m_physicsSystem;				// Physics System
	LogicSystem     * m_logicSystem;
	AnimationSystem * m_animationSystem;
	ScriptManager	* m_ScriptManager;	
	AudioSystem		* m_audioSystem;
	ButtonSystem    * m_buttonSystem;
	GraphicsPreprocess* m_GraphicsPreprocess;
	CheckpointSystem * m_checkpoint;
	XBoxInputSystem  * m_xboxInput;
	DialogueSystem   * m_dialogue;

	//Factories
	ResourceFactoryManager * m_ResourceFactoryManager;

	//EventHandler
	EventHandler    * m_ev;

	// Editor
	Editor			 * m_editor; 

	// Profiler Handler
	ProfileHandler *m_profiler;

	//Member variables
	std::map<std::string,System*> systems;

	bool pause;
	bool debug = false;
	bool exit;
	bool transition = false;

	const float dt = 1.f / 60;

	unsigned int user_cpu_core;
};

