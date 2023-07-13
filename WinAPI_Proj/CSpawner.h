#pragma once
#include "GObject.h"
#include "CEnemy.h"
class Player;

class CSpawner :public GObject
{
public:
	int m_iScore;
private:
	vector<CEnemy*> m_vEnemy;
	vector<CBullet*> m_vBullet;
	Player* m_player;

	
	float m_fStartTimer;
	float m_fDifficult;
	float m_fSpawnTimer;
	int m_iSpawnCount;

	bool m_bDamaged;
	float m_fDamagedCount;
	float m_fStartDmgTimer;

public:
	CSpawner();
	CSpawner(Vec2 _vPos, Vec2 _vScale);
	virtual ~CSpawner();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Render(HDC _dc)  override;
	virtual bool Collision(GObject& vObj) override;
	void InstBullet();
	void InstEnemy();
	void SetPlayer(Player& _player) {m_player = &_player;}

};

