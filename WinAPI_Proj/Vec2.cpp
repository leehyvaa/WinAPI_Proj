#include "pch.h"
#include "Vec2.h"





Vec2 Vec2::operator-(const Vec2& v) const
{
	return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator*(float n) const
{
	return Vec2(x*n,y*n);
}

Vec2 Vec2::operator/(float n) const
{
	assert(!0.f == n);
	return Vec2(x/n,y/n);
}

Vec2 Vec2::operator/(const Vec2& v) const
{
	assert(!(0.f == v.x || 0.f == v.y));
	return Vec2(x/v.x, y/v.y);
}

Vec2 Vec2::operator*(const Vec2& v) const
{
	return Vec2(x*v.x,y*v.y);
}

Vec2 Vec2::operator+(const Vec2& v) const
{
	return Vec2(x+v.x,y+v.y);
}

float Vec2::Length()
{
	return sqrt(x * x + y * y);
}

float Vec2::Cross(Vec2& _vec, POINT p)
{
	POINT v1;
	v1.x = _vec.x - x;
	v1.y = _vec.y - y;

	POINT v2;
	v2.x = p.x - x;
	v2.y = p.y - y;

	float ans = v1.x * v2.y - v1.y * v2.x;
	return ans;
}

Vec2& Vec2::normalize()
{
	float fLen = Length();

	assert(fLen != 0.f);

	x /= fLen;
	y /= fLen;

	return *this;
}
