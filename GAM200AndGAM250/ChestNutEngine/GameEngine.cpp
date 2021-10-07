/******************************************************************************/
/*!
\file   GameEngine.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts definition for GameEngine

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "GameEngine.h"
#include "Utils.h"
#include <iostream>
#include "PauseGame.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

GameEngine::GameEngine(WNDPROC WndProc)
	: pause{ false }, exit{ false },  user_cpu_core{ std::thread::hardware_concurrency() }
{
	// m_WndProc is the m_WndProc procedure from the main file
	m_mainWindow = new MainWindow(WndProc, 1280, 800, "Test", false);	// Creates the new renderer
	m_inputSystem = new InputSystem();		// Creates the input system
	m_GameTimer = new FrameController();				// Create the game timer
	m_graphicsSystem = new GraphicsSystem(m_mainWindow->GetWndWidth(), m_mainWindow->GetWndHeight());
	m_ev = new EventHandler();
	m_HealthSystem = new HealthSystem(m_ev);
	m_physicsSystem = new PhysicsSystem(m_ev, pause);
	m_logicSystem = new LogicSystem{ m_ev, pause };
	m_xboxInput = new XBoxInputSystem{ m_inputSystem };
	m_audioSystem = new AudioSystem{ m_ev, pause };
	m_ScriptManager = new ScriptManager(m_inputSystem, m_xboxInput, m_audioSystem);
	m_ResourceFactoryManager = new ResourceFactoryManager(m_mainWindow, &(m_ScriptManager->LState), m_ev, m_graphicsSystem);
	m_GraphicsPreprocess = new GraphicsPreprocess{ &m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(), m_ev };
	m_buttonSystem = new ButtonSystem(m_xboxInput, m_inputSystem, m_ResourceFactoryManager->GetSceneActorFactory(), m_ev, m_ResourceFactoryManager->GetSpriteSheetFactory(), pause, m_audioSystem->m_master_volume);
	m_transition = new TransitionSystem{ transition, m_ev };
	m_checkpoint = new CheckpointSystem(m_ev, m_ResourceFactoryManager->GetSceneActorFactory());
	m_dialogue = new DialogueSystem{ m_xboxInput, m_inputSystem, m_ev, &m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(),  m_ResourceFactoryManager->GetSpriteSheetFactory(), pause };
	m_animationSystem = new AnimationSystem(m_ev, pause, m_dialogue->m_dialogueMode);
	m_profiler = new ProfileHandler{ { &m_graphicsSystem->profiler, &m_HealthSystem->profiler, &m_physicsSystem->profiler, &m_animationSystem->profiler, &m_logicSystem->profiler } };
	m_editor = new Editor{ m_inputSystem, m_profiler, m_ResourceFactoryManager, m_ev, pause, m_physicsSystem->gravity };
}

GameEngine::~GameEngine() 
{
	// Delete editor
	SafeDelete(m_editor);
	// Delete game window
	SafeDelete(m_mainWindow);
	// Delete input system
	SafeDelete(m_inputSystem);
	// Delete game timer
	SafeDelete(m_GameTimer);
	// Delete graphics system
	SafeDelete(m_graphicsSystem);
	// Delete health system
	SafeDelete(m_HealthSystem);
	// Delete physics dsystem
	SafeDelete(m_physicsSystem);
	// Delete animation system
	SafeDelete(m_animationSystem);
	// Delete logic system
	SafeDelete(m_logicSystem);
	// Delete Factory
	SafeDelete(m_ResourceFactoryManager);
	// Delete event handler
	SafeDelete(m_ev);
	// Delete profile handler
	SafeDelete(m_profiler)
	// Delete script manager
	SafeDelete(m_ScriptManager);
	SafeDelete(m_audioSystem);
	SafeDelete(m_GraphicsPreprocess);
	SafeDelete(m_buttonSystem);
	SafeDelete(m_transition);
	SafeDelete(m_checkpoint);
	SafeDelete(m_xboxInput);
	SafeDelete(m_dialogue);
}

bool GameEngine::Initialize()
{
	srand((unsigned int)time(NULL));
	// Initialize the engine systems
	if (!m_mainWindow->WindowCreate(m_graphicsSystem))
		return false;
	if (!m_inputSystem->Initialize(m_mainWindow->GetHWND()))
		return false;

	m_ResourceFactoryManager->InitFactory();
	m_editor->InitEditor(*m_mainWindow->GetHWND(), m_graphicsSystem->default_camera);
	//m_graphicsSystem->default_camera->LatchToObject(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(), "Surtur");
	m_audioSystem->Init(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer());
	m_logicSystem->SetSceneFactory(&m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer());
	m_buttonSystem->Initialize();
	m_dialogue->Initialize();
	m_checkpoint->Initialize();

	while (m_ResourceFactoryManager->logo_timer > 0.0f)
	{
		/*if (InputCheckTriggered(CVK_ESC))
		{
			m_ResourceFactoryManager->logo_timer -= 5.0f;
		}*/

		m_graphicsSystem->startDrawingScene();
		m_graphicsSystem->drawLogo(m_ResourceFactoryManager->GetPrefabActorFactory()->background_logo);
		if (m_ResourceFactoryManager->logo_timer > 5.0f)
		{
			m_graphicsSystem->drawLogo(m_ResourceFactoryManager->GetPrefabActorFactory()->digipen_logo);
		}
		else
		{
			m_graphicsSystem->drawLogo(m_ResourceFactoryManager->GetPrefabActorFactory()->team_logo);
		}
		m_graphicsSystem->stopDrawingScene(*m_mainWindow->GetHDC());
		m_ResourceFactoryManager->logo_timer -= dt;
	}


	m_ev->SubscribeEvent(this, &GameEngine::ReinitializeEngine);
	m_ev->SubscribeEvent(this, &GameEngine::Pause);
	m_ev->SubscribeEvent(this, &GameEngine::Restart);
	m_ev->SubscribeEvent(this, &GameEngine::Exit);
	m_ev->SubscribeEvent(this, &GameEngine::LoadNewLevel);

	//m_ev->PublishEvent(new LoadGameState{ "Resource/Json/Level/ta2.json" });

	pause = false;
	return true;
}


