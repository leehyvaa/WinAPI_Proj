#pragma once
#include "Vec2.h"

class GameObject;

class CCollider
{
private:
	static UINT g_iNextID;

	GameObject* m_pOwner;
	Vec2 m_vOffsetPos;
	Vec2 m_vFinalPos; //finalUpdate���� �� ������ ���
	Vec2 m_vScale;

	UINT m_iID; //�浹ü�� ���� ID��
	int m_iCol;

	bool m_bActive; //�浹ü Ȱ�� ����

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	UINT GetID() { return m_iID; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }

	GameObject* GetObj() { return m_pOwner; }

	void FinalUpdate();
	void Render(HDC _dc);

	void OnCollision(CCollider* _pOther);//�浹 ���� ��� ȣ��
	void OnCollisionEnter(CCollider* pOther);
	void OnCollisionExit(CCollider* pOther);

	//���Կ����� �Լ��� ������ delete �ع���
	CCollider& operator = (CCollider& _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class GameObject;
};

