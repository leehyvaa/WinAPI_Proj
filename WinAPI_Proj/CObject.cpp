#include "CObject.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <math.h>
#include "pch.h"

#define PI 3.141592
#define degreeToRadian(degree) ((degree)*PI/180)


void DrawRect(HDC hdc, POINT center, cVector3 top, cVector3 bot, cVector3 left, cVector3 right);
double LengthPts(float pt1_x, float pt1_y, POINT pt2);

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
	
	

	position.x = position.x + moveSpeed * dirVector.Getx();
	position.y = position.y + moveSpeed * dirVector.Gety();
	


}

void CCircle::Draw(HDC dc)
{
	
	
	Ellipse(dc, position.x - radius, position.y - radius, position.x + radius, position.y + radius);
	

}

bool CCircle::Collision(CObject& vObj)
{
	
	
	/*BOOL InCircle(POINT pt1, POINT pt2)
	{
		if (LengthPts(pt1, pt2) < radius) return TRUE;

		return FALSE;
	}*/

	if (radius * 2 > LengthPts(vObj.GetPos().x, vObj.GetPos().y, this->position))
	{
		dirVector.Setx(-dirVector.Getx());
		dirVector.Sety(-dirVector.Gety());

	}


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


	
	vRectPos[0] = cVector3(position.x, position.y - radius, 0);
	vRectPos[1] = cVector3(position.x, position.y+radius, 0);
	vRectPos[2] = cVector3(position.x-radius,position.y, 0);
	vRectPos[3] = cVector3(position.x+radius,position.y, 0);


}

CRectangle::~CRectangle()
{

}


void CRectangle::Update()
{

	position.x = position.x + moveSpeed * dirVector.Getx();
	position.y = position.y + moveSpeed * dirVector.Gety();

	rotate += 5.0;
	double angle = degreeToRadian(rotate);


	vRectPos[0] = cVector3(position.x, position.y - radius, 0);
	vRectPos[1] = cVector3(position.x + radius, position.y, 0);
	vRectPos[2] = cVector3(position.x, position.y + radius, 0);
	vRectPos[3] = cVector3(position.x - radius, position.y, 0);

	
}

void CRectangle::Draw(HDC dc)
{

	POINT p[4];
	for (int i = 0; i < 4; i++)
	{
		p[i].x = vRectPos[i].Getx();
		p[i].y = vRectPos[i].Gety();
	}
	Polygon(dc, p, 4);
}

bool CRectangle::Collision(CObject& vObj)
{

	if (radius * 2 > LengthPts(vObj.GetPos().x, vObj.GetPos().y, this->position))
	{
		dirVector.Setx(-dirVector.Getx());
		dirVector.Sety(-dirVector.Gety());

	}


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





CStar::CStar(HWND hWnd, POINT pos)
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
	cVector3 a(position.x, position.y - radius, 0);
	vRectPos[0] = a;
	vRectPos[1] = cVector3(position.x, position.y + radius, 0);
	vRectPos[2] = cVector3(position.x - radius, position.y, 0);
	vRectPos[3] = cVector3(position.x + radius, position.y, 0);


}

CStar::~CStar()
{

}


void CStar::Update()
{
	
	position.x = position.x + moveSpeed * dirVector.Getx();
	position.y = position.y + moveSpeed * dirVector.Gety();

	rotate += 5.0;


	double innerRadian = 0;
	double finalAngle = 0;
	for (int i = 0; i < 10; i++)
	{

		finalAngle = degreeToRadian(innerRadian + rotate);

		if (i % 2 == 0)
		{
			vRectPos[i] = cVector3(position.x + cos(finalAngle)*radius, position.y+ sin(finalAngle)*radius, 0);
		}
		else
		{
			vRectPos[i] = cVector3(position.x + cos(finalAngle) * radius/2, position.y+ sin(finalAngle) * radius/2, 0);

		}
		innerRadian += 36;
	}



}

void CStar::Draw(HDC dc)
{

	//DrawRect(dc, position, vRectPos[0], vRectPos[1], vRectPos[2], vRectPos[3]);
	//Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);


	POINT p[10];
	for (int i = 0; i < 10; i++)
	{
		p[i].x = vRectPos[i].Getx();
		p[i].y = vRectPos[i].Gety();
	}
	Polygon(dc, p, 10);
}

bool CStar::Collision(CObject &vObj)
{


	if (radius * 2 > LengthPts(vObj.GetPos().x, vObj.GetPos().y, this->position))
	{
		dirVector.Setx(-dirVector.Getx());
		dirVector.Sety(-dirVector.Gety());

	}

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



double LengthPts(float pt1_x,float pt1_y, POINT pt2)
{
	return (sqrt(
		((float)(pt2.x - pt1_x) * (pt2.x - pt1_x)) +
		((float)(pt2.y - pt1_y) * (pt2.y - pt1_y))
	));
}




//void DrawRect(HDC hdc, POINT center, cVector3 top, cVector3 bot, cVector3 left, cVector3 right)
//{
//	using namespace std;
//	vector<cVector3> vPoint;
//	vPoint.push_back(top);
//	vPoint.push_back(bot);
//	vPoint.push_back(left);
//	vPoint.push_back(right);
//
//
//	for (int i = 0; i < 4; i++)
//	{
//		MoveToEx(hdc, vPoint[i].Getx(), vPoint[i].Gety(), NULL);
//		
//		if (i >= 3)
//			LineTo(hdc, vPoint[0].Getx(), vPoint[0].Gety());
//		else
//			LineTo(hdc, vPoint[i+1].Getx(), vPoint[i + 1].Gety());
//
//
//	}
//	
//
//
//}
