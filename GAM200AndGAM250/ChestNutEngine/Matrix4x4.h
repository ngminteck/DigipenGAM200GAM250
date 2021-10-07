#pragma once
#include "Vector4D.h"
#include "Vector3D.h"
const float DEG2RAD = 3.141593f / 180.0f;
const float RAD2DEG = 180.0f / 3.141593f;
const float EPSILON = 0.00001f;


class Matrix4x4
{
public:
	// constructors
	Matrix4x4();  // init with identity
	Matrix4x4(const float src[16]);
	Matrix4x4(float m00, float m01, float m02, float m03, // 1st column
			  float m04, float m05, float m06, float m07, // 2nd column
			  float m08, float m09, float m10, float m11, // 3rd column
			  float m12, float m13, float m14, float m15);// 4th column

	void        set(const float src[16]);
	void        set(float m00, float m01, float m02, float m03, // 1st column
					float m04, float m05, float m06, float m07, // 2nd column
					float m08, float m09, float m10, float m11, // 3rd column
					float m12, float m13, float m14, float m15);// 4th column
	const float* get() const;

	Matrix4x4&    identity();
	// transform matrix
	Matrix4x4&    translate(float x, float y, float z);   // translation by (x,y,z)
	Matrix4x4&    translate(const Vector3D& v);            //
	Matrix4x4&    rotate(float angle, const Vector3D& axis); // rotate angle(degree) along the given axix
	Matrix4x4&    rotate(float angle, float x, float y, float z);
	Matrix4x4&    rotateX(float angle);                   // rotate on X-axis with degree
	Matrix4x4&    rotateY(float angle);                   // rotate on Y-axis with degree
	Matrix4x4&    rotateZ(float angle);                   // rotate on Z-axis with degree
	Matrix4x4&    scale(float scale);                     // uniform scale
	Matrix4x4&    scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis

	// operators
	Matrix4x4    operator+(const Matrix4x4& rhs) const;    // add rhs
	Matrix4x4    operator-(const Matrix4x4& rhs) const;    // subtract rhs
	Matrix4x4&   operator+=(const Matrix4x4& rhs);         // add rhs and update this object
	Matrix4x4&   operator-=(const Matrix4x4& rhs);         // subtract rhs and update this object
	Vector4D     operator*(const Vector4D& rhs) const;    // multiplication: v' = M * v
	Vector3D     operator*(const Vector3D& rhs) const;    // multiplication: v' = M * v
	Matrix4x4    operator*(const Matrix4x4& rhs) const;    // multiplication: M3 = M1 * M2
	Matrix4x4&   operator*=(const Matrix4x4& rhs);         // multiplication: M1' = M1 * M2
	bool         operator==(const Matrix4x4& rhs) const;   // exact compare, no epsilon
	bool         operator!=(const Matrix4x4& rhs) const;   // exact compare, no epsilon
	float        operator[](int index) const;            // subscript operator v[0], v[1]
	float&       operator[](int index);                  // subscript operator v[0], v[1]

	// friends functions
	friend Matrix4x4 operator-(const Matrix4x4& m);                       // unary operator (-)
	friend Matrix4x4 operator*(float scalar, const Matrix4x4& m);         // pre-multiplication
	friend Vector3D operator*(const Vector3D& vec, const Matrix4x4& m);   // pre-multiplication
	friend Vector4D operator*(const Vector4D& vec, const Matrix4x4& m);   // pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& m);
private:
	float m[16];
};