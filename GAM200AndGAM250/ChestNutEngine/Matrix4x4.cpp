#include "Matrix4x4.h"
#include <iomanip>
Matrix4x4::Matrix4x4()
{
	identity();
}
Matrix4x4::Matrix4x4(const float src[16])
{
	set(src);
}
Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
	float m04, float m05, float m06, float m07,
	float m08, float m09, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	set(m00, m01, m02, m03,
		m04, m05, m06, m07,
		m08, m09, m10, m11,
		m12, m13, m14, m15);
}

void	Matrix4x4::set(const float src[16])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
	m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
	m[8] = src[8];  m[9] = src[9];  m[10] = src[10]; m[11] = src[11];
	m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
}
void	Matrix4x4::set(float m00, float m01, float m02, float m03,
					   float m04, float m05, float m06, float m07,
					   float m08, float m09, float m10, float m11,
					   float m12, float m13, float m14, float m15)
{
	m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
	m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
	m[8] = m08;  m[9] = m09;  m[10] = m10;  m[11] = m11;
	m[12] = m12;  m[13] = m13;  m[14] = m14;  m[15] = m15;
}

const float* Matrix4x4::get() const
{
	return m;
}

Matrix4x4&    Matrix4x4::identity()
{
	m[0] = m[5] = m[10] = m[15] = 1.0f;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	return *this;
}

Matrix4x4&    Matrix4x4::translate(float x, float y, float z)
{
	m[0] += m[3] * x;   m[4] += m[7] * x;   m[8] += m[11] * x;   m[12] += m[15] * x;
	m[1] += m[3] * y;   m[5] += m[7] * y;   m[9] += m[11] * y;   m[13] += m[15] * y;
	m[2] += m[3] * z;   m[6] += m[7] * z;   m[10] += m[11] * z;   m[14] += m[15] * z;

	return *this;
}
Matrix4x4&   Matrix4x4::translate(const Vector3D& v)
{
	return translate(v.x, v.y, v.z);
}
Matrix4x4&   Matrix4x4::rotate(float angle, float x, float y, float z)
{
	float c = cosf(angle * DEG2RAD);    // cosine
	float s = sinf(angle * DEG2RAD);    // sine
	float c1 = 1.0f - c;                // 1 - c
	float m0 = m[0], m4 = m[4], m8 = m[8], m12 = m[12],
		m1 = m[1], m5 = m[5], m9 = m[9], m13 = m[13],
		m2 = m[2], m6 = m[6], m10 = m[10], m14 = m[14];

	// build rotation matrix
	float r0 = x * x * c1 + c;
	float r1 = x * y * c1 + z * s;
	float r2 = x * z * c1 - y * s;
	float r4 = x * y * c1 - z * s;
	float r5 = y * y * c1 + c;
	float r6 = y * z * c1 + x * s;
	float r8 = x * z * c1 + y * s;
	float r9 = y * z * c1 - x * s;
	float r10 = z * z * c1 + c;

	// multiply rotation matrix
	m[0] = r0 * m0 + r4 * m1 + r8 * m2;
	m[1] = r1 * m0 + r5 * m1 + r9 * m2;
	m[2] = r2 * m0 + r6 * m1 + r10 * m2;
	m[4] = r0 * m4 + r4 * m5 + r8 * m6;
	m[5] = r1 * m4 + r5 * m5 + r9 * m6;
	m[6] = r2 * m4 + r6 * m5 + r10 * m6;
	m[8] = r0 * m8 + r4 * m9 + r8 * m10;
	m[9] = r1 * m8 + r5 * m9 + r9 * m10;
	m[10] = r2 * m8 + r6 * m9 + r10 * m10;
	m[12] = r0 * m12 + r4 * m13 + r8 * m14;
	m[13] = r1 * m12 + r5 * m13 + r9 * m14;
	m[14] = r2 * m12 + r6 * m13 + r10 * m14;

	return *this;
}

