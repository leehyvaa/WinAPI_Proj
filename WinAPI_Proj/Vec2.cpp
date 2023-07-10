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
	return Vec2(x/n,y/n);
}

float Vec2::Length()
{
	return sqrt(x * x + y * y);
}

Vec2 Vec2::normalize()
{
	return Vec2(x / Length(), y / Length());
}
