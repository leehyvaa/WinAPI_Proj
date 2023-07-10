#include"pch.h"
#include "cVector3.h"

const double PI = std::acos(-1);

cVector3::cVector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
cVector3::cVector3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


cVector3::~cVector3()
{
	
}

double cVector3::Getx()
{
	return x;
}

double cVector3::Gety()
{
	return y;
}

double cVector3::Getz()
{
	return z;
}

void cVector3::Setx(double n)
{
	x = n;
}

void cVector3::Sety(double n)
{
	y = n;
}

void cVector3::Setz(double n)
{
	z = n;
}

cVector3 cVector3::operator=(const cVector3& v) 
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;

	return cVector3(v.x,v.y,v.z);
}

cVector3 cVector3::operator=(const POINT& v) const
{
	return cVector3(v.x,v.y,0);
}


bool cVector3::operator==(const cVector3& v) const
{
	if (x == v.x && y == v.y && z == v.z)
		return true;
	else
		return false;
}

bool cVector3::operator!=(const cVector3& v) const
{
	if (x == v.x && y == v.y && z == v.z)
		return false;
	else
		return true;
}

cVector3 cVector3::operator+(const cVector3& v) const
{
	return cVector3(x+v.x,y+v.y,z+v.z);
}

cVector3 cVector3::operator+(const POINT& v) const
{
	return cVector3(x+v.x,y+v.y,z);
}

cVector3 cVector3::operator-(const cVector3& v) const
{
	return cVector3(x - v.x, y - v.y, z - v.z);
}

cVector3 cVector3::operator*(double n) const
{
	return cVector3(x*n,y*n,z*n);
}

cVector3 cVector3::operator/(double n) const
{
	return cVector3(x/n,y/n,z/n);
}

double cVector3::Length()
{
	
	return sqrt(x * x + y * y + z * z);
}

cVector3 cVector3::normalize()
{
	return cVector3(x/Length(),y/Length(),z/Length());
}

double cVector3::dot(cVector3& v1)
{
	return x*v1.x + y*v1.y + z*v1.z;
}

cVector3 cVector3::cross(cVector3& v1)
{
	return cVector3(y*v1.z-z*v1.y, z*v1.x-x*v1.z,x*v1.y - y*v1.x);
}

double cVector3::angle(cVector3& v1)
{


	return ((acos(dot(v1)/(Length()*v1.Length()))) * PI) /180;
}

cVector3 operator*(double n, cVector3& vec)
{
	return cVector3(vec.x*n,vec.y*n,vec.z*n);
}

std::ostream& operator<<(std::ostream& os, const cVector3& v)
{
	os << v.x<<" "<<v.y<<" "<< v.z;
	return os;
	// TODO: 여기에 return 문을 삽입합니다.
}
