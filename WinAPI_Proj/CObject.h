#pragma once
#include "framework.h"
#include "WinAPI_Proj.h"
#include "cVector3.h"
#include <vector>
using namespace std;

class CObject
{
protected:
	POINT position;
	float moveSpeed;
	cVector3 dirVector;
	int type;
	HWND hWnd;
	RECT rectView;

	//HDC hdc;
	float rotate;

public:
	CObject();
	CObject(POINT pos,int t);
	POINT GetPos() { return position; }
	virtual ~CObject();
	virtual void Update() = 0;
	virtual void Draw(HDC dc) = 0;
	virtual bool Collision(CObject& vObj) = 0;


};



class CCircle : public CObject
{
private:
	float radius;
public:
	CCircle() : CObject() {}
	CCircle(HWND hWnd, POINT pos);
	~CCircle() override;
	void Update() override;
	void Draw(HDC dc) override;
	bool Collision(CObject& vObj) override;

};


class CRectangle : public CObject
{
private:
	float radius;
	cVector3 vRectPos[4];
public:
	CRectangle() : CObject() { radius = 0; }
	CRectangle(HWND hWnd, POINT pos);
	~CRectangle() override;
	void Update() override;
	void Draw(HDC dc) override;
	bool Collision(CObject& vObj) override;

};



class CStar : public CObject
{
private:
	float radius;
	cVector3 vRectPos[10];
public:
	CStar() : CObject() { radius = 0; }
	CStar(HWND hWnd, POINT pos);
	~CStar() override;
	void Update() override;
	void Draw(HDC dc) override;
	bool Collision(CObject& vObj) override;

};

//�����ϰ� 2�� ����
//1�̸� ����
//���� �Ŀ��� ���� ���� �����ߴ� �׷��̶� �ٽ� ����
