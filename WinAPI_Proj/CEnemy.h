#pragma once
#include "GObject.h"
#include "CBullet.h"
class CEnemy : public GObject
{
private:
	int m_iHp;

public:
	CEnemy();
	CEnemy(Vec2 _vPos, Vec2 _vScale, float _speed);
	virtual ~CEnemy();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;
	void Damaged(float _damage);

};

