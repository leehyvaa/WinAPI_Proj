#pragma once
#include "GObject.h"
#include "CBullet.h"
class CSpawner;

class Player : public GObject
{
private:
	
	vector<CBullet*> m_vBullet;
	vector<GObject*> m_target;
	CSpawner* m_spawner;
	float m_fStartTimer;

	
public:
	Player();
	Player(Vec2 _vPos, Vec2 _vScale);
	virtual ~Player();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Render(HDC _dc)  override;
	virtual bool Collision(GObject& vObj) override;
	virtual void InstBullet();
	
	void Damaged();
	void SetSpawner(CSpawner& _spawner) { m_spawner = &_spawner; }
};

