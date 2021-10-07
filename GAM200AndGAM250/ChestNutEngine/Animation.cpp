/******************************************************************************/
/*!
\file   Animation.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Animation

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Animation.h"

Animation::Animation(Mesh* mesh, float w, float h, unsigned numFrames, std::vector<float> interval,Vector2D min,Vector2D max)
	:m_mesh{mesh}, m_offset{w,h},m_min{min},m_max{max},
	m_numFrames{numFrames}, m_frameInterval{interval}
{

}

Animation::~Animation()
{
	delete m_mesh;
}

Mesh* Animation::GetMesh()
{
	return m_mesh;
}

glm::vec2 Animation::GetOffset()
{
	return m_offset;
}
unsigned Animation::GetNumFrames()
{
	return m_numFrames;
}
float Animation::GetFrameInterval(unsigned n)
{
	if (m_frameInterval.size()-1 < n)
		n = 0;
	return m_frameInterval[n];
}

	