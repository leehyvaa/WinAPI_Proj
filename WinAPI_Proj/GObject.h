#pragma once
#include "Vec2.h"

enum collisionType
{
	NONE,
	PLAYER,
	ENEMY,
	SPAWNER,
	P_BULLET,
	E_BULLET,
	WALL,
	COMMAND,
};

class GObject
{
public:
	Vec2 m_vPos;
	Vec2 m_vScale;
	Vec2 m_vDir;
	float m_fSpeed;
	int type;
	vector<GObject*> m_CollisionObj;

public:
	GObject();
	virtual ~GObject();
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	

	virtual void Update();
	virtual void Draw();
	virtual bool Collision(GObject& _vObj);

};

