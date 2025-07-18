﻿#pragma once
#include "Vec2.h"

// Direct2D 전방 선언
struct ID2D1RenderTarget;

class GameObject;

struct AABB {
    Vec2 Min;
    Vec2 Max;
};

class CCollider
{
private:
	static UINT g_iNextID;

    GameObject* m_pOwner;
    Vec2 m_vOffsetPos;     
    Vec2 m_vScale;          // OBB의 전체 너비와 높이 (로컬 축 기준)
    Vec2 m_vHalfExtents;    // OBB의 절반 너비/높이
    Vec2 m_vFinalPos;       // 최종 월드 좌표 (중심점)
    float m_fWorldRotation; // 최종 월드 회전 각도 (라디안)
    Vec2 m_vAxes[2];        // OBB의 월드 기준 로컬 축 벡터(노말)

    AABB m_AABB;  // AABB 캐싱
    Vec2 m_vCorners[4];  // 꼭짓점 저장
    
    UINT m_iID;             // 충돌체의 고유 ID값
    int m_iCol;             // 충돌 횟수 카운터

    bool m_bActive;        
    
public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
    void SetScale(Vec2 _vScale)
	{
	    m_vScale = _vScale;
	    m_vHalfExtents = m_vScale / 2.f; // 절반 크기 업데이트
	}

	UINT GetID() { return m_iID; }
	GameObject* GetObj() { return m_pOwner; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
    Vec2 GetHalfExtents() { return m_vHalfExtents; }
	Vec2 GetFinalPos() { return m_vFinalPos; }
    Vec2* GetAxes() { return m_vAxes; }
    const AABB& GetAABB() const { return m_AABB; }  // AABB 접근자 추가
    

	void FinalUpdate();
	void Render(ID2D1RenderTarget* _pRenderTarget);

	void OnCollision(CCollider* _pOther);
	void OnCollisionEnter(CCollider* pOther);
	void OnCollisionExit(CCollider* pOther);

	// 대입연산자 함수를 못쓰게 delete 해버림
	CCollider& operator = (CCollider& _origin) = delete;

    
    bool IsActive() const { return m_bActive; }
    void SetActive(bool _bActive) { m_bActive = _bActive; }
    
    // 충돌체 초기화 함수
    void Reset() 
    {
        m_iCol = 0;
        m_bActive = true;
        // 위치는 소유자 오브젝트에 따라 결정되므로 리셋 불필요
    }
public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class GameObject;
};

