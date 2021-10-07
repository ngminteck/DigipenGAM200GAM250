#pragma once
/******************************************************************************/
/*!
\file  EditorConsole.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EditorConsole class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include <string>
#include <vector>
#include <time.h>
#include "imgui.h"
#include "imgui_stl.h"

class EditorConsole
{
public:
	EditorConsole() {}
	~EditorConsole() {}

	void PrintConsole() const;
	void WriteToConsole(const std::string &);
	void Draw() const;
private:
	std::vector<std::string> logs;
};