#pragma once
class Vec2
{
public:
	float x;
	float y;


public :
	Vec2()
		:x(0.f)
		, y(0.f)
	{}
	
	Vec2(float _x, float _y)
		:x(_x)
		, y(_y)
	{}

	Vec2(int _x, int _y)
		:x((float)_x)
		, y((float)_y)
	{}

	Vec2 operator-(const Vec2& v) const;
	Vec2 operator*(float n) const;
	Vec2 operator/(float n) const;



	float Length();
	Vec2 normalize();

};

