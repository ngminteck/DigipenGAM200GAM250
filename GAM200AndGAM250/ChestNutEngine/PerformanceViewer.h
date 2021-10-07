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
#include "ProfileHandler.h"
#include "imgui.h"
#include "imgui_stl.h"

#include <sstream>

class PerformanceViewer
{

public:
	PerformanceViewer(ProfileHandler *);
	~PerformanceViewer();

	void Draw() const;

private:
	ProfileHandler * m_profiler;

};