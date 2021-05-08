#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Vec3.hpp"

#include <iostream>

class Quat
{
public:
	Real w;
	Real x;
	Real y;
	Real z;

	Quat() { w = 1; x = 0; y = 0; z = 0; }
	Quat(const Quat &q) { w = q.w; x = q.x; y = q.y; z = q.z; }
	Quat(Quat && q) { w = std::move(q.w); x = std::move(q.x); y = std::move(q.y); z = std::move(q.z); }
	Quat(Real _w, Real _x, Real _y, Real _z) { w = _w; x = _x; y = _y; z = _z; }
	Quat(Real s, const Vec3 &v) { w = s; x = v.x; y = v.y; z = v.z; }
	~Quat() = default;

	inline Quat &operator=(const Quat &q) { if (this != &q) { w = q.w; x = q.x; y = q.y; z = q.z; }; return *this; }
	inline Quat &operator=(Quat && q) { w = std::move(q.w); x = std::move(q.x); y = std::move(q.y); z = std::move(q.z); return *this; }

	inline bool operator==(const Quat &q) const { return w == q.w && x == q.x && y == q.y && z == q.z; }
	inline bool operator!=(const Quat &q) const { return w != q.w || x != q.x || y != q.y || z != q.z; }

	inline Quat &operator+() { return *this; }
	inline Quat operator-() const { return Quat(-w, -x, -y, -z); }
	inline Real operator[](uint i) const { i = math::min(i, 3u); return *(&w + i); }
	inline Real &operator[](uint i) { i = math::min(i, 3u); return *(&w + i); }

	inline Quat &operator+=(const Quat &q);
	inline Quat &operator-=(const Quat &q);
	inline Quat &operator*=(const Quat &q);
	inline Quat &operator/=(const Quat &q);
	inline Quat &operator+=(Real c);
	inline Quat &operator-=(Real c);
	inline Quat &operator*=(Real c);
	inline Quat &operator/=(Real c);

	inline Quat getConjugate() const { return Quat(w, -x, -y, -z); }
	inline Quat &conjugate();
	inline Real squaredNorm() const { return w * w + x * x + y * y + z * z; }
	inline Real norm() const { return sqrt(squaredNorm()); }
	inline Quat normalized() const;
	inline Quat &normalize();
	inline Quat reciprocal() const;
	inline Quat &reciprocate();

	inline void getAxisAngle(Vec3 &axisOut, Real &angleOut) const;
};

