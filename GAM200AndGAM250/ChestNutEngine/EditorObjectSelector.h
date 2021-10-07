#pragma once
/******************************************************************************/
/*!
\file  EditorObjectSelector.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EditorObjectSelector class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Factory.h"
#include "Input.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_stl.h"
#include "EditorConsole.h"
#include "ResourceFactoryManager.h"
#include "CommandManager.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

class EditorObjectSelector
{

public:
	EditorObjectSelector(InputSystem * input, EditorConsole * console, ResourceFactoryManager * resourcefactorymanager, CommandManager * cmd)
		: m_input{ input }, m_console{ console }, m_ResourceFactoryManager{ resourcefactorymanager }, m_cmd{ cmd },
		  m_profiler{new Profiler{"Object selector"}}
	{}

	~EditorObjectSelector();
	bool CheckCollisionWithMouse(const Vector2D &, const Vector2D &, const Vector2D &) const;
	void Update();
	Camera * m_cam;
	Profiler * m_profiler;

	//Keep track of the current rb component of the object selected by mouse.
	TransformComponent * objselectedTransComp;
	AABBComponent * objselectedAABBComp;
private:
	ResourceFactoryManager * m_ResourceFactoryManager;
	InputSystem		  * m_input;
	EditorConsole	  * m_console;
	CommandManager    * m_cmd;
	Vector2D			m_mouse;
	Vector2D			m_temp;
};