#include "Vector4D.h"
Vector4D::Vector4D()
	: x{0},y{0},z{0},w{0}
{
}

Vector4D::Vector4D(float x, float y, float z, float w)
	: x{ x }, y{ y }, z{ z }, w{ w }
{
}

void        Vector4D::set(float ix, float iy, float iz, float iw)
{
	x = ix, y = iy, z = iz, w= iw;
}
float       Vector4D::length() const
{
	return sqrtf(x*x + y * y + z * z + w * w);
}
float       Vector4D::distance(const Vector4D& vec) const
{
	return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z) + (vec.w - w)*(vec.w - w));
}
Vector4D&   Vector4D::normalize()
{
	const float EPSILON = 0.000001f;
	float xxyyzz = x * x + y * y + z * z;
	if(xxyyzz < EPSILON)
	    return *this;
	float invLength = 1.0f / sqrtf(xxyyzz);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return *this;
}
float       Vector4D::dot(const Vector4D& vec) const
{
	return (x*vec.x + y * vec.y + z * vec.z + w * vec.w);
}
// Arithmetic and boolean 
Vector4D    Vector4D::operator-() const
{
	return Vector4D(-x, -y, -z, -w);
}
Vector4D    Vector4D::operator+(const Vector4D& rhs) const
{
	return Vector4D(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}
Vector4D    Vector4D::operator-(const Vector4D& rhs) const
{
	return Vector4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}
Vector4D&   Vector4D::operator+=(const Vector4D& rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this;
}
Vector4D&   Vector4D::operator-=(const Vector4D& rhs)
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;
}
Vector4D    Vector4D::operator*(const float scale) const
{
	return Vector4D(x*scale, y*scale, z*scale, w*scale);
}
Vector4D    Vector4D::operator*(const Vector4D& rhs) const
{
	return Vector4D(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
}
Vector4D&   Vector4D::operator*=(const float scale)
{
	x *= scale; y *= scale; z *= scale; w *= scale; return *this;
}
Vector4D&   Vector4D::operator*=(const Vector4D& rhs)
{
	x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this;
}
Vector4D    Vector4D::operator/(const float scale) const
{
	return Vector4D(x / scale, y / scale, z / scale, w / scale);
}
Vector4D&   Vector4D::operator/=(const float scale)
{
	x /= scale; y /= scale; z /= scale; w /= scale; return *this;
}
bool		Vector4D::operator==(const Vector4D& rhs) const
{
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}
bool		Vector4D::operator!=(const Vector4D& rhs) const
{
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}
bool		Vector4D::operator<(const Vector4D& rhs) const
{
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	if (z < rhs.z) return true;
	if (z > rhs.z) return false;
	if (w < rhs.w) return true;
	if (w > rhs.w) return false;
	return false;
}
float		Vector4D::operator[](int index) const
{
	return (&x)[index];
}
float&		Vector4D::operator[](int index)
{
	return (&x)[index];
}

Vector4D	operator*(const float a, const Vector4D vec)
{
	return Vector4D(a*vec.x, a*vec.y, a*vec.z, a*vec.w);
}
std::ostream& operator<<(std::ostream& os, const Vector4D& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return os;
}