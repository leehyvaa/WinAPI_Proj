#pragma once
class CGround;




class CGroundPrefab
{
public:
	static CGround* CreateGround(GROUND_TYPE _eType, Vec2 _pos1, Vec2 _pos2);

private:
	CGroundPrefab(){}
	~CGroundPrefab() {}

};



