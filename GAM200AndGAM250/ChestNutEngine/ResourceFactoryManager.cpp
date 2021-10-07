/******************************************************************************/
/*!
\file   ResourceFactoryManager.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	the resource contianer manager, sceneactor,prefabactor, spritesheet and
	menu container

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "ResourceFactoryManager.h"

ResourceFactoryManager::ResourceFactoryManager(MainWindow * window, sol::state* luaState, EventHandler * eventhandle, GraphicsSystem * graphic)
	: m_win{window}, m_graphic{graphic}, logo_timer{0.0f},
	m_SpriteSheetFactory{ new SpriteSheetFactory{} },
	 m_PrefabActorFactory{ new PrefabActorFactory{m_SpriteSheetFactory,luaState,window,graphic} },
	 m_SceneActorFactory { new SceneActorFactory {m_PrefabActorFactory,window, eventhandle, graphic} }
{

}

ResourceFactoryManager::~ResourceFactoryManager()
{
	SafeDelete(m_SceneActorFactory);
	SafeDelete(m_PrefabActorFactory);
	SafeDelete(m_SpriteSheetFactory);
}

void ResourceFactoryManager::InitFactory()
{
	m_SpriteSheetFactory->ScanNoAnimatedTextureAndOutputToJson();
	m_SpriteSheetFactory->ScanSpriteSheetFromDirectory();
	m_PrefabActorFactory->InitComponentFactoryAndConstructDefaultPrefab();

	m_PrefabActorFactory->InitDigipenLogoAndTeamLogo();

	logo_timer = 10.0f;
	m_graphic->startDrawingScene();
	m_graphic->drawLogo(m_PrefabActorFactory->background_logo);
	m_graphic->drawLogo(m_PrefabActorFactory->digipen_logo);
	m_graphic->stopDrawingScene(*m_win->GetHDC());

	m_PrefabActorFactory->ScanPrefabActorDirectory();
	m_SceneActorFactory->InitLoadingScreen();


}

SceneActorFactory * ResourceFactoryManager::GetSceneActorFactory()
{
	return m_SceneActorFactory;
}

PrefabActorFactory * ResourceFactoryManager::GetPrefabActorFactory()
{
	return m_PrefabActorFactory;
}

SpriteSheetFactory * ResourceFactoryManager::GetSpriteSheetFactory()
{
	return m_SpriteSheetFactory;
}
/*
void ResourceFactoryManager::AddToScript(sol::state* luaState)
{
	luaState->new_usertype	<ResourceFactoryManager>("GetSceneActorFactory",
		"GetSceneActorFactory", &ResourceFactoryManager::GetSceneActorFactory
		);
}*/