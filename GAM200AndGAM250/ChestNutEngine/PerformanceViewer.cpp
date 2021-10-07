/******************************************************************************/
/*!
\file  PerformanceViewer.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for PerformanceViewer class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "PerformanceViewer.h"
#include "Profiler.h"
#include <iomanip>
PerformanceViewer::PerformanceViewer(ProfileHandler * profiler)
	:m_profiler{ profiler } {}

PerformanceViewer::~PerformanceViewer(){}


void PerformanceViewer::Draw() const
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	float main_size_y = viewport->Size.y * 0.3f;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	ImGui::Begin("Performance Viewer", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ 300,300 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ viewport->Pos.x , (viewport->Pos.y + viewport->Size.y) - main_size_y }, ImGuiCond_FirstUseEver);

	ImGui::Text("FPS");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Spacing();

	ImGui::Text("Window data");
	ImGui::Text("Mouse position x: %.0f, y: %.0f", (ImGui::GetMousePos().x - ImGui::GetMainViewport()->Size.x / 2), (ImGui::GetMainViewport()->Size.y / 2 - ImGui::GetMousePos().y));
	ImGui::Text("Window width: %.0f, Window height: %.0f", ImGui::GetMainViewport()->Size.x / 2, ImGui::GetMainViewport()->Size.y / 2);

	static const unsigned maxLength = 73;
	static const unsigned maxLengthPerColumn = static_cast<unsigned>((maxLength - 3) / 3);
	double totaltime = m_profiler->m_totaltime;

	ImGui::Text("-----------------------+------------------------+-----------------------");
	for (auto elem : m_profiler->m_profilers)
	{
		std::string name = elem->system_name;
		std::string system;
		std::string time;
		std::string util;
		std::string spaces = "";
		std::string print;

		std::stringstream time_s;
		time_s << elem->duration.count() << " ms";
		time = time_s.str();

		std::stringstream util_s;
		int utilization = static_cast<int>(elem->duration.count() / totaltime * 100);
		util_s << utilization << " %";
		util = util_s.str();
		ImGui::CalcTextSize(system.c_str());
		print = name + time + util;
		ImGui::Text(name.c_str());
		ImGui::SameLine(200.f);
		ImGui::Text(time.c_str());
		ImGui::SameLine(400.f);
		ImGui::Text(util.c_str());
		util_s.str(std::string());
		ImGui::Text("-----------------------+------------------------+-----------------------");
	}

	ImGui::End();

}