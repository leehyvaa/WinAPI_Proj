#pragma once
#include "GObject.h"

class CommandCenter :public GObject
{
private:
	int m_iHp;


public:
	CommandCenter();
	CommandCenter(Vec2 _vPos, Vec2 _vScale);
	virtual ~CommandCenter();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;

};

