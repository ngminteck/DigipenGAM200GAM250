/******************************************************************************/
/*!
\file  GraphicsContainer.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the custom STL container for graphic render, sort with layer

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once


class SceneActor;
class PrefabActor;
struct GraphicsContainer
{
public:
	GraphicsContainer(bool orth_camera , unsigned layer, SceneActor * actor, PrefabActor * prefab);
	bool operator<(const GraphicsContainer& graphic_container) const;

	bool orth_cam;
	unsigned z_layer;
	SceneActor * scene_actor;
	PrefabActor * prefab_actor;
};