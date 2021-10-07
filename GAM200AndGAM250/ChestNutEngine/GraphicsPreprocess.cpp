/******************************************************************************/
/*!
\file  GraphicsPreprocess.h

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

#include "GraphicsPreprocess.h"

GraphicsPreprocess::GraphicsPreprocess(std::map<unsigned, SceneActor *> * scene_actor_container, EventHandler * ev)
	:m_scene_actor_container{scene_actor_container}, m_ev{ev}
{
	m_ev->SubscribeEvent(this, &GraphicsPreprocess::ReInitializeSystem);
}


GraphicsPreprocess::~GraphicsPreprocess()
{
	m_graphics_container.clear();
}

std::vector<GraphicsContainer>& GraphicsPreprocess::GetGraphicContainerRef()
{
	return m_graphics_container;
}

std::vector<GraphicsContainer> * GraphicsPreprocess::GetGraphicContainerPtr()
{
	return &m_graphics_container;
}

void GraphicsPreprocess::ReInitializeSystem(LoadLevel * )
{
	Init();
}

void GraphicsPreprocess::Init()
{
	m_graphics_container.clear();

	if (m_scene_actor_container->size() > 0)
	{
		std::cout << "graphics preprocess test begin" << std::endl;
		for (auto & elem : *m_scene_actor_container)
		{
			auto scene = elem.second;

			auto prefab = scene->GetPrefabActor();

			auto comp = prefab->GetComponentsPtr();

			if (comp->find(ComponentType::SpriteComponent) != comp->end())
			{
				auto sprite = dynamic_cast<SpriteComponent*>((*comp)[ComponentType::SpriteComponent]);
				if (sprite)
				{
					m_graphics_container.push_back(GraphicsContainer(sprite->IsOrthCam(), sprite->GetLayer(), scene, prefab));
					//std::cout << prefab->GetPrefabActorName() << "-" << sprite->IsOrthCam() << std::endl;
				}
			}
			// check prefab have any child layer
			if (prefab->GetChildPrefabActors().size() > 0)
			{
				for (auto child_prefab : prefab->GetChildPrefabActors())
				{
					ChildPrefabPrepare(scene, child_prefab.second);
				}
			}
			// check game instance have any child layer
			if (elem.second->GetChildSceneActors().size() > 0)
			{
				for (auto child_scene : scene->GetChildSceneActors())
				{
					ChildScenePrepare(child_scene.second);
				}
			}
		}
	}
	std::sort(m_graphics_container.begin(), m_graphics_container.end());
}

void GraphicsPreprocess::ChildPrefabPrepare(SceneActor * sceneactor, PrefabActor* prefabactor)
{
	auto prefab = prefabactor;

	auto comp = prefab->GetComponentsPtr();

	if ((*comp).find(ComponentType::SpriteComponent) != (*comp).end())
	{
		auto sprite = dynamic_cast<SpriteComponent*>((*comp).find(ComponentType::SpriteComponent)->second);
		m_graphics_container.push_back(GraphicsContainer(sprite->IsOrthCam(), sprite->GetLayer(), sceneactor, prefab));
	}
	// check prefab have any child layer
	if (prefab->GetChildPrefabActors().size() > 0)
	{
		for (auto child_prefab : prefab->GetChildPrefabActors())
		{
			ChildPrefabPrepare(sceneactor, child_prefab.second);
		}
	}
}

void GraphicsPreprocess::ChildScenePrepare(SceneActor * sceneactor)
{
	auto scene = sceneactor;

	auto prefab = scene->GetPrefabActor();

	auto comp = prefab->GetComponentsPtr();

	if ((*comp).find(ComponentType::SpriteComponent) != (*comp).end())
	{
		auto sprite = dynamic_cast<SpriteComponent*>((*comp).find(ComponentType::SpriteComponent)->second);
		m_graphics_container.push_back(GraphicsContainer(sprite->IsOrthCam(), sprite->GetLayer(), scene, prefab));
	}


	// check prefab have any child layer
	if (prefab->GetChildPrefabActors().size() > 0)
	{
		for (auto child_prefab : prefab->GetChildPrefabActors())
		{
			ChildPrefabPrepare(scene, child_prefab.second);
		}
	}
	// check game instance have any child layer
	if (scene->GetChildSceneActors().size() > 0)
	{
		for (auto child_scene : scene->GetChildSceneActors())
		{
			ChildScenePrepare(child_scene.second);
		}
	}
}