void GameEngine::Update(MSG& msg, bool)
{

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Check for new message
	{
		TranslateMessage(&msg);						// Translate The Message
		DispatchMessage(&msg);						// Dispatch The Message
	}

	StartFrame();								// Capture Time at the Start of Frame
	m_editor->SetupEditorFrame();

	if(exit) 
		msg.message = WM_QUIT;

	if (!ImGui::IsAnyItemFocused()) 
	m_xboxInput->Update();
    m_inputSystem->Update();


	if (m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer().size() > 0)
	{
		for (int step = 0; step < m_GameTimer->GetNOS(); ++step)
		{
			m_logicSystem->Update(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer());
			m_physicsSystem->Update(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(), dt);
			m_audioSystem->Update();
			m_checkpoint->Update();
			m_animationSystem->Update(m_GraphicsPreprocess->GetGraphicContainerPtr());
			if (debug) 
				m_editor->Update();
		}
	}

	m_buttonSystem->Update();
	m_dialogue->Update();
	
	
		m_graphicsSystem->startDrawingScene();
		m_graphicsSystem->drawScene(m_GraphicsPreprocess->GetGraphicContainerPtr(), m_ResourceFactoryManager->GetSceneActorFactory()->GetCurrentSceneCamera(), dt, m_ResourceFactoryManager->GetSceneActorFactory()->GetLoadingContainer(),debug);
		if (debug)
		{
			m_editor->DrawEditor();
			m_editor->UpdateEditorViewPort();
		}

		m_graphicsSystem->stopDrawingScene(*m_mainWindow->GetHDC());
	

	m_profiler->Update();
	m_editor->EndFrame();

	EndFrame();  // Capture Time at End of Frame


	if (InputCheckTriggered(CVK_TILDE))
	{
		debug = !debug;
		m_physicsSystem->gravity = true;
	}

	if (InputCheckTriggered(CVK_ESC) || m_xboxInput->isKeyTriggered(BUTTON_START))
	{
		m_ev->PublishEvent(new SwitchLevelEvent{});
		//m_ev->PublishEvent(new LoadLevel{});
		if(m_dialogue->m_dialogueMode == false)
			pause = !pause;

		m_buttonSystem->TogglePauseMenu(pause);
		//->PublishEvent(new PlayLevel{});
	}

	if (InputCheckCurr(CVK_CTRL))
	{
		if (InputCheckTriggered(CVK_1))
			m_ev->PublishEvent(new LoadGameState{"Resource/Json/Level/Level001.json"});
		if (InputCheckTriggered(CVK_2))
			m_ev->PublishEvent(new LoadGameState{ "Resource/Json/Level/Level002.json" });
		if (InputCheckTriggered(CVK_3))
			m_ev->PublishEvent(new LoadGameState{ "Resource/Json/Level/Boss.json" });
	}



#ifdef DEBUG


  if (InputCheckCurr(CVK_O))
  {
    m_graphicsSystem->SetZoom(0.05f);
  }
  if (InputCheckCurr(CVK_P))
  {
    m_graphicsSystem->SetZoom(-0.05f);
  }
#endif // _DEBUG
}


void GameEngine::ResizeWindow(int width, int height)
{
	// Resize the window
	m_graphicsSystem->ResizeScene(width, height);
}

void GameEngine::ToggleFullScreenMode() 
{
	// Toggle between fullscreen and window mode
	m_mainWindow->ToggleFullScreenMode(m_graphicsSystem);
}

void GameEngine::DestroyWindow()
{
	// Close the window
	m_editor->ShutdownEditor();
	m_mainWindow->WindowDestroy();

}

bool GameEngine::InputCheckCurr(unsigned char key)
{
	return m_inputSystem->InputCheckCurr(key);
}

bool GameEngine::InputCheckPrev(unsigned char key)
{
	return m_inputSystem->InputCheckPrev(key);
}

