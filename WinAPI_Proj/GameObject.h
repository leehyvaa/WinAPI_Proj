#pragma once
//오브젝트를 설계할 땐 항상 복사생성자를 커스텀해서 만들어야 하는지를 생각할 것

class CCollider;
class CAnimator;

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

	CCollider* m_pCollider;
	CAnimator* m_pAnimator;


	bool m_bAlive;
public:


public:
	GameObject();
	GameObject(const GameObject& _origin);
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
	void CreateAnimator();
	CAnimator* GetAnimator() { return m_pAnimator; }

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	virtual void Update() = 0;
	virtual void FinalUpdate() final;
	virtual void Render(HDC _dc);
	void Component_Render(HDC _dc);

	virtual GameObject* Clone() = 0;

private:
	void SetDead() { m_bAlive = false; }

	friend class CEventMgr;
};

#pragma once


