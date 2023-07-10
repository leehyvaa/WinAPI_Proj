#pragma once
#include "GObject.h"
#include "Player.h"
#include "Vec2.h"

class CBullet : public GObject
{
private:
	

public:
	CBullet();
	CBullet(Vec2 _vDir, Vec2 _vPos,float _speed);
	virtual ~CBullet();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;
};

