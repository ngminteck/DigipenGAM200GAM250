/******************************************************************************/
/*!
\file  GraphicsPreprocess.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the graphic preprocess as is there any change in scene actor
container it will sort the container in layer order.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <unordered_map>
#include "SceneActor.h"
#include <vector>
#include "GraphicsContainer.h"
#include "EventHandler.h"
#include "LoadLevel.h"
class GraphicsPreprocess
{
public:
	GraphicsPreprocess(std::map<unsigned, SceneActor *> *, EventHandler *);
	~GraphicsPreprocess();

	std::vector<GraphicsContainer> & GetGraphicContainerRef();
	std::vector<GraphicsContainer> * GetGraphicContainerPtr();
	void ReInitializeSystem(LoadLevel *);
	void Init();


private:
	std::map<unsigned, SceneActor *> * m_scene_actor_container;
	std::vector<GraphicsContainer> m_graphics_container;
	EventHandler * m_ev;
	void ChildPrefabPrepare(SceneActor * sceneactor, PrefabActor* prefabactor);
	void ChildScenePrepare(SceneActor * sceneactor);
};