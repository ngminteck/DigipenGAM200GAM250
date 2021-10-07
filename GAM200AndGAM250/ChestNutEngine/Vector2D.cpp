/******************************************************************************/
/*!
\file   Vector2D.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Vector2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "Vector2D.h"

// constructors

Vector2D::Vector2D()
	: x(0), y(0)
{ }

Vector2D::Vector2D(const float & x, const float & y)
	: x(x), y(y)
{ }

Vector2D::~Vector2D()
{ }

float Vector2D::GetX() const
{
	return x;
}

float Vector2D::GetY() const
{
	return y;
}

void Vector2D::SetX(float val)
{
	x = val;
}

void Vector2D::SetY(float val)
{
	y = val;
}

Vector2D& Vector2D::operator=(const Vector2D vec) 
{
	x = vec.x;
	y = vec.y;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec)
{
	os << vec.x << ", " << vec.y;
	return os; 
}

bool operator==(const Vector2D& lhs, const Vector2D& rhs)
{
	return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
}

bool operator!=(const Vector2D& lhs, const Vector2D& rhs)
{
	return !(lhs == rhs);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	x += vec.x;
	y += vec.y;
	return *this; 
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{ 
	x -= vec.x;
	y -= vec.y; 
	return *this; 
}

Vector2D& Vector2D::operator*=(const Vector2D& vec) 
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	x /= vec.x;
	y /= vec.y;
	return *this;
}

Vector2D& Vector2D::operator+=(const float& val) 
{
	x += val;
	y += val; 
	return *this;
}

Vector2D& Vector2D::operator-=(const float& val)
{
	x -= val;
	y -= val; 
	return *this;
}

Vector2D& Vector2D::operator*=(const float& val) 
{
	x *= val;
	y *= val;
	return *this;
}

Vector2D& Vector2D::operator/=(const float& val) 
{
	x /= val;
	y /= val; 
	return *this;
}

Vector2D Vector2D::operator-() const 
{
	return Vector2D(-x, -y);
}

Vector2D operator+(Vector2D lhs, const Vector2D& rhs) 
{
	return lhs += rhs;
}

Vector2D operator-(Vector2D lhs, const Vector2D& rhs) 
{
	return lhs -= rhs;
}

Vector2D operator*(Vector2D lhs, const Vector2D& rhs) 
{
	return lhs *= rhs;
}

Vector2D operator/(Vector2D lhs, const Vector2D& rhs) 
{
	return lhs /= rhs;
}

Vector2D operator+(Vector2D lhs, const float& rhs)
{
	lhs += rhs;
	return lhs; 
}

Vector2D operator+(const float& lhs, Vector2D rhs) 
{
	rhs += lhs;
	return rhs;
}

Vector2D operator-(Vector2D lhs, const float& rhs) 
{
	lhs -= rhs;
	return lhs;
}

Vector2D operator-(const float& lhs, Vector2D rhs) 
{
	rhs -= lhs;
	return rhs;
}

Vector2D operator*(Vector2D lhs, const float& rhs) 
{
	lhs *= rhs;
	return lhs;
}

Vector2D operator*(const float& lhs, Vector2D rhs) 
{
	rhs *= lhs; 
	return rhs;
}

Vector2D operator/(Vector2D lhs, const float& rhs) 
{
	lhs /= rhs;
	return lhs;
}

Vector2D operator/(const float& lhs, Vector2D rhs) 
{
	rhs /= lhs; 
	return rhs; 
}

float Vector2DDotProduct(const Vector2D& first, const Vector2D& second)
{
	return (first.GetX()*second.GetX() + first.GetY()*second.GetY());
}

float Vector2DLength(const Vector2D& vec)
{
	return sqrt(vec.GetX()*vec.GetX() + vec.GetY()*vec.GetY());
}

float Vector2DLengthSquared(const Vector2D& vec)
{
	return (vec.GetX()*vec.GetX() + vec.GetY()*vec.GetY());
}

Vector2D Vector2DNormal(const Vector2D& vec)
{
	return Vector2D(-vec.GetY(), vec.GetX());
}

void Vector2DNormalize(Vector2D& vec)
{
	vec /= Vector2DLength(vec);
}

Vector2D& Vector2DMax(Vector2D& first, Vector2D& second)
{
	return Vector2DLengthSquared(first) >= Vector2DLengthSquared(second) ? first : second;
}

Vector2D& Vector2DMin(Vector2D& first, Vector2D& second)
{
	return Vector2DLengthSquared(first) <= Vector2DLengthSquared(second) ? first : second;
}

float Vector2DAngle(const Vector2D& first, const Vector2D& second)
{
	return acos(Vector2DDotProduct(first, second) / (Vector2DLength(first)*Vector2DLength(second)));
}

void Vector2D::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<Vector2D>("vec2",
		//Constructor
		sol::constructors<Vector2D(), Vector2D(float, float)>(),
		//Operator Overload
		sol::meta_function::addition, sol::overload(
			sol::resolve<Vector2D(Vector2D, const Vector2D&)>(&::operator+),
			sol::resolve<Vector2D(Vector2D, const float&)>(&::operator+),
			sol::resolve<Vector2D(const float&, Vector2D)>(&::operator+)),
		sol::meta_function::subtraction, sol::overload(
			sol::resolve<Vector2D(Vector2D, const Vector2D&)>(&::operator-),
			sol::resolve<Vector2D(Vector2D, const float&)>(&::operator-),
			sol::resolve<Vector2D(const float&, Vector2D)>(&::operator-)),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector2D(Vector2D, const Vector2D&)>(&::operator*),
			sol::resolve<Vector2D(Vector2D, const float&)>(&::operator*),
			sol::resolve<Vector2D(const float&, Vector2D)>(&::operator*)),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector2D(Vector2D, const Vector2D&)>(&::operator/),
			sol::resolve<Vector2D(Vector2D, const float&)>(&::operator/),
			sol::resolve<Vector2D(const float&, Vector2D)>(&::operator/)),
		sol::meta_function::unary_minus, &Vector2D::operator-,
		sol::meta_function::equal_to, &Vector2D::operator=,
		//Member Functions
		"Dot",&::Vector2DDotProduct,
		"Length",&::Vector2DLength,
		"LengthSquared",&::Vector2DLengthSquared,
		"Normalize",&::Vector2DNormalize,
		"Max",&::Vector2DMax,
		"Min",&::Vector2DMin,
		"Angle",&::Vector2DAngle,
		//Variables
		"x", &Vector2D::x,
		"y", &Vector2D::y
		);
}