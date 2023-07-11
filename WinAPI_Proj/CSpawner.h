#pragma once
#include "GObject.h"
#include "CEnemy.h"
class Player;

class CSpawner :public GObject
{

private:
	vector<CEnemy*> m_vEnemy;
	vector<CBullet*> m_vBullet;
	Player* m_player;

	int m_iScore;
	float m_fStartTimer;

public:
	CSpawner();
	CSpawner(Vec2 _vPos, Vec2 _vScale);
	virtual ~CSpawner();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;
	void InstBullet();
	void InstEnemy();
	void SetPlayer(Player& _player) {m_player = &_player;}

};

