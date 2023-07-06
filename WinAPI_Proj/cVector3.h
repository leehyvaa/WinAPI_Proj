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

	double dot(cVector3& v1); //dotproduct ���������
	cVector3 cross(cVector3& v1); 
	double angle(cVector3& v1);

	friend cVector3 operator*(double n, cVector3& vec);
	friend std::ostream& operator<<(std::ostream& os, const cVector3& v);//���
	//�ٸ� ��ü�� �ִ� �Լ��� �Ű������� �߰��ϰ� ���� �� �����
	//ostream ��ü�� �ִ� �Լ��� �Ű������� ostream���� cVetor3�� ����ϰ� �ش޶� �ϴ°� 
};

