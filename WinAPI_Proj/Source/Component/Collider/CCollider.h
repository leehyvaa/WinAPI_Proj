#pragma once
#include "Vec2.h"

class GameObject;

class CCollider
{
private:
	static UINT g_iNextID;

	GameObject* m_pOwner;
	Vec2 m_vOffsetPos;
	Vec2 m_vFinalPos; //finalUpdate에서 매 프레임 계산
	Vec2 m_vScale;

	UINT m_iID; //충돌체의 고유 ID값
	int m_iCol;

	bool m_bActive; //충돌체 활성 여부

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

	void OnCollision(CCollider* _pOther);//충돌 중인 경우 호출
	void OnCollisionEnter(CCollider* pOther);
	void OnCollisionExit(CCollider* pOther);

	//대입연산자 함수를 못쓰게 delete 해버림
	CCollider& operator = (CCollider& _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class GameObject;
};

