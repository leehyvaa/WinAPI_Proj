#include "pch.h"
#include "CTool.h"

float CTool::LengthPts(Vec2 pt1, Vec2 pt2)
{
	return (sqrt(
		((float)(pt2.x - pt1.x) * (pt2.x - pt1.x)) +
		((float)(pt2.y - pt1.y) * (pt2.y - pt1.y))
	));
}
