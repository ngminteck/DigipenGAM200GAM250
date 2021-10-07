/******************************************************************************/
/*!
\file   EditorContentBrowser.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief container the ImGui of File directory and  the asset panel


All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <filesystem>


#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stl.h"

#include "TextureFactory.h"

#include "ResourceFactoryManager.h"
#include "EditorConsole.h"
#include "PrefabEditorWindow.h"
#include "CommandManager.h"


class EditorContentBrowser
{
public:
	EditorContentBrowser(EditorConsole *, ResourceFactoryManager *, CommandManager *);
	~EditorContentBrowser();
	void Init();
	void Draw();
	Profiler * m_profiler;
private:
	float thumbnail_size_x;
	float thumbnail_size_y;
	std::string search_asset;
	std::string new_name;
	std::string current_selected_directory;
	std::string resource;
	ResourceFactoryManager * m_ResourceFactoryManager;
	CommandManager * m_CommandManager;
	EditorConsole * m_console;
	
	
	std::vector <std::string> memory_asset_directory_name;
	std::unordered_map<unsigned, PrefabEditorWindow *> prefab_editor_window;
	SpriteSheetstring spritesheetdata;

	void Directory();
	void Asset(int col_max);
	void RecursiveDirectory(std::string path);
	void DestroyPrefabEditorWindow(unsigned key);

};
