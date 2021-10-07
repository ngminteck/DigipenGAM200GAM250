/******************************************************************************/
/*!
\file   Shapes.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Shapes

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Vector2D.h"
#include <array>

class Triangle
{
public:
	Triangle() { }
	Triangle(Vector2D p0, Vector2D p1, Vector2D p2);

	bool PointInTriangle(Vector2D p) const;

	std::array<Vector2D, 3> m_vertices;
};

class Rect
{
public:
	Rect() { }
	~Rect() { }
	// top left corner is p0
	Rect(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3);


    std::array<Vector2D, 4> m_vertices;
};
