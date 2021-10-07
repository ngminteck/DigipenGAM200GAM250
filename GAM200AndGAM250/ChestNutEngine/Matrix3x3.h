/******************************************************************************/
/*!
\file   Matrix3x3.h

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
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

class Matrix3x3
{
public:
	// constructors

	// default ctor creates zero matrix
	Matrix3x3();
	// copy ctor
	Matrix3x3(const Matrix3x3& m);
	// build matrix
	Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
	// construct a scale matrix
	Matrix3x3(float sx, float sy, float sz);
	// construct a translation matrix
	Matrix3x3(float tx, float ty);
	// construct a rotational matrix
	Matrix3x3(float rad);

	// get determinant of matrix
	float Determinant();

	// overloaded operator

	// assignment operator
	Matrix3x3& operator=(const Matrix3x3& rhs);
	// stream operator
	friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& mtx);

	// comparison operator
	friend inline bool operator==(const Matrix3x3& lhs, const Matrix3x3& rhs);
	friend inline bool operator!=(const Matrix3x3& lhs, const Matrix3x3& rhs);

	// arithmetic operator
	Matrix3x3& operator+=(const Matrix3x3& rhs);
	Matrix3x3& operator*=(const Matrix3x3& rhs);
	Matrix3x3& operator*=(const float val);

	// arithmetic operator
	friend Matrix3x3 operator*(const Matrix3x3& mtx1, const Matrix3x3& mtx2);
	friend Matrix3x3 operator+(const Matrix3x3& mtx1, const Matrix3x3& mtx2);
	friend Matrix3x3 operator-(const Matrix3x3& mtx1, const Matrix3x3& mtx2);
	friend Matrix3x3 operator*(const Matrix3x3& mtx1, const float val);
	friend Matrix3x3 operator*(const float val, const Matrix3x3& mtx1);

	friend void Scale(Matrix3x3 & dest, const Matrix3x3 & src1, const Matrix3x3& mtx3x3);
	friend void Translate(Matrix3x3 & dest, const Matrix3x3 & src1, const Matrix3x3 & mtx3x3);
	friend void Rotate(Matrix3x3 & dest, const Matrix3x3 & src1, const Matrix3x3& mtx3x3);

	std::vector< std::vector<float> > GetMatrix() const;
private:
		std::vector< std::vector<float> > m_;
};

// constants
const Matrix3x3 IdentityMatrix = { 1, 0, 0,
					    	       0, 1, 0, 
					    	       0, 0, 1 };
const Matrix3x3 ZeroMatrix = { 0, 0, 0,
                               0, 0, 0,
                               0, 0, 0 };

