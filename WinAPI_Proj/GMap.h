#pragma once

class GMap
{
	SINGLE(GMap)
public:
	POINT m_arrMap[4];
	list<MovePoint> m_playerRail;
	list<MovePoint> m_tempRail;

public:
	void init();
	void DrawGrid(HDC hdc, POINT center, float width, float height, int xCount, int yCount);
	void DrawMap(HDC hdc);
	void UpdateMap();
};

