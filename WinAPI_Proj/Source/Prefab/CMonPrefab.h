﻿#pragma once
class CMonster;

enum class MON_TYPE
{
	SHOOTER,
	DEFENDER,

};

class CMonPrefab
{
public:
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);


private:
	CMonPrefab() {}
	~CMonPrefab() {}
};

