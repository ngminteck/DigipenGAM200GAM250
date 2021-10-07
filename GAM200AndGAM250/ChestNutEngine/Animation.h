/******************************************************************************/
/*!
\file   Animation.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains struct definitions for Animation class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Mesh.h"
#include "Vector2D.h"
class Animation
{
public:
	Animation(Mesh* mesh, float w, float h, unsigned numFrames, std::vector<float> interval, Vector2D min = { 0,0 },Vector2D max = { 1,1 });
	~Animation();

	Mesh* GetMesh();
	glm::vec2 GetOffset();
	unsigned GetNumFrames();
	float GetFrameInterval(unsigned);

	Vector2D m_min, m_max;
private:
	Mesh * m_mesh;
	glm::vec2 m_offset;
	unsigned m_numFrames;
	std::vector<float>m_frameInterval;
};