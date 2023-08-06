#pragma once
#include "CScene.h"
class SecondGameScene1 :public CScene
{
private:
	bool m_bUseForce;
	float m_fForceRadius;
	float m_fCurRadius;
	float m_fForce;
	Vec2 m_vForcePos;

public:
	virtual void Update();
	virtual void Render(HDC _dc);

	virtual void Enter() override;
	virtual void Exit() override;

	void CreateForce();

public:
	SecondGameScene1();
	~SecondGameScene1();

};


