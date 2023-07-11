#pragma once
#include "GObject.h"
#include "Vec2.h"

class CBullet : public GObject
{
private:
	GObject* m_owner;

public:
	CBullet();
	CBullet(Vec2 _vDir, Vec2 _vPos,float _speed,GObject& m_owner,int _type);
	virtual ~CBullet();

	Vec2 GetPos() { return m_vPos; }

	float LengthPts(Vec2 pt1, Vec2 pt2);
	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;
};

