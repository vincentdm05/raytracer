#pragma once

#include "Common.hpp"

#include "Math.hpp"

#include <iostream>

class Vec3
{
public:
	union
	{
		struct
		{
			Real x;
			Real y;
			Real z;
		};
		struct
		{
			Real r;
			Real g;
			Real b;
		};
	};

	Vec3() { x = 0; y = 0; z = 0; }
	Vec3(Real _x, Real _y, Real _z) { x = _x; y = _y; z = _z; }
	Vec3(const Vec3 &other) { x = other.x; y = other.y; z = other.z; }
	Vec3(Vec3 && other) { x = std::move(other.x); y = std::move(other.y); z = std::move(other.z); }
	~Vec3() = default;

	inline Vec3 &operator=(const Vec3 &v) { if (this != &v) { x = v.x; y = v.y; z = v.z; }; return *this; }
	inline Vec3 &operator=(Vec3 && v) { x = std::move(v.x); y = std::move(v.y); z = std::move(v.z); return *this; }

	inline bool operator==(const Vec3 &v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator!=(const Vec3 &v) const { return x != v.x || y != v.y || z != v.z; }

	inline const Vec3 &operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-x, -y, -z); }
	inline Real operator[](uint i) const { return *(&x + (i % 3)); }
	inline Real &operator[](uint i) { return *(&x + (i % 3)); }

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
	Real l = length();
	if (l != 0)
		*this /= l;
	return *this;
}

inline std::istream &operator>>(std::istream &is, Vec3 &v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
	os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
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
	return -v + c;
}

inline Vec3 operator*(Real c, const Vec3 &v)
{
	return v * c;
}

inline Vec3 operator/(Real c, const Vec3 &v)
{
	return Vec3(c / v.x, c / v.y, c / v.z);
}

inline Real min(const Vec3 &v)
{
	return min(v.x, min(v.y, v.z));
}

inline Vec3 min(const Vec3 &a, const Vec3 &b)
{
	return Vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

inline Real max(const Vec3 &v)
{
	return max(v.x, max(v.y, v.z));
}

inline Vec3 max(const Vec3 &a, const Vec3 &b)
{
	return Vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

inline Vec3 abs(const Vec3 &v)
{
	return Vec3(abs(v.x), abs(v.y), abs(v.z));
}

inline bool closeEnough(const Vec3 &a, const Vec3 &b, Real epsilon)
{
	return max(abs(a - b)) < epsilon;
}

inline bool all(const Vec3 &v)
{
	return v.x && v.y && v.z;
}

inline bool any(const Vec3 &v)
{
	return v.x || v.y || v.z;
}

inline Vec3 sign(const Vec3 &v)
{
	return Vec3(v.x > 0 ? 1 : -1, v.y > 0 ? 1 : -1, v.z > 0 ? 1 : -1);
}

inline Vec3 lerp(const Vec3 &a, const Vec3 &b, Real t)
{
	t = clamp(t, 0.0, 1.0);
	return (1 - t) * a + t * b;
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
	Real l = v.length();
	if (l != 0)
		return v / l;
	return v;
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