Matrix4x4&   Matrix4x4::rotate(float angle, const Vector3D& axis)
{
	return rotate(angle, axis.x, axis.y, axis.z);
}
Matrix4x4&   Matrix4x4::rotateX(float angle)
{
	float c = cosf(angle * DEG2RAD);
	float s = sinf(angle * DEG2RAD);
	float m1 = m[1], m2 = m[2],
		m5 = m[5], m6 = m[6],
		m9 = m[9], m10 = m[10],
		m13 = m[13], m14 = m[14];

	m[1] = m1 * c + m2 * -s;
	m[2] = m1 * s + m2 * c;
	m[5] = m5 * c + m6 * -s;
	m[6] = m5 * s + m6 * c;
	m[9] = m9 * c + m10 * -s;
	m[10] = m9 * s + m10 * c;
	m[13] = m13 * c + m14 * -s;
	m[14] = m13 * s + m14 * c;

	return *this;
}
Matrix4x4&   Matrix4x4::rotateY(float angle)
{
	float c = cosf(angle * DEG2RAD);
	float s = sinf(angle * DEG2RAD);
	float m0 = m[0], m2 = m[2],
		m4 = m[4], m6 = m[6],
		m8 = m[8], m10 = m[10],
		m12 = m[12], m14 = m[14];

	m[0] = m0 * c + m2 * s;
	m[2] = m0 * -s + m2 * c;
	m[4] = m4 * c + m6 * s;
	m[6] = m4 * -s + m6 * c;
	m[8] = m8 * c + m10 * s;
	m[10] = m8 * -s + m10 * c;
	m[12] = m12 * c + m14 * s;
	m[14] = m12 * -s + m14 * c;

	return *this;
}
Matrix4x4&   Matrix4x4::rotateZ(float angle)
{
	float c = cosf(angle * DEG2RAD);
	float s = sinf(angle * DEG2RAD);
	float m0 = m[0], m1 = m[1],
		m4 = m[4], m5 = m[5],
		m8 = m[8], m9 = m[9],
		m12 = m[12], m13 = m[13];

	m[0] = m0 * c + m1 * -s;
	m[1] = m0 * s + m1 * c;
	m[4] = m4 * c + m5 * -s;
	m[5] = m4 * s + m5 * c;
	m[8] = m8 * c + m9 * -s;
	m[9] = m8 * s + m9 * c;
	m[12] = m12 * c + m13 * -s;
	m[13] = m12 * s + m13 * c;

	return *this;
}
Matrix4x4&   Matrix4x4::scale(float s)
{
	return scale(s, s, s);
}
Matrix4x4&   Matrix4x4::scale(float sx, float sy, float sz)
{
	m[0] *= sx;   m[4] *= sx;   m[8]  *= sx;   m[12] *= sx;
	m[1] *= sy;   m[5] *= sy;   m[9]  *= sy;   m[13] *= sy;
	m[2] *= sz;   m[6] *= sz;   m[10] *= sz;   m[14] *= sz;
	return *this;
}

