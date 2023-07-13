#pragma once
#include "GObject.h"
#include "CBullet.h"
class CEnemy : public GObject
{
private:
	GObject* m_owner;

public:
	CEnemy();
	CEnemy(Vec2 _vPos, Vec2 _vScale, float _speed,GObject& _owner);
	virtual ~CEnemy();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Render(HDC _dc)  override;
	virtual bool Collision(GObject& vObj) override;
	float LengthPts(Vec2 pt1, Vec2 pt2);


};

