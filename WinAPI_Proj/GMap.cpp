#include "pch.h"
#include "GMap.h"

GMap::GMap()
	:m_arrMap{ 0 }
{

}

GMap::~GMap()
{
}

void GMap::init()
{
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 760; j++)
		{
            m_arrMap[i][j] = Rail;
         
			if(i<=9 || i >= 990)
				m_arrMap[i][j] = Rail;
			else if (j <= 9 || i >= 750)
				m_arrMap[i][j] = Rail;
            else if (i == 4 || i == 995)
                m_arrMap[i][j] = Wall;
            else if (j == 4 || i == 755)
                m_arrMap[i][j] = Wall;

            
		}
	}
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