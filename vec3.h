#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>

typedef float real_t;

class vec3
{
private:
	real_t e[3];

public:
	real_t &x = e[0];
	real_t &y = e[1];
	real_t &z = e[2];
	real_t &r = e[0];
	real_t &g = e[1];
	real_t &b = e[2];

	vec3() { x = 0; y = 0; z = 0; }
	vec3(real_t _x, real_t _y, real_t _z) { x = _x; y = _y; z = _z; }
	~vec3() {}

	inline const vec3 &operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-x, -y, -z); }
	inline real_t operator[](uint i) const { return e[i]; }
	inline real_t &operator[](uint i) { return e[i]; }

	inline vec3 &operator+=(const vec3 &v);
	inline vec3 &operator-=(const vec3 &v);
	inline vec3 &operator*=(const vec3 &v);
	inline vec3 &operator/=(const vec3 &v);
	inline vec3 &operator+=(real_t c);
	inline vec3 &operator-=(real_t c);
	inline vec3 &operator*=(real_t c);
	inline vec3 &operator/=(real_t c);

	inline real_t squaredLength() const { return x * x + y * y + z * z; }
	inline real_t length() const { return sqrt(squaredLength()); }
	inline vec3 &normalize();
};

inline vec3 &vec3::operator+=(const vec3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline vec3 &vec3::operator-=(const vec3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline vec3 &vec3::operator*=(const vec3 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline vec3 &vec3::operator/=(const vec3 &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline vec3 &vec3::operator+=(real_t c)
{
	x += c;
	y += c;
	z += c;
	return *this;
}

inline vec3 &vec3::operator-=(real_t c)
{
	x -= c;
	y -= c;
	z -= c;
	return *this;
}

inline vec3 &vec3::operator*=(real_t c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

inline vec3 &vec3::operator/=(real_t c)
{
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

inline vec3 &vec3::normalize()
{
	real_t invl = real_t(1) / length();
	*this *= invl;
	return *this;
}

inline std::istream &operator>>(std::istream &is, vec3 &v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const vec3 &v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
}

inline vec3 operator+(const vec3 &a, const vec3 &b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator-(const vec3 &a, const vec3 &b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator*(const vec3 &a, const vec3 &b)
{
	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline vec3 operator/(const vec3 &a, const vec3 &b)
{
	return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline vec3 operator+(const vec3 &v, real_t c)
{
	return vec3(v.x + c, v.y + c, v.z + c);
}

inline vec3 operator-(const vec3 &v, real_t c)
{
	return vec3(v.x - c, v.y - c, v.z - c);
}

inline vec3 operator*(const vec3 &v, real_t c)
{
	return vec3(v.x * c, v.y * c, v.z * c);
}

inline vec3 operator/(const vec3 &v, real_t c)
{
	return vec3(v.x / c, v.y / c, v.z / c);
}

inline vec3 operator+(real_t c, const vec3 &v)
{
	return v + c;
}

inline vec3 operator-(real_t c, const vec3 &v)
{
	return v - c;
}

inline vec3 operator*(real_t c, const vec3 &v)
{
	return v * c;
}

inline vec3 operator/(real_t c, const vec3 &v)
{
	return vec3(c / v.x, c / v.y, c / v.z);
}

inline real_t dot(const vec3 &a, const vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3 &a, const vec3 &b)
{
	return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline vec3 normalize(const vec3 &v)
{
	real_t invl = real_t(1) / v.length();
	return v * invl;
}
