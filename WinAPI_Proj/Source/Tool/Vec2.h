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
		:x(static_cast<float>(_x))
		, y(static_cast<float>(_y))
	{}

	Vec2(const POINT& _pt)
		:x(static_cast<float>(_pt.x))
		, y(static_cast<float>(_pt.y))
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
		x = static_cast<float>(_pt.x);
		y = static_cast<float>(_pt.y);
		return *this;
	}


	bool operator == (const Vec2& v)
	{
		if (x == v.x && y == v.y)
			return true;
		return false;
	}

    // 2D 벡터 외적 (z 성분만 리턴)
    float Cross(const Vec2& _vec) const
	{
	    return x * _vec.y - y * _vec.x;
	}
    
    // 벡터 회전 함수
    Vec2 Rotate(float _angle) const
	{
	    float cosA = cos(_angle);
	    float sinA = sin(_angle);
	    return Vec2(x * cosA - y * sinA, x * sinA + y * cosA);
	}

	float Length();
	Vec2& Normalize();
	float Dot(Vec2& _vec) { return x * _vec.x + y * _vec.y; }
	double Angle(Vec2& _vec);
	float Cross(Vec2& _vec,POINT p);


	bool IsZero();
};