inline Quat &Quat::operator+=(const Quat &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

inline Quat &Quat::operator-=(const Quat &q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return *this;
}

// Note that this is equivalent to:
// *this = *this * q;
// Remember that chaining rotations is done from right to left
inline Quat &Quat::operator*=(const Quat &q)
{
	Vec3 v1(x, y, z);
	Vec3 v2(q.x, q.y, q.z);
	Real newW = w * q.w - dot(v1, v2);
	v1 = w * v2 + q.w * v1 + cross(v1, v2);
	w = newW;
	x = v1.x;
	y = v1.y;
	z = v1.z;
	return *this;
}

inline Quat &Quat::operator/=(const Quat &q)
{
	Quat rcp = q.reciprocal();
	*this *= rcp;
	return *this;
}

inline Quat &Quat::operator+=(Real c)
{
	w += c;
	x += c;
	y += c;
	z += c;
	return *this;
}

inline Quat &Quat::operator-=(Real c)
{
	w -= c;
	x -= c;
	y -= c;
	z -= c;
	return *this;
}

inline Quat &Quat::operator*=(Real c)
{
	w *= c;
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

inline Quat &Quat::operator/=(Real c)
{
	w /= c;
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

inline Quat &Quat::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

inline Quat Quat::normalized() const
{
	Quat q = *this;
	return q.normalize();
}

inline Quat &Quat::normalize()
{
	Real n = norm();
	if (n == 0)
	{
		*this = Quat();
	}
	else
	{
		Real invn = 1.0 / n;
		*this *= invn;
	}
	return *this;
}

inline Quat Quat::reciprocal() const
{
	Real sqNorm = squaredNorm();
	if (sqNorm == 0)
		return Quat();

	Quat q = getConjugate();
	q /= sqNorm;
	return q;
}

inline Quat &Quat::reciprocate()
{
	Real sqNorm = squaredNorm();
	if (sqNorm == 0)
	{
		*this = Quat();
	}
	else
	{
		conjugate();
		*this /= sqNorm;
	}
	return *this;
}

inline void Quat::getAxisAngle(Vec3 &axisOut, Real &angleOut) const
{
	angleOut = 2.0 * acos(w);
	if (angleOut == 0)
	{
		axisOut = Vec3();
	}
	else
	{
		axisOut = Vec3(x, y, z) / sqrt(1.0 - w * w);
	}
}

inline std::istream &operator>>(std::istream &is, Quat &q)
{
	is >> q.w >> q.x >> q.y >> q.z;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Quat &q)
{
	os << "Quat(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
	return os;
}

inline Quat operator+(const Quat &a, const Quat &b)
{
	return Quat(a.w + b.w, a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Quat operator-(const Quat &a, const Quat &b)
{
	return Quat(a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Quat operator*(const Quat &a, const Quat &b)
{
	Vec3 a_v(a.x, a.y, a.z);
	Vec3 b_v(b.x, b.y, b.z);
	return Quat(a.w * b.w - dot(a_v, b_v), a.w * b_v + b.w * a_v + cross(a_v, b_v));
}

inline Quat operator/(const Quat &a, const Quat &b)
{
	return a * b.reciprocal();
}

inline Quat operator+(const Quat &q, Real c)
{
	return Quat(q.w + c, q.x + c, q.y + c, q.z + c);
}

inline Quat operator-(const Quat &q, Real c)
{
	return Quat(q.w - c, q.x - c, q.y - c, q.z - c);
}

inline Quat operator*(const Quat &q, Real c)
{
	return Quat(q.w * c, q.x * c, q.y * c, q.z * c);
}

inline Quat operator/(const Quat &q, Real c)
{
	return Quat(q.w / c, q.x / c, q.y / c, q.z / c);
}

inline Quat operator+(Real c, const Quat &q)
{
	return q + c;
}

inline Quat operator-(Real c, const Quat &q)
{
	return -q + c;
}

inline Quat operator*(Real c, const Quat &q)
{
	return q * c;
}

inline Quat operator/(Real c, const Quat &q)
{
	return q.reciprocal() * c;
}

inline Real min(const Quat &q)
{
	return math::min(math::min(q.w, q.x), math::min(q.y, q.z));
}

inline Quat min(const Quat &a, const Quat &b)
{
	return Quat(math::min(a.w, b.w), math::min(a.x, b.x), math::min(a.y, b.y), math::min(a.z, b.z));
}

inline Real max(const Quat &q)
{
	return math::max(math::max(q.w, q.x), math::max(q.y, q.z));
}

inline Quat max(const Quat &a, const Quat &b)
{
	return Quat(math::max(a.w, b.w), math::max(a.x, b.x), math::max(a.y, b.y), math::max(a.z, b.z));
}

inline Quat abs(const Quat &q)
{
	return Quat(math::abs(q.w), math::abs(q.x), math::abs(q.y), math::abs(q.z));
}

inline bool closeEnough(const Quat &a, const Quat &b, Real epsilon)
{
	return max(abs(a - b)) < epsilon;
}

inline Quat lerp(const Quat &a, const Quat &b, Real t)
{
	t = math::clamp(t, Real(0), Real(1));
	return (1 - t) * a + t * b;
}

inline Real dot(const Quat &a, const Quat &b)
{
	return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Quat conjugate(const Quat &q)
{
	return Quat(q.w, -q.x, -q.y, -q.z);
}

inline Quat normalize(const Quat &q)
{
	Real norm = q.norm();
	if (norm == 0)
		return Quat();

	Real invNorm = 1.0 / norm;
	return q * invNorm;
}

inline Quat reciprocate(const Quat &q)
{
	Real sqNorm = q.squaredNorm();
	if (sqNorm == 0)
		return Quat();

	Real invSqNorm = 1.0 / sqNorm;
	return q.getConjugate() * invSqNorm;
}

inline Quat axisAngleToQuat(const Vec3 &axis, Real angle)
{
	if (angle == 0)
		return Quat();

	Real axisLength = axis.length();
	if (axisLength == 0)
		return Quat();

	Real halfAngle = angle * 0.5;
	return Quat(cos(halfAngle), (axis / axisLength) * sin(halfAngle));
}

inline Vec3 rotate(const Vec3 &v, const Quat &q)
{
	Vec3 q_v(q.x, q.y, q.z);
	Vec3 res = dot(v, q_v) * q_v;
	res += q.w * cross(q_v, v);
	res *= 2;
	res += (q.w * q.w - q_v.squaredLength()) * v;
	return res;
}
