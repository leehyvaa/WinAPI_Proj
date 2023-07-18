#pragma once
#include "GameObject.h"
enum PlayerDir
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	DEFAULT,
};
struct MovePoint
{
	int x;
	int y;
	PlayerDir dir;
	
};

class GPlayer : public GameObject
{

private:
	vector<MovePoint> m_arrMovePoint;
	vector<int> m_arrTempRail_X;
	vector<int> m_arrTempRail_Y;

	bool m_OnDrawRail;
	bool clockWise;
	bool inBoss;
	PlayerDir enterDir;
public:
	GPlayer();
	GPlayer(Vec2 _vPos, Vec2 _vScale);
	virtual ~GPlayer();

	

	virtual void Update() override;
	virtual void Render(HDC _dc)  override;


	void Damaged();
	void DrawRail();
	void DrawEnemyZone(PlayerDir dir, int i, MapType type, MapType typeTwo);
	void ResetRail();
	void CalculateDir(PlayerDir enterDir, PlayerDir endDir);
};

