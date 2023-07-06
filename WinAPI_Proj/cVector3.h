#pragma once

#include <iostream>
#include <Windows.h>
class cVector3
{
private:
	double x;
	double y;
	double z;
public:
	cVector3();
	cVector3(double x, double y, double z);
	~cVector3();

	double Getx();
	double Gety();
	double Getz();

	void Setx(double n);
	void Sety(double n);
	void Setz(double n);


	cVector3 operator=(const cVector3& v);
	cVector3 operator=(const POINT &v) const;



	bool operator==(const cVector3& v) const;
	bool operator!=(const cVector3& v) const;
	cVector3 operator+(const cVector3& v) const;
	cVector3 operator+(const POINT& v) const;

	cVector3 operator-(const cVector3& v) const;
	cVector3 operator*(double n) const;
	cVector3 operator/(double n) const;
	

	double Length();
	cVector3 normalize();

	double dot(cVector3& v1); //dotproduct 상수가나옴
	cVector3 cross(cVector3& v1); 
	double angle(cVector3& v1);

	friend cVector3 operator*(double n, cVector3& vec);
	friend std::ostream& operator<<(std::ostream& os, const cVector3& v);//출력
	//다른 객체에 있는 함수에 매개변수를 추가하고 싶을 때 사용함
	//ostream 객체에 있는 함수에 매개변수를 ostream말고도 cVetor3를 사용하게 해달라 하는것 
};

