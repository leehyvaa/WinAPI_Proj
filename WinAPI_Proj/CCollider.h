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

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	void FinalUpdate();
	void Render(HDC _dc);

	//���Կ����� �Լ��� ������ delete �ع���
	CCollider& operator = (CCollider& _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class GameObject;
};

