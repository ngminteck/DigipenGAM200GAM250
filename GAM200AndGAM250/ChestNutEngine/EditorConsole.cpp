/******************************************************************************/
/*!
\file  EditorConsole.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EditorConsole class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "EditorConsole.h"


void EditorConsole::PrintConsole() const
{
	if (logs.size())
	{
		for (auto & log : logs)
			ImGui::Text(log.c_str());
	}
}

void EditorConsole::Draw() const
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	//float main_size_x = viewport->Size.x;
	float main_size_y = viewport->Size.y * 0.3f;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	ImGui::Begin("Console", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ 300, 300 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ viewport->Pos.x , (viewport->Pos.y + viewport->Size.y) - main_size_y }, ImGuiCond_FirstUseEver);
	
	PrintConsole();

	ImGui::End();

}

std::string FormatTimeStamp(std::string timestamp)
{
	//Remove the new line
	timestamp.pop_back();
	return std::string{ "[" + timestamp + "]: " };
	
}


void EditorConsole::WriteToConsole(const std::string & log)
{
	//Get time
	char str[26];
	time_t now = time(0);
	ctime_s(str, sizeof(str), &now);
	std::string timestamp = FormatTimeStamp(std::string{ str });
	logs.push_back(std::string{ timestamp + log });
}