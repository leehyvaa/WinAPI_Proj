#pragma once
#include "GameObject.h"

class CTexture;

class GPlayer : public GameObject
{

private:
	CTexture* m_pTex;
	vector<MovePoint> m_arrMovePoint;
	

	bool m_OnDrawRail;
	bool clockWise;
	bool inBoss;
	PlayerDir updateDir;
public:
	GPlayer();
	GPlayer(Vec2 _vPos, Vec2 _vScale);
	virtual ~GPlayer();

	

	virtual void Update() override;
	virtual void Render(HDC _dc)  override;


	void Damaged();
	void DrawTempRail(POINT _p,PlayerDir dir);
	void DrawRail();
	void DrawCheck(POINT _nextP, POINT _crntP, PlayerDir _dir);

	bool CheckBoss(PlayerDir dir, int i);
	void ResetRail();
	void CalculateDir(PlayerDir enterDir, PlayerDir endDir);
	bool InLine(list<MovePoint>& _line, POINT _point);
	bool InBox(list<MovePoint>& _line, POINT _point);
	int CollisionCount(Vec2 v, Vec2 v2, POINT _p, POINT _rayP);
};

