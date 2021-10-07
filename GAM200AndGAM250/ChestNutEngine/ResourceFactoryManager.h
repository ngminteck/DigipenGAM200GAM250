/******************************************************************************/
/*!
\file   ResourceFactoryManager.h

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
#pragma once
#include "SpriteSheetFactory.h"
#include "PrefabActorFactory.h"
#include "SceneActorFactory.h"
#include "ShaderFactory.h"
#include "MeshFactory.h"
#include <sol.hpp>
#include "Utils.h"

#include "EventHandler.h"

#include "MainWindow.h"
#include "GraphicsSystem.h"

class ResourceFactoryManager
{
public:
	ResourceFactoryManager(MainWindow * window,sol::state* luaState, EventHandler * eventhandle , GraphicsSystem * graphic);
	~ResourceFactoryManager();
	
	void InitFactory();

	SceneActorFactory * GetSceneActorFactory();
	PrefabActorFactory * GetPrefabActorFactory();
	SpriteSheetFactory * GetSpriteSheetFactory();

	float logo_timer;

	//void AddToScript(sol::state* luaState);
private:
	SpriteSheetFactory *m_SpriteSheetFactory;
	PrefabActorFactory *m_PrefabActorFactory;
	SceneActorFactory * m_SceneActorFactory;

	MainWindow* m_win;
	GraphicsSystem * m_graphic;

};