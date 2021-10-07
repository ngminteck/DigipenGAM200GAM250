#pragma once
#include <iostream>
#include <sol.hpp>

struct Vector3D
{
	float x;
	float y;
	float z;

	// ctors
	Vector3D();
	Vector3D(float x, float y, float z);

	// utils functions
	void        set(float x, float y, float z);
	float       length() const;                         //
	float       distance(const Vector3D& vec) const;     // distance between two vectors
	float       angle(const Vector3D& vec) const;        // angle between two vectors
	Vector3D&    normalize();                            //
	float       dot(const Vector3D& vec) const;          // dot product
	Vector3D     cross(const Vector3D& vec) const;        // cross product

	// operators
	Vector3D     operator-() const;                      // unary operator (negate)
	Vector3D     operator+(const Vector3D& rhs) const;    // add rhs
	Vector3D	 operator+(float);
	Vector3D     operator-(const Vector3D& rhs) const;    // subtract rhs
	Vector3D&    operator+=(const Vector3D& rhs);         // add rhs and update this object
	Vector3D&    operator+=(const float rhs);         // add rhs and update this object
	Vector3D&    operator-=(const Vector3D& rhs);         // subtract rhs and update this object
	Vector3D     operator*(const float scale) const;     // scale
	Vector3D     operator*(const Vector3D& rhs) const;    // multiplay each element
	Vector3D&    operator*=(const float scale);          // scale and update this object
	Vector3D&    operator*=(const Vector3D& rhs);         // product each element and update this object
	Vector3D     operator/(const float scale) const;     // inverse scale
	Vector3D&    operator/=(const float scale);          // scale and update this object
	bool        operator==(const Vector3D& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Vector3D& rhs) const;   // exact compare, no epsilon
	bool        operator<(const Vector3D& rhs) const;    // comparison for sort
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float&      operator[](int index);                  // subscript operator v[0], v[1]
	void		operator=(const Vector3D& rhs);


	friend Vector3D operator*(const float a, const Vector3D vec);
	friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec);

	static void AddToScript(sol::state * luaState);
};