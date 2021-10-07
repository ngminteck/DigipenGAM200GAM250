/******************************************************************************/
/*!
\file   Matrix3x3.cpp

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of Vector2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Matrix3x3.h"

// default ctor creates zero matrix
Matrix3x3::Matrix3x3()
	:m_(3, std::vector<float>(3))
{
	m_[0] = { 1, 0, 0 };
	m_[1] = { 0, 1, 0 };
	m_[2] = { 0, 0, 1 };
}

// copy ctor
Matrix3x3::Matrix3x3(const Matrix3x3 & m)
	:m_(3, std::vector<float>(3))
{
	m_[0] = { m.m_[0] };
	m_[1] = { m.m_[1] };
	m_[2] = { m.m_[2] };
}

// build matrix
Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
	:m_(3, std::vector<float>(3))
{
	m_[0] = { m00, m01, m02 };
	m_[1] = { m10, m11, m12 };
	m_[2] = { m20, m21, m22 };
}

// construct a scale matrix
Matrix3x3::Matrix3x3(float sx, float sy, float sz)
	:m_(3, std::vector<float>(3))
{
	m_[0] = { sx, 0, 0 };
	m_[1] = { 0, sy, 0 };
	m_[2] = { 0, 0, sz };
}

// construct a translation matrix
Matrix3x3::Matrix3x3(float tx, float ty)
	:m_(3, std::vector<float>(3))
{
	m_[0] = { 1, 0, tx };
	m_[1] = { 0, 1, ty };
	m_[2] = { 0, 0, 1 };
}

// construct a rotational matrix
Matrix3x3::Matrix3x3(float rad)
	:m_(3, std::vector<float>(3))
{
	m_[0] = { cosf(rad), -sinf(rad), 0 };
	m_[1] = { sinf(rad),  cosf(rad), 0 };
	m_[2] = { 0,        0,         1 };
}

// get determinant of matrix
float Matrix3x3::Determinant()
{
	return m_[0][0] * (m_[1][1] * m_[2][2] - m_[1][2] * m_[2][1]) -
		m_[0][1] * (m_[1][0] * m_[2][2] - m_[1][2] * m_[2][0]) +
		m_[0][2] * (m_[1][0] * m_[2][1] - m_[1][1] * m_[2][0]);
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
{
	m_[0] = { rhs.m_[0] };
	m_[1] = { rhs.m_[1] };
	m_[2] = { rhs.m_[2] };
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Matrix3x3& mtx)
{
	os << mtx.m_[0][0] << " " << mtx.m_[0][1] << " " << mtx.m_[0][2] << std::endl;
	os << mtx.m_[1][0] << " " << mtx.m_[1][1] << " " << mtx.m_[1][2] << std::endl;
	os << mtx.m_[2][0] << " " << mtx.m_[2][1] << " " << mtx.m_[2][2] << std::endl;
	return os;
}

inline bool operator==(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	return lhs.m_ == rhs.m_;
}

inline bool operator!=(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	return !(lhs == rhs);
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& rhs)
{
	m_[0][0] += rhs.m_[0][0]; m_[0][1] += rhs.m_[0][1]; m_[0][2] += rhs.m_[0][2];
	m_[1][0] += rhs.m_[1][0]; m_[1][1] += rhs.m_[1][1]; m_[1][2] += rhs.m_[1][2];
	m_[2][0] += rhs.m_[2][0]; m_[2][1] += rhs.m_[2][1]; m_[2][2] += rhs.m_[2][2];
	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs)
{
	float tmp00 = m_[0][0] * rhs.m_[0][0] + m_[0][1] * rhs.m_[1][0] + m_[0][2] * rhs.m_[2][0];
	float tmp01 = m_[0][0] * rhs.m_[0][1] + m_[0][1] * rhs.m_[1][1] + m_[0][2] * rhs.m_[2][1];
	float tmp02 = m_[0][0] * rhs.m_[0][2] + m_[0][1] * rhs.m_[1][2] + m_[0][2] * rhs.m_[2][2];
	float tmp10 = m_[1][0] * rhs.m_[0][0] + m_[1][1] * rhs.m_[1][0] + m_[1][2] * rhs.m_[2][0];
	float tmp11 = m_[1][0] * rhs.m_[0][1] + m_[1][1] * rhs.m_[1][1] + m_[1][2] * rhs.m_[2][1];
	float tmp12 = m_[1][0] * rhs.m_[0][2] + m_[1][1] * rhs.m_[1][2] + m_[1][2] * rhs.m_[2][2];
	float tmp20 = m_[2][0] * rhs.m_[0][0] + m_[2][1] * rhs.m_[1][0] + m_[2][2] * rhs.m_[2][0];
	float tmp21 = m_[2][0] * rhs.m_[0][1] + m_[2][1] * rhs.m_[1][1] + m_[2][2] * rhs.m_[2][1];
	float tmp22 = m_[2][0] * rhs.m_[0][2] + m_[2][1] * rhs.m_[1][2] + m_[2][2] * rhs.m_[2][2];
	m_[0][0] = tmp00; m_[0][1] = tmp01; m_[0][2] = tmp02;
	m_[1][0] = tmp10; m_[1][1] = tmp11; m_[1][2] = tmp12;
	m_[2][0] = tmp20; m_[2][1] = tmp21; m_[2][2] = tmp22;
	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const float val)
{
	m_[0][0] *= val; m_[0][1] *= val; m_[0][2] *= val;
	m_[1][0] *= val; m_[1][1] *= val; m_[1][2] *= val;
	m_[2][0] *= val; m_[2][1] *= val; m_[2][2] *= val;
	return *this;
}

std::vector< std::vector<float> > Matrix3x3::GetMatrix() const
{
	return m_;
}

Matrix3x3 operator*(const Matrix3x3& mtx1, const Matrix3x3& mtx2)
{
	Matrix3x3 ret;
	ret.m_[0][0] = mtx1.m_[0][0] * mtx2.m_[0][0] + mtx1.m_[0][1] * mtx2.m_[1][0] + mtx1.m_[0][2] * mtx2.m_[2][0];
	ret.m_[0][1] = mtx1.m_[0][0] * mtx2.m_[0][1] + mtx1.m_[0][1] * mtx2.m_[1][1] + mtx1.m_[0][2] * mtx2.m_[2][1];
	ret.m_[0][2] = mtx1.m_[0][0] * mtx2.m_[0][2] + mtx1.m_[0][1] * mtx2.m_[1][2] + mtx1.m_[0][2] * mtx2.m_[2][2];
	ret.m_[1][0] = mtx1.m_[1][0] * mtx2.m_[0][0] + mtx1.m_[1][1] * mtx2.m_[1][0] + mtx1.m_[1][2] * mtx2.m_[2][0];
	ret.m_[1][1] = mtx1.m_[1][0] * mtx2.m_[0][1] + mtx1.m_[1][1] * mtx2.m_[1][1] + mtx1.m_[1][2] * mtx2.m_[2][1];
	ret.m_[1][2] = mtx1.m_[1][0] * mtx2.m_[0][2] + mtx1.m_[1][1] * mtx2.m_[1][2] + mtx1.m_[1][2] * mtx2.m_[2][2];
	ret.m_[2][0] = mtx1.m_[2][0] * mtx2.m_[0][0] + mtx1.m_[2][1] * mtx2.m_[1][0] + mtx1.m_[2][2] * mtx2.m_[2][0];
	ret.m_[2][1] = mtx1.m_[2][0] * mtx2.m_[0][1] + mtx1.m_[2][1] * mtx2.m_[1][1] + mtx1.m_[2][2] * mtx2.m_[2][1];
	ret.m_[2][2] = mtx1.m_[2][0] * mtx2.m_[0][2] + mtx1.m_[2][1] * mtx2.m_[1][2] + mtx1.m_[2][2] * mtx2.m_[2][2];
	return ret;
}

Matrix3x3 operator+(const Matrix3x3& mtx1, const Matrix3x3& mtx2)
{
	Matrix3x3 ret;
	ret.m_[0][0] = mtx1.m_[0][0] + mtx2.m_[0][0]; ret.m_[0][1] = mtx1.m_[0][1] + mtx2.m_[0][1]; ret.m_[0][2] = mtx1.m_[0][2] + mtx2.m_[0][2];
	ret.m_[1][0] = mtx1.m_[1][0] + mtx2.m_[1][0]; ret.m_[1][1] = mtx1.m_[1][1] + mtx2.m_[1][1]; ret.m_[1][2] = mtx1.m_[1][2] + mtx2.m_[1][2];
	ret.m_[2][0] = mtx1.m_[2][0] + mtx2.m_[2][0]; ret.m_[2][1] = mtx1.m_[2][1] + mtx2.m_[2][1]; ret.m_[2][2] = mtx1.m_[2][2] + mtx2.m_[2][2];
	return ret;
}

Matrix3x3 operator-(const Matrix3x3& mtx1, const Matrix3x3& mtx2)
{
	Matrix3x3 ret;
	ret.m_[0][0] = mtx1.m_[0][0] - mtx2.m_[0][0]; ret.m_[0][1] = mtx1.m_[0][1] - mtx2.m_[0][1]; ret.m_[0][2] = mtx1.m_[0][2] - mtx2.m_[0][2];
	ret.m_[1][0] = mtx1.m_[1][0] - mtx2.m_[1][0]; ret.m_[1][1] = mtx1.m_[1][1] - mtx2.m_[1][1]; ret.m_[1][2] = mtx1.m_[1][2] - mtx2.m_[1][2];
	ret.m_[2][0] = mtx1.m_[2][0] - mtx2.m_[2][0]; ret.m_[2][1] = mtx1.m_[2][1] - mtx2.m_[2][1]; ret.m_[2][2] = mtx1.m_[2][2] - mtx2.m_[2][2];
	return ret;
}

Matrix3x3 operator*(const Matrix3x3& mtx1, const float val)
{
	Matrix3x3 ret;
	ret.m_[0][0] = mtx1.m_[0][0] * val; ret.m_[0][1] = mtx1.m_[0][1] * val; ret.m_[0][2] = mtx1.m_[0][2] * val;
	ret.m_[1][0] = mtx1.m_[1][0] * val; ret.m_[1][1] = mtx1.m_[1][1] * val; ret.m_[1][2] = mtx1.m_[1][2] * val;
	ret.m_[2][0] = mtx1.m_[2][0] * val; ret.m_[2][1] = mtx1.m_[2][1] * val; ret.m_[2][2] = mtx1.m_[2][2] * val;
	return ret;
}

Matrix3x3 operator*(const float val, const Matrix3x3& mtx1)
{
	Matrix3x3 ret;
	ret.m_[0][0] = mtx1.m_[0][0] * val; ret.m_[0][1] = mtx1.m_[0][1] * val; ret.m_[0][2] = mtx1.m_[0][2] * val;
	ret.m_[1][0] = mtx1.m_[1][0] * val; ret.m_[1][1] = mtx1.m_[1][1] * val; ret.m_[1][2] = mtx1.m_[1][2] * val;
	ret.m_[2][0] = mtx1.m_[2][0] * val; ret.m_[2][1] = mtx1.m_[2][1] * val; ret.m_[2][2] = mtx1.m_[2][2] * val;
	return ret;
}

void Scale(Matrix3x3 & dest, const Matrix3x3 & src1, const Matrix3x3& mtx3x3)
{
	dest = src1 * mtx3x3;
}

void Translate(Matrix3x3 & dest, const Matrix3x3 & src1, const Matrix3x3 & mtx3x3)
{
	dest = src1 * mtx3x3;
}

void Rotate(Matrix3x3 & dest, const Matrix3x3 & src1, const Matrix3x3& mtx3x3)
{
	dest = src1 * mtx3x3;
}

