#pragma once
class CGround;

enum class GROUND_TYPE
{
	GROUND,
	NONGROUND,
	DAMAGEZONE,
	DEADZONE,
};


class CGroundPrefab
{
public:
	static CGround* CreateGround(GROUND_TYPE _eType, Vec2 _pos1, Vec2 _pos2);

private:
	CGroundPrefab(){}
	~CGroundPrefab() {}

};



