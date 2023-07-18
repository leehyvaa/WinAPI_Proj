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


class GameObject
{
private:
	Vec2 m_vPos;
	Vec2 m_vScale;
	Vec2 m_vDir;

public:
	vector<GameObject*> m_target;
	ObjectType objectType;

public:
	GameObject();
	virtual ~GameObject();

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetTarget(GameObject* _target);


	virtual void Update() = 0;
	virtual void Render(HDC _dc);


	//virtual bool Collision(GameObject& _vObj);

};

#pragma once


