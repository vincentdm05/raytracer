#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Vec3.hpp"

#include <iostream>
#include <cmath>

class Quat
{
public:
	union
	{
		struct
		{
			Real s;
			Vec3 v;
		};
		struct
		{
			Real w;
			Real x;
			Real y;
			Real z;
		};
	};

	Quat() { w = 1; x = 0; y = 0; z = 0; }
	Quat(const Quat &q) { s = q.s; v = q.v; }
	Quat(Quat && q) { s = std::move(q.s); v = std::move(q.v); }
	Quat(Real _w, Real _x, Real _y, Real _z) { w = _w; x = _x; y = _y; z = _z; }
	Quat(Real _s, const Vec3 &_v) { s = _s; v = _v; }
	~Quat() = default;

	inline Quat &operator=(const Quat &other) { if (this != &other) { s = other.s; v = other.v; }; return *this; }
	inline Quat &operator=(Quat && q) { s = std::move(q.s); v = std::move(q.v); return *this; }

	inline bool operator==(const Quat &q) const { return s == q.s && v == q.v; }
	inline bool operator!=(const Quat &q) const { return s != q.s || v != q.v; }

	inline Quat &operator+() { return *this; }
	inline Quat operator-() const { return Quat(-s, -v); }
	inline Real operator[](uint i) const { if (i == 0) return s; return v[i - 1]; }
	inline Real &operator[](uint i) { if (i == 0) return s; return v[i - 1]; }

	inline Quat &operator+=(const Quat &q);
	inline Quat &operator-=(const Quat &q);
	inline Quat &operator*=(const Quat &q);
	inline Quat &operator/=(const Quat &q);
	inline Quat &operator+=(Real c);
	inline Quat &operator-=(Real c);
	inline Quat &operator*=(Real c);
	inline Quat &operator/=(Real c);

	inline Quat getConjugate() const { return Quat(s, -v); }
	inline Quat &conjugate();
	inline Real squaredNorm() const { return s * s + v.squaredLength(); }
	inline Real norm() const { return sqrt(squaredNorm()); }
	inline Quat normalized() const;
	inline Quat &normalize();
	inline Quat reciprocal() const;
	inline Quat &reciprocate();

	inline void getAxisAngle(Vec3 &axisOut, Real &angleOut) const;
};

inline Quat &Quat::operator+=(const Quat &q)
{
	s += q.s;
	v += q.v;
	return *this;
}

inline Quat &Quat::operator-=(const Quat &q)
{
	s -= q.s;
	v -= q.v;
	return *this;
}

// Note that this is equivalent to:
// *this = *this * q;
// Remember that chaining rotations is done in reverse order
inline Quat &Quat::operator*=(const Quat &q)
{
	Real newS = s * q.s - dot(v, q.v);
	Vec3 newV = s * q.v + q.s * v + cross(v, q.v);
	s = newS;
	v = newV;
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
	s += c;
	v += c;
	return *this;
}

inline Quat &Quat::operator-=(Real c)
{
	s -= c;
	v -= c;
	return *this;
}

inline Quat &Quat::operator*=(Real c)
{
	s *= c;
	v *= c;
	return *this;
}

inline Quat &Quat::operator/=(Real c)
{
	s /= c;
	v /= c;
	return *this;
}

inline Quat &Quat::conjugate()
{
	v = -v;
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
	angleOut = 2.0 * acos(s);
	if (angleOut == 0)
	{
		axisOut = Vec3();
	}
	else
	{
		axisOut = v / sqrt(1.0 - s * s);
	}
}

inline std::istream &operator>>(std::istream &is, Quat &q)
{
	is >> q.s >> q.v;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Quat &q)
{
	os << "Quat(" << q.s << ", " << q.v << ")";
	return os;
}

inline Quat operator+(const Quat &a, const Quat &b)
{
	return Quat(a.s + b.s, a.v + b.v);
}

inline Quat operator-(const Quat &a, const Quat &b)
{
	return Quat(a.s - b.s, a.v - b.v);
}

inline Quat operator*(const Quat &a, const Quat &b)
{
	return Quat(a.s * b.s - dot(a.v, b.v), a.s * b.v + b.s * a.v + cross(a.v, b.v));
}

inline Quat operator/(const Quat &a, const Quat &b)
{
	return a * b.reciprocal();
}

inline Quat operator+(const Quat &q, Real c)
{
	return Quat(q.s + c, q.v + c);
}

inline Quat operator-(const Quat &q, Real c)
{
	return Quat(q.s - c, q.v - c);
}

inline Quat operator*(const Quat &q, Real c)
{
	return Quat(q.s * c, q.v * c);
}

inline Quat operator/(const Quat &q, Real c)
{
	return Quat(q.s / c, q.v / c);
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
	return min(q.s, min(q.v));
}

inline Quat min(const Quat &a, const Quat &b)
{
	return Quat(min(a.s, b.s), min(a.v, b.v));
}

inline Real max(const Quat &q)
{
	return max(q.s, max(q.v));
}

inline Quat max(const Quat &a, const Quat &b)
{
	return Quat(max(a.s, b.s), max(a.v, b.v));
}

inline Quat abs(const Quat &q)
{
	return Quat(abs(q.s), abs(q.v));
}

inline bool closeEnough(const Quat &a, const Quat &b, Real epsilon)
{
	return max(abs(a - b)) < epsilon;
}

inline Quat lerp(const Quat &a, const Quat &b, Real t)
{
	t = clamp(t, Real(0), Real(1));
	return (1 - t) * a + t * b;
}

inline Real dot(const Quat &a, const Quat &b)
{
	return a.s * b.s + dot(a.v, b.v);
}

inline Quat conjugate(const Quat &q)
{
	return Quat(q.s, -q.v);
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
	Vec3 res = dot(v, q.v) * q.v;
	res += q.s * cross(q.v, v);
	res *= 2;
	res += (q.s * q.s - q.v.squaredLength()) * v;
	return res;
}
