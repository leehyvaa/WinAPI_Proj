#pragma once
#include "GObject.h"

class CWall : public GObject
{
private:
	int m_iHp;


public:
	CWall();
	CWall(Vec2 _vPos, Vec2 _vScale);
	virtual ~CWall();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;

	
};

