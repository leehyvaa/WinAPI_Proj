#include "pch.h"
#include "GMap.h"

GMap::GMap()
{
    m_arrMap[0] = { 20,20 };
    m_arrMap[1] = { 730,20 };
    m_arrMap[2] = { 730,980 };
    m_arrMap[3] = { 20,980 };

}

GMap::~GMap()
{
}

void GMap::init()
{
  

   
}


void GMap::DrawGrid(HDC hdc, POINT center, float width, float height, int xCount, int yCount)
{
    POINT start = { center.x - width / 2 , center.y - height / 2 };
    POINT end = { center.x + width / 2 , center.y + height / 2 };


    float xInterval = width / xCount;
    float yInterval = height / yCount;

    for (int i = 0; i < xCount + 1; i++)
    {
        MoveToEx(hdc, start.x + xInterval * i, start.y, NULL);
        LineTo(hdc, start.x + xInterval * i, end.y);
    }

    for (int i = 0; i < yCount + 1; i++)
    {
        MoveToEx(hdc, start.x, start.y + yInterval * i, NULL);
        LineTo(hdc, end.x, start.y + yInterval * i);
    }

}

void GMap::DrawMap(HDC hdc)
{
  
    Polygon(hdc, m_arrMap, 4);

 

}

void GMap::UpdateMap()
{
   

}
