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

	Vec2(const POINT& _pt)
		:x((float)_pt.x)
		, y((float)_pt.y)
	{}

	Vec2 operator-(const Vec2& v) const;
	Vec2 operator*(float n) const;
	Vec2 operator/(float n) const;
	Vec2 operator/(const Vec2& v) const;
	Vec2 operator*(const Vec2& v) const;
	void operator +=(Vec2 v)
	{
		x += v.x;
		y += v.y;
	}
	void operator -=(Vec2 v)
	{
		x -= v.x;
		y -= v.y;
	}
	void operator -=(float _f)
	{
		x -= _f;
		y -= _f;
	}
	void operator *=(float _f)
	{
		x *= _f;
		y *= _f;
	}



	Vec2 operator+(const Vec2& v) const;
	Vec2 operator-()
	{
		return Vec2(-x, -y);
	}

	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
		return *this;
	}


	bool operator == (const Vec2& v)
	{
		if (x == v.x && y == v.y)
			return true;
		return false;
	}

	float Length();
	Vec2& Normalize();
	float Dot(Vec2& _vec) { return x * _vec.x + y * _vec.y; }
	double Angle(Vec2& _vec);
	float Cross(Vec2& _vec,POINT p);


	bool IsZero();
};

