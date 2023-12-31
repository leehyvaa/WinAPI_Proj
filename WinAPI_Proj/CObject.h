#pragma once
#include "framework.h"
#include "WinAPI_Proj.h"
#include "cVector3.h"

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

public:
	CObject();
	CObject(POINT pos,int t);
	virtual ~CObject();
	virtual void Update() = 0;
	virtual void Draw(HDC dc) = 0;
	virtual bool Collision() = 0;


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
	bool Collision() override;

};


class CRectangle : public CObject
{
private:
	float radius;
	RECT rect;
public:
	CRectangle() : CObject() {}
	CRectangle(HWND hWnd, POINT pos);
	~CRectangle() override;
	void Update() override;
	void Draw(HDC dc) override;
	bool Collision() override;

};

//분할하고 2면 머지
//1이면 머지
//머지 후에는 조금 전에 분할했던 그룹이랑 다시 정렬
