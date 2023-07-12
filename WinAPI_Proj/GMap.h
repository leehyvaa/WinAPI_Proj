#pragma once
class GMap
{
	SINGLE(GMap)
public:
	int m_arrMap[1000][760];



public:
	void init();
	void DrawGrid(HDC hdc, POINT center, float width, float height, int xCount, int yCount);

};

