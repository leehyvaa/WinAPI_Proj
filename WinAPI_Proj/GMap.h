#pragma once

class GMap
{
	SINGLE(GMap)
public:
	POINT* pMap;
	list<MovePoint> m_enemyZone;

	list<MovePoint> m_firstRail;
	list<MovePoint> m_secondRail;

	list<MovePoint> m_playerRail;
	list<MovePoint> m_tempRail;

public:
	void init();
	void DrawGrid(HDC hdc, POINT center, float width, float height, int xCount, int yCount);
	void DrawMap(HDC hdc);
	void UpdateMap();
};

