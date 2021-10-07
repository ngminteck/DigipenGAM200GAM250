/******************************************************************************/
/*!
\file  GraphicsContainer.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the custom STL container for graphic render, sort with layer

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "GraphicsContainer.h"
#include "SceneActor.h"
GraphicsContainer::GraphicsContainer(bool orth_camera, unsigned layer, SceneActor * actor, PrefabActor * prefab)
:orth_cam { orth_camera },z_layer{layer}, scene_actor{actor}, prefab_actor{prefab}
{ 


}

bool GraphicsContainer::operator < (const GraphicsContainer& graphic_container) const
{
	if(orth_cam == graphic_container.orth_cam)
		return (z_layer < graphic_container.z_layer);

	return (orth_cam < graphic_container.orth_cam);
}