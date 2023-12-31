#include "CObject.h"
#include <cstdlib>
#include <ctime>

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
	radius = 50;

	/*POINT leftTop = {position.x - radius / 2 , position.y - radius / 2 };
	POINT leftBot = { position.x - radius / 2 , position.y + radius / 2 };
	POINT rightBot = { position.x + radius / 2 , position.y + radius / 2 };
	POINT rightTop = { position.x + radius / 2 , position.y - radius / 2 };*/


	rect.left = position.x - radius;
	rect.right = position.x + radius;
	rect.top = position.y - radius;
	rect.bottom = position.y + radius;
}

CRectangle::~CRectangle()
{

}


void CRectangle::Update()
{
	Collision();
	position.x = position.x + moveSpeed * dirVector.Getx();
	position.y = position.y + moveSpeed * dirVector.Gety();

	rect.left = position.x - radius;
	rect.right = position.x + radius;
	rect.top = position.y - radius;
	rect.bottom = position.y + radius;

	
}

void CRectangle::Draw(HDC dc)
{

	
	Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);

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