/******************************************************************************/
/*!
\file   Shapes.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the implementation of Shapes

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Shapes.h"

Triangle::Triangle(Vector2D p0, Vector2D p1, Vector2D p2)
	: m_vertices{ p0, p1, p2 }
{ }

bool Triangle::PointInTriangle(Vector2D p) const
{
	Vector2D p0 = m_vertices[0]; Vector2D p1 = m_vertices[1]; Vector2D p2 = m_vertices[2];
	float s = p0.y*p2.x - p0.x*p2.y + (p2.y - p0.y)*p.x + (p0.x - p2.x)*p.y;
	float t = p0.x*p1.y - p0.y*p1.x + (p0.y - p1.y)*p.x + (p1.x - p0.x)*p.y;
	if ((s < 0) != (t < 0))
		return false;
	float Area = -p1.y*p2.x + p0.y*(p2.x - p1.x) + p0.x*(p1.y - p2.y) + p1.x*p2.y;
	if (Area < 0.0f)
	{
		s = -s;
		t = -t;
		Area = -Area;
	}
	return s > 0 && t > 0 && (s + t) <= Area;
}

// top left corner is p0
Rect::Rect(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3)
	:m_vertices{ p0, p1, p2, p3 }
{ }