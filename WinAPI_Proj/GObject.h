#pragma once
#include "framework.h"
#include "WinAPI_Proj.h"
#include "cVector3.h"
#include <vector>
using namespace std;


class GObject
{
protected:
	POINT position;
	float moveSpeed;
	cVector3 dirVector;
	float scale;

	HWND hWnd;
	RECT rectView;

	//HDC hdc;
	float rotate;

public:
	GObject();
	virtual ~GObject();
	POINT GetPos() { return position; }
	virtual void Update() = 0;
	virtual void Draw(HDC dc) = 0;
	virtual bool Collision(GObject& vObj) = 0;

};

