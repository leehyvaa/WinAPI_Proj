#pragma once


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
	wstring m_strName;

	Vec2 m_vPos;
	Vec2 m_vScale;
	//Vec2 m_vDir;

	CCollider* m_pCollider;

	bool m_bAlive;
public:

	ObjectType objectType;

public:
	GameObject();
	virtual ~GameObject();

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }


	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	bool IsDead() { return !m_bAlive; }

	void CreateCollider();
	CCollider* GetCollider() {return m_pCollider; }


	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	virtual void Update() = 0;
	virtual void FinalUpdate() final;
	virtual void Render(HDC _dc);
	void Component_Render(HDC _dc);


private:
	void SetDead() { m_bAlive = false; }

	friend class CEventMgr;
};

#pragma once


