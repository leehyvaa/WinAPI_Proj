#pragma once
#include "GObject.h"
#include "CEnemy.h"
#include "Player.h"
class CSpawner : public Player
{

private:
	vector<CEnemy*> m_vEnemy;
	vector<CBullet*> m_vBullet;
	int m_iScore;
	float m_fTimer;

public:
	CSpawner();
	CSpawner(Vec2 _vPos, Vec2 _vScale);
	virtual ~CSpawner();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;
	void InstBullet() override;
	void InstEnemy();


};

