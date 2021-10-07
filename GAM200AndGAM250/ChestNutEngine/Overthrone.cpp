#include "Overthrone.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

Overthrone::Overthrone(GameEngine * engine, GameStateManager * gamestate)
	:m_engine{ engine }, m_gamestate{gamestate}
{

}

int Overthrone::Init()
{
	if (!m_engine)	// Shutdown if failed to create									
		return -1;
	if (!m_engine->Initialize())
		return -1;
	std::cout << "run" << std::endl;
	return 1;
}

void Overthrone::InitializeCamera()
{
	//m_engine->GetGraphicsSystem()->default_camera->LatchToObject(m_engine->GetResourceManager()->GetSceneActorFactory()->GetSceneActorContainer(), "Surtur");
}

void Overthrone::Load(std::string path)
{
	m_engine->levelPath = path;
	InitializeCamera();
	m_engine->GetResourceManager()->GetSceneActorFactory()->LoadSceneActorFromJsonFile(path.c_str());
}

void Overthrone::Update(MSG & msg, bool & active)
{
	m_engine->Update(msg, active);
}

void Overthrone::Unload()
{
	m_engine->GetResourceManager()->GetSceneActorFactory()->UnloadSceneActorFactory();
}

Overthrone::~Overthrone()
{
}

void Overthrone::RunGame(MSG & msg, bool & active)
{
	Init();
	m_gamestate->current = m_gamestate->next;
	while (m_gamestate->current == m_gamestate->next)
	{
		if (msg.message == WM_QUIT) break;
		Load(m_gamestate->GetGameState());
		while (msg.message != WM_QUIT)
		{
			Update(msg, active);
		}
	}
    Unload();
}

void Overthrone::ExitGame()
{
	//SafeDelete(m_engine);
}