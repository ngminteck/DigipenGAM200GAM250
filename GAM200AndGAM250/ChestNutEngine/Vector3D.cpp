#include "Vector3D.h"

Vector3D::Vector3D()
	: x{ 0 }, y{ 0 }, z{ 0 }
{
}

Vector3D::Vector3D(float x, float y, float z)
	: x{ x }, y{ y }, z{ z }
{
}

void        Vector3D::set(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
}
float       Vector3D::length() const
{
	return sqrtf(x*x + y * y + z * z);
}
float       Vector3D::distance(const Vector3D& vec) const
{
	return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z));
}
float       Vector3D::angle(const Vector3D& vec) const
{
	float l1 = this->length();
	float l2 = vec.length();
	float d = this->dot(vec);
	float angle = acosf(d / (l1 * l2)) / 3.141592f * 180.0f;
	return angle;
}
Vector3D&   Vector3D::normalize()
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
float       Vector3D::dot(const Vector3D& vec) const
{
	return (x*vec.x + y * vec.y + z * vec.z);
}
Vector3D    Vector3D::cross(const Vector3D& vec) const
{
	return Vector3D(y*vec.z - z * vec.y, z*vec.x - x * vec.z, x*vec.y - y * vec.x);
}
Vector3D    Vector3D::operator-() const
{
	return Vector3D(-x, -y, -z);
}
Vector3D    Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(x+rhs.x, y+rhs.y, z+rhs.z);
}
Vector3D	Vector3D::operator+(const float rhs)
{
	return Vector3D(x + rhs, y + rhs,0.f); 
}
Vector3D    Vector3D::operator-(const Vector3D& rhs) const
{
	return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}
Vector3D&   Vector3D::operator+=(const Vector3D& rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}
Vector3D&   Vector3D::operator+=(const float rhs)
{
	x += rhs; y += rhs; z += rhs; return *this;
}
Vector3D&   Vector3D::operator-=(const Vector3D& rhs)
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}
Vector3D    Vector3D::operator*(const float s) const
{
	return Vector3D(x*s, y*s, z*s);
}
Vector3D    Vector3D::operator*(const Vector3D& rhs) const
{
	return Vector3D(x*rhs.x, y*rhs.y, z*rhs.z);
}
Vector3D&   Vector3D::operator*=(const float s)
{
	x *= s; y *= s; z *= s; return *this;
}
Vector3D&   Vector3D::operator*=(const Vector3D& rhs)
{
	x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}
Vector3D    Vector3D::operator/(const float scale) const
{
	return Vector3D(x / scale, y / scale, z / scale);
}
Vector3D&   Vector3D::operator/=(const float scale)
{
	x /= scale; y /= scale; z /= scale; return *this;
}
bool        Vector3D::operator==(const Vector3D& rhs) const
{
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}
bool        Vector3D::operator!=(const Vector3D& rhs) const
{
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}
bool        Vector3D::operator<(const Vector3D& rhs) const
{
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	if (z < rhs.z) return true;
	if (z > rhs.z) return false;
	return false;
}
float       Vector3D::operator[](int index) const
{
	return (&x)[index];
}
float&      Vector3D::operator[](int index)
{
	return (&x)[index];
}
void Vector3D::operator=(const Vector3D & rhs)
{
	x = rhs.x; y = rhs.y; z = rhs.z;
}
 Vector3D operator*(const float a, const Vector3D vec) 
{
	return Vector3D(a*vec.x, a*vec.y, a*vec.z);
}
 std::ostream& operator<<(std::ostream& os, const Vector3D& vec)
 {
	 os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	 return os;
 }


void Vector3D::AddToScript(sol::state * luaState)
{

	luaState->new_usertype<Vector3D>("vec3",
		//Constructor
		sol::constructors<Vector3D(), Vector3D(float, float, float)>(),
		//Operator Overloads
		sol::meta_function::addition, sol::overload(
			sol::resolve<Vector3D(const Vector3D&)const>(&Vector3D::operator+),
			sol::resolve<Vector3D(const float)>(&Vector3D::operator+)),
		sol::meta_function::subtraction, sol::resolve<Vector3D(const Vector3D&) const>(&Vector3D::operator-),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector3D(const float) const>(&Vector3D::operator*),
			sol::resolve<Vector3D(const Vector3D&)const>(&Vector3D::operator*)),
		sol::meta_function::division, &Vector3D::operator/,
		sol::meta_function::unary_minus, sol::resolve<Vector3D() const>(&Vector3D::operator-),
		sol::meta_function::equal_to, &Vector3D::operator=,
		//Member Functions
		"Set", &Vector3D::set,
		"Length", &Vector3D::length,
		"Distance", &Vector3D::distance,
		"Angle", &Vector3D::angle,
		"Normalize", &Vector3D::normalize,
		"Dot", &Vector3D::dot,
		"Cross", &Vector3D::cross,
		//Variables
		"x", &Vector3D::x,
		"y", &Vector3D::y,
		"z", &Vector3D::z
		);
 }