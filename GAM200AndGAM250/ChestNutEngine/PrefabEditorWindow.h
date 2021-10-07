/******************************************************************************/
/*!
\file   PrefabEditorWindow.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	each prefab will have this imgui window editor to edit the prefab itself

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "PrefabActor.h"
#include "ResourceFactoryManager.h"
#include "CommandManager.h"
#include "ValueChange.h"

#include "imgui.h"
#include "imgui_stl.h"
#include "imgui_internal.h"

class PrefabEditorWindow
{
public:
	PrefabEditorWindow(PrefabActor * prefabactor, ResourceFactoryManager * resourcemanager, CommandManager * command_manager);
	~PrefabEditorWindow();

	bool GetPrefabEditorWindowActive() const;
	void SetPrefabEditorWindowActive(bool value);
	void SetPrefabActorToNullBeforeDestroyWindow();
	PrefabActor * GetPrefabActorInfor();
	void Draw();
private:
	PrefabActor * prefab_actor;
	ResourceFactoryManager * resource;
	CommandManager * m_cmd;
	EventHandler   * m_ev;

	bool prefab_editor_window_active;
	bool prefab_editor_check_change;
	bool moving;
	bool deleting;

	enum BoxType : int
	{
		TEXTBOX = 0,
		INPUTBOX = 1,
		SLIDER = 2,
		SLIDERROTATE = 3
	};

	void PrefabWorldOutliner();
	void PrefabDetails();
	void ChildPrefabWorldOutliner(std::unordered_map<unsigned, PrefabActor *> child_object);
	void ChildPrefabDetails(std::unordered_map<unsigned, PrefabActor *> & child_object);
	void PrintComponent(const std::string & label , Component * comp);
	template<typename T> void PrintComponentData(const std::string & label, Component * comp);
	void PrintSpriteComponentData(const std::string & label, Component * comp);
	void PrintScriptComponentData(const std::string & label, Component * comp);
	void PrintCameraComponentData(const std::string & label, Component * comp);
	void PrintButtonComponentData(const std::string & label, Component * comp);
	void PrintComponentDataInterface(const std::string & label, const std::string & variable, TypeErasedObj & variable_type);

	void PrintString(const std::string & inputLabel, const std::string & name, std::string * str);
	void PrintInt(const std::string & inputLabel, int * value, BoxType box);
	void PrintFloat(const std::string & inputLabel, float * value, BoxType box);

};

