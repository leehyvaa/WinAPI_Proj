#pragma once
#include "Vec2.h"

class CCollider;

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

	CCollider* m_pCollider;

public:

	ObjectType objectType;

public:
	GameObject();
	virtual ~GameObject();

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void CreateCollider();
	CCollider* GetCollider() {return m_pCollider; }



	virtual void Update() = 0;
	virtual void FinalUpdate() final;
	virtual void Render(HDC _dc);
	void Component_Render(HDC _dc);

};

#pragma once


