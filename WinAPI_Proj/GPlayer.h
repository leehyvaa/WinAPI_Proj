#pragma once
#include "GameObject.h"


class GPlayer : public GameObject
{

private:
	vector<MovePoint> m_arrMovePoint;

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
	bool CheckBoss(PlayerDir dir, int i);
	void ResetRail();
	void CalculateDir(PlayerDir enterDir, PlayerDir endDir);
	bool InLine(list<MovePoint>& _line, POINT _point);
	bool InBox(const POINT* _box, int _count, POINT _point);

};

