/******************************************************************************/
/*!
\file   EditorPrefabEditor.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	prefab editor

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <cstring>

#include "GameObject.h"
#include "MyFunction.h"
#include "imgui.h"
#include "imgui_stl.h"

#include "EditorConsole.h"
#include "EditorObjectViewer.h"

class EditorPrefabEditor
{
public:
	EditorPrefabEditor(EditorConsole * console)
		:m_console{ console } {}

	//void DrawComponentInfo(GameObject * prefab_obj);

private:
	//void UpdateInstanceObject(GameObject * prefab_obj, Factory * factory);

	EditorConsole * m_console;
};