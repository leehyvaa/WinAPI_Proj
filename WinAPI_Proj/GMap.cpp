#include "pch.h"
#include "GMap.h"

GMap::GMap()
    :pMap(nullptr)
{
    m_enemyZone.push_back(MovePoint{ 20,20 });
    m_enemyZone.push_back(MovePoint{ 730,20 });
    m_enemyZone.push_back(MovePoint{ 730,980 });
    m_enemyZone.push_back(MovePoint{ 20,980 });


    m_playerRail.push_back(MovePoint{ 20,20 });
    m_playerRail.push_back(MovePoint{ 730,20 });
    m_playerRail.push_back(MovePoint{ 730,980 });
    m_playerRail.push_back(MovePoint{ 20,980 });

    UpdateMap();
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
  
    Polygon(hdc, pMap, m_enemyZone.size());
    

 

}

void GMap::UpdateMap()
{
    //이쪽 작업중이었나?


    pMap = new POINT[m_enemyZone.size()];
    
    list<MovePoint>::iterator iter = m_enemyZone.begin();

    int i = 0;
    for (iter = m_enemyZone.begin(); iter!=m_enemyZone.end(); iter++)
    {
        pMap[i] = POINT{iter->x,iter->y };
        i++;
    }

}
