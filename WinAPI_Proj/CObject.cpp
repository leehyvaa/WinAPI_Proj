#include "CObject.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#define PI 3.141592
#define degreeToRadian(degree) ((degree)*PI/180)


void DrawRect(HDC hdc, POINT center, cVector3 top, cVector3 bot, cVector3 left, cVector3 right);

CObject::CObject()
{

}

CObject::CObject(POINT pos, int t)
{


}

CObject::~CObject()
{
}

CCircle::~CCircle()
{
}

CCircle::CCircle(HWND hWnd,POINT pos)
{
	srand((unsigned)time(NULL));
	
	
	dirVector.Setx(((double)(rand() % 20+1) / 10 )-1);
	dirVector.Sety(((double)(rand() % 20+1) / 10 )-1);
	
	GetClientRect(hWnd, &rectView);
	this->hWnd = hWnd;
	position.x = pos.x;
	position.y = pos.y;
	rotate = 0;
	moveSpeed = rand()%10 +1;
	radius = 50;
}

void CCircle::Update()
{
	Collision();
	

	position.x = position.x + moveSpeed * dirVector.Getx();
	position.y = position.y + moveSpeed * dirVector.Gety();
	


}

void CCircle::Draw(HDC dc)
{
	
	
	Ellipse(dc, position.x - radius, position.y - radius, position.x + radius, position.y + radius);
	

}

bool CCircle::Collision()
{
	

	if (position.x + radius + moveSpeed * dirVector.Getx() > rectView.right)
	{
		dirVector.Setx(-dirVector.Getx());
		return true;
	}


	if (position.x - radius + moveSpeed*dirVector.Getx() < rectView.left)
	{
		dirVector.Setx(-dirVector.Getx());
		return true;
	}
       
	if (position.y - radius + moveSpeed * dirVector.Gety() < rectView.top)
	{
		dirVector.Sety(-dirVector.Gety());

		return true;
	}

	if (position.y + radius + moveSpeed * dirVector.Gety() > rectView.bottom)
	{
		dirVector.Sety(-dirVector.Gety());
		return true;
	}
        


	return false;

}


CRectangle::CRectangle(HWND hWnd, POINT pos)
{
	srand((unsigned)time(NULL));


	dirVector.Setx(((double)(rand() % 20 + 1) / 10) - 1);
	dirVector.Sety(((double)(rand() % 20 + 1) / 10) - 1);

	GetClientRect(hWnd, &rectView);
	this->hWnd = hWnd;
	position.x = pos.x;
	position.y = pos.y;
	moveSpeed = rand() % 10 + 1;
	rotate = 0;
	radius = 50;

	/*POINT leftTop = {position.x - radius / 2 , position.y - radius / 2 };
	POINT leftBot = { position.x - radius / 2 , position.y + radius / 2 };
	POINT rightBot = { position.x + radius / 2 , position.y + radius / 2 };
	POINT rightTop = { position.x + radius / 2 , position.y - radius / 2 };*/

	vRectPos[0] = cVector3(position.x, position.y-radius,0);
	vRectPos[1] = cVector3(position.x, position.y+radius, 0);
	vRectPos[2] = cVector3(position.x-radius,position.y, 0);
	vRectPos[3] = cVector3(position.x+radius,position.y, 0);


}

CRectangle::~CRectangle()
{

}


void CRectangle::Update()
{
	Collision();
	position.x = position.x + moveSpeed * dirVector.Getx();
	position.y = position.y + moveSpeed * dirVector.Gety();

	rotate += 5.0;
	double angle = degreeToRadian(rotate);



	//rect.left = position.x - radius;
	//rect.right = position.x + radius;
	//rect.top = position.y - radius;
	//rect.bottom = position.y + radius;

	vRectPos[0] = cVector3(position.x, position.y - radius, 0);
	vRectPos[1] = cVector3(position.x, position.y + radius, 0);
	vRectPos[2] = cVector3(position.x - radius, position.y, 0);
	vRectPos[3] = cVector3(position.x + radius, position.y, 0);

	
}

void CRectangle::Draw(HDC dc)
{

	DrawRect(dc, position, vRectPos[0], vRectPos[1], vRectPos[2], vRectPos[3]);
	//Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);

}

bool CRectangle::Collision()
{


	if (position.x + radius + moveSpeed * dirVector.Getx() > rectView.right)
	{
		dirVector.Setx(-dirVector.Getx());
		return true;
	}


	if (position.x - radius + moveSpeed * dirVector.Getx() < rectView.left)
	{
		dirVector.Setx(-dirVector.Getx());
		return true;
	}

	if (position.y - radius + moveSpeed * dirVector.Gety() < rectView.top)
	{
		dirVector.Sety(-dirVector.Gety());

		return true;
	}

	if (position.y + radius + moveSpeed * dirVector.Gety() > rectView.bottom)
	{
		dirVector.Sety(-dirVector.Gety());
		return true;
	}



	return false;

}


void DrawRect(HDC hdc, POINT center, cVector3 top, cVector3 bot, cVector3 left, cVector3 right)
{
	using namespace std;
	vector<cVector3> vPoint;
	vPoint.push_back(top);
	vPoint.push_back(bot);
	vPoint.push_back(left);
	vPoint.push_back(right);


	for (int i = 0; i < 4; i++)
	{
		MoveToEx(hdc, vPoint[i].Getx(), vPoint[i].Gety(), NULL);
		
		if (i >= 3)
			LineTo(hdc, vPoint[0].Getx(), vPoint[0].Gety());
		else
			LineTo(hdc, vPoint[i+1].Getx(), vPoint[i + 1].Gety());


	}



}
