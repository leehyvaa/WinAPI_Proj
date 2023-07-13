#pragma once
#include "GameObject.h"
class GPlayer : public GameObject
{

private:
	vector<int> m_arrTempRail_X;
	vector<int> m_arrTempRail_Y;
	bool m_OnDrawRail;

public:
	GPlayer();
	GPlayer(Vec2 _vPos, Vec2 _vScale);
	virtual ~GPlayer();

	

	virtual void Update() override;
	virtual void Render(HDC _dc)  override;


	void Damaged();
	void DrawRail();
	void ResetRail();
	
};