Matrix4x4    Matrix4x4::operator+(const Matrix4x4& rhs) const
{
	return Matrix4x4(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3],
					 m[4] + rhs[4], m[5] + rhs[5], m[6] + rhs[6], m[7] + rhs[7],
					 m[8] + rhs[8], m[9] + rhs[9], m[10] + rhs[10], m[11] + rhs[11],
					 m[12] + rhs[12], m[13] + rhs[13], m[14] + rhs[14], m[15] + rhs[15]);
}
Matrix4x4    Matrix4x4::operator-(const Matrix4x4& rhs) const
{
	return Matrix4x4(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3],
					 m[4] - rhs[4], m[5] - rhs[5], m[6] - rhs[6], m[7] - rhs[7],
					 m[8] - rhs[8], m[9] - rhs[9], m[10] - rhs[10], m[11] - rhs[11],
					 m[12] - rhs[12], m[13] - rhs[13], m[14] - rhs[14], m[15] - rhs[15]);
}
Matrix4x4&   Matrix4x4::operator+=(const Matrix4x4& rhs)
{
	m[0] += rhs[0];   m[1] += rhs[1];   m[2] += rhs[2];   m[3] += rhs[3];
	m[4] += rhs[4];   m[5] += rhs[5];   m[6] += rhs[6];   m[7] += rhs[7];
	m[8] += rhs[8];   m[9] += rhs[9];   m[10] += rhs[10];  m[11] += rhs[11];
	m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
	return *this;
}
Matrix4x4&   Matrix4x4::operator-=(const Matrix4x4& rhs)
{
	m[0] -= rhs[0];   m[1] -= rhs[1];   m[2] -= rhs[2];   m[3] -= rhs[3];
	m[4] -= rhs[4];   m[5] -= rhs[5];   m[6] -= rhs[6];   m[7] -= rhs[7];
	m[8] -= rhs[8];   m[9] -= rhs[9];   m[10] -= rhs[10];  m[11] -= rhs[11];
	m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
	return *this;
}
Vector4D     Matrix4x4::operator*(const Vector4D& rhs) const
{
	return Vector4D(m[0] * rhs.x + m[4] * rhs.y + m[8] * rhs.z + m[12] * rhs.w,
					m[1] * rhs.x + m[5] * rhs.y + m[9] * rhs.z + m[13] * rhs.w,
					m[2] * rhs.x + m[6] * rhs.y + m[10] * rhs.z + m[14] * rhs.w,
					m[3] * rhs.x + m[7] * rhs.y + m[11] * rhs.z + m[15] * rhs.w);
}
Vector3D     Matrix4x4::operator*(const Vector3D& rhs) const
{
	return Vector3D(m[0] * rhs.x + m[1] * rhs.y + m[2] * rhs.z /*+ m[12]*/,
					m[4] * rhs.x + m[5] * rhs.y + m[6] * rhs.z  /*+m[7]*/,
					m[8] * rhs.x + m[9] * rhs.y + m[10] * rhs.z /*+ m[14]*/);
}
Matrix4x4    Matrix4x4::operator*(const Matrix4x4& rhs) const
{
	return Matrix4x4(m[0] * rhs[0]  + m[4] * rhs[1]  + m[8] * rhs[2]  + m[12] * rhs[3], m[1]  * rhs[0]  + m[5] * rhs[1]  + m[9] * rhs[2]  + m[13] * rhs[3],  m[2] * rhs[0]  + m[6] * rhs[1]  + m[10] * rhs[2]  + m[14] * rhs[3],  m[3] * rhs[0]  + m[7] * rhs[1]  + m[11] * rhs[2]  + m[15] * rhs[3],
					 m[0] * rhs[4]  + m[4] * rhs[5]  + m[8] * rhs[6]  + m[12] * rhs[7], m[1]  * rhs[4]  + m[5] * rhs[5]  + m[9] * rhs[6]  + m[13] * rhs[7],  m[2] * rhs[4]  + m[6] * rhs[5]  + m[10] * rhs[6]  + m[14] * rhs[7],  m[3] * rhs[4]  + m[7] * rhs[5]  + m[11] * rhs[6]  + m[15] * rhs[7],
					 m[0] * rhs[8]  + m[4] * rhs[9]  + m[8] * rhs[10] + m[12] * rhs[11], m[1] * rhs[8]  + m[5] * rhs[9]  + m[9] * rhs[10] + m[13] * rhs[11], m[2] * rhs[8]  + m[6] * rhs[9]  + m[10] * rhs[10] + m[14] * rhs[11], m[3] * rhs[8]  + m[7] * rhs[9]  + m[11] * rhs[10] + m[15] * rhs[11],
					 m[0] * rhs[12] + m[4] * rhs[13] + m[8] * rhs[14] + m[12] * rhs[15], m[1] * rhs[12] + m[5] * rhs[13] + m[9] * rhs[14] + m[13] * rhs[15], m[2] * rhs[12] + m[6] * rhs[13] + m[10] * rhs[14] + m[14] * rhs[15], m[3] * rhs[12] + m[7] * rhs[13] + m[11] * rhs[14] + m[15] * rhs[15]);

}
Matrix4x4&   Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	*this = *this * rhs;
	return *this;
}
bool		 Matrix4x4::operator==(const Matrix4x4& n) const
{
	return (m[0] == n[0]) && (m[1] == n[1]) && (m[2] == n[2]) && (m[3] == n[3]) &&
		   (m[4] == n[4]) && (m[5] == n[5]) && (m[6] == n[6]) && (m[7] == n[7]) &&
		   (m[8] == n[8]) && (m[9] == n[9]) && (m[10] == n[10]) && (m[11] == n[11]) &&
		   (m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
}
bool		Matrix4x4::operator!=(const Matrix4x4& n) const
{
	return (m[0] != n[0]) || (m[1] != n[1]) || (m[2] != n[2]) || (m[3] != n[3]) ||
	       (m[4] != n[4]) || (m[5] != n[5]) || (m[6] != n[6]) || (m[7] != n[7]) ||
	       (m[8] != n[8]) || (m[9] != n[9]) || (m[10] != n[10]) || (m[11] != n[11]) ||
	       (m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
}
float        Matrix4x4::operator[](int index) const
{
	return m[index];
}
float&       Matrix4x4::operator[](int index)
{
	return m[index];
}

Matrix4x4 operator-(const Matrix4x4& rhs)
{
	return Matrix4x4(-rhs[0], -rhs[1], -rhs[2], -rhs[3],
					 -rhs[4], -rhs[5], -rhs[6], -rhs[7],
					 -rhs[8], -rhs[9], -rhs[10], -rhs[11],
					 -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}
Matrix4x4 operator*(float s, const Matrix4x4& rhs)
{
	return Matrix4x4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3],
				     s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7],
					 s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11],
					 s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
}
Vector3D operator*(const Vector3D& v, const Matrix4x4& m)
{
	return Vector3D(v.x*m[0] + v.y*m[1] + v.z*m[2],
					v.x*m[4] + v.y*m[5] + v.z*m[6],
					v.x*m[8] + v.y*m[9] + v.z*m[10]);
}
Vector4D operator*(const Vector4D& v, const Matrix4x4& m)
{
	return Vector4D(v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3],
					v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7],
					v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11],
					v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]);
}
std::ostream& operator<<(std::ostream& os, const Matrix4x4& m)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[8] << " " << std::setw(10) << m[12] << "]\n"
		<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[9] << " " << std::setw(10) << m[13] << "]\n"
		<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[6] << " " << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << "]\n"
		<< "[" << std::setw(10) << m[3] << " " << std::setw(10) << m[7] << " " << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << "]\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}