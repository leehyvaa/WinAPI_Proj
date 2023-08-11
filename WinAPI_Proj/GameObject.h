#pragma once
//������Ʈ�� ������ �� �׻� ��������ڸ� Ŀ�����ؼ� ������ �ϴ����� ������ ��


#include "global.h"
#include "CCamera.h"
class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;




class GameObject
{
private:
	wstring m_strName;

	Vec2 m_vPos;
	Vec2 m_vScale;

	CCollider* m_pCollider;
	CAnimator* m_pAnimator;
	CRigidBody* m_pRigidBody;
	CGravity* m_pGravity;

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
	void CreateRigidBody();
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	void CreateGravity();
	CGravity* GetGravity() { return m_pGravity; }


	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};


	virtual void Start() {}; //�� ���۵Ǳ� ������ ȣ��Ǵ��Լ�
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);

	void Component_Render(HDC _dc);

	virtual GameObject* Clone() = 0;

private:
	void SetDead() { m_bAlive = false; }

	friend class CEventMgr;
};

#pragma once


