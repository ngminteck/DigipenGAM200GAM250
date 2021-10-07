/******************************************************************************/
/*!
\file   Vector2D.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Vector2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include <iostream>
#include <utility>
#include <cmath>
#include <tuple>
#include <sol.hpp>
class Vector2D
{
public:

	// constructors
	Vector2D();
	Vector2D(const float& x, const float& y);
	~Vector2D();
	
	// Getter/Setter
	float GetX() const;
	float GetY() const;
	void SetX(float);
	void SetY(float);

	// assignment operator
	Vector2D& operator=(const Vector2D vec);

	// stream operator
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

	// comparison operator
	friend bool operator==(const Vector2D& lhs, const Vector2D& rhs);
	friend bool operator!=(const Vector2D& lhs, const Vector2D& rhs);
	
	// arithmetic compound operators
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	// arithmetic compound operators for scalar
	Vector2D& operator+=(const float& val);
	Vector2D& operator-=(const float& val);
	Vector2D& operator*=(const float& val);
	Vector2D& operator/=(const float& val);
	
	// unary operator
	Vector2D operator-() const;

	static void AddToScript(sol::state * luaState);

	float x;
	float y;
} typedef Point2D;

// arithmetic operators
Vector2D operator+(Vector2D lhs, const Vector2D& rhs);
Vector2D operator-(Vector2D lhs, const Vector2D& rhs);
Vector2D operator*(Vector2D lhs, const Vector2D& rhs);
Vector2D operator/(Vector2D lhs, const Vector2D& rhs);

// arithmetic operators for scalar
Vector2D operator+(Vector2D lhs, const float& rhs);
Vector2D operator+(const float& lhs, Vector2D rhs);

Vector2D operator-(Vector2D lhs, const float& rhs);
Vector2D operator-(const float& lhs, Vector2D rhs);

Vector2D operator*(Vector2D lhs, const float& rhs);
Vector2D operator*(const float& lhs, Vector2D rhs);

Vector2D operator/(Vector2D lhs, const float& rhs);
Vector2D operator/(const float& lhs, Vector2D rhs);

// utility functions
float Vector2DDotProduct(const Vector2D& first, const Vector2D& second);

float Vector2DLength(const Vector2D& vec);

float Vector2DLengthSquared(const Vector2D& vec);

// rotates 90degrees counter clock-wise
Vector2D Vector2DNormal(const Vector2D& vec);

void Vector2DNormalize(Vector2D& vec);

// gets the longer vector
Vector2D& Vector2DMax(Vector2D& first, Vector2D& second);

// gets the shorter vector
Vector2D& Vector2DMin(Vector2D& first, Vector2D& second);

// gets the acute angle of two vectors
float Vector2DAngle(const Vector2D& first, const Vector2D& second);