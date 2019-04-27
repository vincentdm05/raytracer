#pragma once

#include "Common.hpp"

#include <iostream>
#include <math.h>

class Vec3
{
private:
	Real e[3];

public:
	Real &x = e[0];
	Real &y = e[1];
	Real &z = e[2];
	Real &r = e[0];
	Real &g = e[1];
	Real &b = e[2];

	Vec3() { x = 0; y = 0; z = 0; }
	Vec3(Real _x, Real _y, Real _z) { x = _x; y = _y; z = _z; }
	Vec3(Vec3 && other) { x = std::move(other.x); y = std::move(other.y); z = std::move(other.z); }
	Vec3(const Vec3 &other) { x = other.x; y = other.y; z = other.z; }

	inline Vec3 &operator=(const Vec3 &v) { if (this != &v) { x = v.x; y = v.y; z = v.z; }; return *this; }

	inline const Vec3 &operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-x, -y, -z); }
	inline Real operator[](uint i) const { return e[i % 3]; }
	inline Real &operator[](uint i) { return e[i % 3]; }

	inline Vec3 &operator+=(const Vec3 &v);
	inline Vec3 &operator-=(const Vec3 &v);
	inline Vec3 &operator*=(const Vec3 &v);
	inline Vec3 &operator/=(const Vec3 &v);
	inline Vec3 &operator+=(Real c);
	inline Vec3 &operator-=(Real c);
	inline Vec3 &operator*=(Real c);
	inline Vec3 &operator/=(Real c);

	inline Real squaredLength() const { return x * x + y * y + z * z; }
	inline Real length() const { return sqrt(squaredLength()); }
	inline Vec3 &normalize();
};

inline Vec3 &Vec3::operator+=(const Vec3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vec3 &Vec3::operator-=(const Vec3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vec3 &Vec3::operator*=(const Vec3 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline Vec3 &Vec3::operator/=(const Vec3 &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline Vec3 &Vec3::operator+=(Real c)
{
	x += c;
	y += c;
	z += c;
	return *this;
}

inline Vec3 &Vec3::operator-=(Real c)
{
	x -= c;
	y -= c;
	z -= c;
	return *this;
}

inline Vec3 &Vec3::operator*=(Real c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

inline Vec3 &Vec3::operator/=(Real c)
{
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

inline Vec3 &Vec3::normalize()
{
	Real invl = 1 / length();
	*this *= invl;
	return *this;
}

inline std::istream &operator>>(std::istream &is, Vec3 &v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
}

inline Vec3 operator+(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vec3 operator/(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline Vec3 operator+(const Vec3 &v, Real c)
{
	return Vec3(v.x + c, v.y + c, v.z + c);
}

inline Vec3 operator-(const Vec3 &v, Real c)
{
	return Vec3(v.x - c, v.y - c, v.z - c);
}

inline Vec3 operator*(const Vec3 &v, Real c)
{
	return Vec3(v.x * c, v.y * c, v.z * c);
}

inline Vec3 operator/(const Vec3 &v, Real c)
{
	return Vec3(v.x / c, v.y / c, v.z / c);
}

inline Vec3 operator+(Real c, const Vec3 &v)
{
	return v + c;
}

inline Vec3 operator-(Real c, const Vec3 &v)
{
	return v - c;
}

inline Vec3 operator*(Real c, const Vec3 &v)
{
	return v * c;
}

inline Vec3 operator/(Real c, const Vec3 &v)
{
	return Vec3(c / v.x, c / v.y, c / v.z);
}

inline Real dot(const Vec3 &a, const Vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Vec3 normalize(const Vec3 &v)
{
	Real invl = 1 / v.length();
	return v * invl;
}

inline Vec3 sqrt(const Vec3 &v)
{
	return Vec3(sqrt(v.x), sqrt(v.y), sqrt(v.z));
}

// n is assumed to be of unit length
inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
	return v - 2 * dot(v, n) * n;
}

// v and n are assumed to be of unit length
inline bool refract(const Vec3 &v, const Vec3 &n, Real refractiveIndexRatio, Vec3 &refracted)
{
	Real vDotN = dot(v, n);
	Real discriminant = 1 - refractiveIndexRatio * refractiveIndexRatio * (1 - vDotN * vDotN);
	if (discriminant > 0)
	{
		refracted = refractiveIndexRatio * (v - n * vDotN) - n * sqrt(discriminant);
		return true;
	}

	return false;
}