bool GameEngine::InputCheckTriggered(unsigned char key)
{
	return m_inputSystem->InputCheckTriggered(key);
}

bool GameEngine::InputCheckReleased(unsigned char key)
{
	return m_inputSystem->InputCheckReleased(key);
}

void GameEngine::InputShowCursor(bool input)
{
	ShowCursor(input);
}

void GameEngine::StartFrame()
{
	m_GameTimer->FrameStart();
}
void GameEngine::EndFrame()
{
	m_GameTimer->FrameEnd();
}

void GameEngine::Pause(PauseGame * ev)
{
	UNREFERENCED_PARAMETER(ev);
	pause = !pause;
}


void GameEngine::ReinitializeEngine(LoadLevel *)
{
	pause = false;
	m_transition->Initialize(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer());
}

void GameEngine::Restart(PlayerDies *)
{
	FadeIn();
	pause = true;
	m_buttonSystem->ActivateDeathMenu();
}

void GameEngine::LoadNewLevel(LoadGameState * gamestate)
{
	//Activate fade to black transition

	FadeIn();
	
	//Unload the current container
	m_graphicsSystem->default_camera->UnlatchObject();
	m_ResourceFactoryManager->GetSceneActorFactory()->UnloadSceneActorFactory();

	LoadingSceneRender();
	

	//Reload the current level
	m_ResourceFactoryManager->GetSceneActorFactory()->LoadSceneActorFromJsonFile(gamestate->m_path.c_str());
	//m_graphicsSystem->default_camera->LatchToObject(m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer(), "Surtur");
	FadeOut();
}


void GameEngine::Exit(QuitGame *)
{
	exit = true;
	//m_ev->PublishEvent(new LoadGameState{"Resource/Json/Level/MainMenu.json"});
}

void GameEngine::FadeIn()
{
	SpriteComponent * ftb = m_transition->m_ftb_sprite;
	SceneActor * ftb_obj = m_transition->m_ftb;
	pause = false;

	if (ftb)
	{
		ftb_obj->SetOnlyActive(true);
		//Set orthoganal objects to false;
		if (ftb->m_OrthogonalCamera == false)
		{
			for (auto & elem : m_ResourceFactoryManager->GetSceneActorFactory()->GetSceneActorContainer())
			{
				auto comp = elem.second->GetComponent(ComponentType::SpriteComponent);
				SpriteComponent * sprite = dynamic_cast<SpriteComponent *>(comp);
				if (sprite && sprite->m_OrthogonalCamera)
					elem.second->SetOnlyActive(false);
			}
		}

		while (ftb->GetFrameCount() != ftb->GetAnimation()->GetNumFrames() - 1)
		{
			m_graphicsSystem->startDrawingScene();
			m_graphicsSystem->drawScene(m_GraphicsPreprocess->GetGraphicContainerPtr(), m_ResourceFactoryManager->GetSceneActorFactory()->GetCurrentSceneCamera(), dt, m_ResourceFactoryManager->GetSceneActorFactory()->GetLoadingContainer(), debug);
			m_graphicsSystem->stopDrawingScene(*m_mainWindow->GetHDC());
			m_animationSystem->Update(m_GraphicsPreprocess->GetGraphicContainerPtr());
		}
	}

}

void GameEngine::FadeOut()
{
	SpriteComponent * ftb = m_transition->m_ftb_sprite;
	SceneActor * ftb_obj = m_transition->m_ftb;
	if (ftb)
	{
		ftb_obj->SetOnlyActive(true);
		ftb->SetFrameCount(ftb->GetAnimation()->GetNumFrames() - 1);
		while (ftb->GetFrameCount() != 0)
		{
			m_graphicsSystem->startDrawingScene();
			m_graphicsSystem->drawScene(m_GraphicsPreprocess->GetGraphicContainerPtr(), m_ResourceFactoryManager->GetSceneActorFactory()->GetCurrentSceneCamera(), dt, m_ResourceFactoryManager->GetSceneActorFactory()->GetLoadingContainer(), debug);
			m_graphicsSystem->stopDrawingScene(*m_mainWindow->GetHDC());
			m_animationSystem->Update(m_GraphicsPreprocess->GetGraphicContainerPtr(), true);
		}
		ftb_obj->SetOnlyActive(false);
	}
}

void GameEngine::ActivateFadeTransition(ActivateFade * fade)
{
	if (fade->m_in)
		FadeIn();
	else
		FadeOut();
}

ResourceFactoryManager * GameEngine::GetResourceManager()
{
	return m_ResourceFactoryManager;
}

GraphicsSystem * GameEngine::GetGraphicsSystem()
{
	return m_graphicsSystem;
}

void GameEngine::LoadingSceneRender()
{
	m_graphicsSystem->startDrawingScene();
	m_graphicsSystem->drawLoadScene(m_ResourceFactoryManager->GetSceneActorFactory()->GetLoadingContainer());
	m_graphicsSystem->stopDrawingScene(*m_mainWindow->GetHDC());
}