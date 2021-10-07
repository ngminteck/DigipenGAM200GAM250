#pragma once
#include <iostream>
struct Vector4D
{
	float x;
	float y;
	float z;
	float w;

	// ctors
	Vector4D();
	Vector4D(float x, float y, float z, float w);

	// utils functions
	void        set(float x, float y, float z, float w);
	float       length() const;                         
	float       distance(const Vector4D& vec) const;     // distance between two vectors
	Vector4D&   normalize();							 // normalize vector
	float       dot(const Vector4D& vec) const;          // dot product

	// operators
	Vector4D    operator-() const;						 // unary operator (negate)
	Vector4D    operator+(const Vector4D& rhs) const;    // add rhs
	Vector4D    operator-(const Vector4D& rhs) const;    // subtract rhs
	Vector4D&   operator+=(const Vector4D& rhs);         // add rhs and update this object
	Vector4D&   operator-=(const Vector4D& rhs);         // subtract rhs and update this object
	Vector4D    operator*(const float scale) const;		 // scale
	Vector4D    operator*(const Vector4D& rhs) const;    // multiply each element
	Vector4D&   operator*=(const float scale);			 // scale and update this object
	Vector4D&   operator*=(const Vector4D& rhs);         // multiply each element and update this object
	Vector4D    operator/(const float scale) const;		 // inverse scale
	Vector4D&   operator/=(const float scale);			 // scale and update this object
	bool		operator==(const Vector4D& rhs) const;   // exact compare, no epsilon
	bool		operator!=(const Vector4D& rhs) const;   // exact compare, no epsilon
	bool		operator<(const Vector4D& rhs) const;    // comparison for sort
	float		operator[](int index) const;             // subscript operator v[0], v[1]
	float&		operator[](int index);                   // subscript operator v[0], v[1]

	friend Vector4D operator*(const float a, const Vector4D vec);
	friend std::ostream& operator<<(std::ostream& os, const Vector4D& vec);
};