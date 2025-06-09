#pragma once
#include "GameObject.h"

// Direct2D 전방 선언
struct ID2D1RenderTarget;

class CTexture;
class CMonster;

class CBullet : public GameObject
{
private:
    float m_fSpeed;
    float m_fRange;
    int m_iDamage;
    CMonster* m_pOwnerMonster;
    float m_fDistanceTraveled; // 총알이 이동한 거리

public:
    CBullet();
    virtual ~CBullet() override;

    // 소유자 몬스터에 따라 총알의 속성을 설정
    void SetBulletInfo(GameObject* _pOwner);

    // 오브젝트 풀로 반환
    void ReturnToPool();

    virtual void Update() override;
    virtual void RenderD2D(ID2D1RenderTarget* _pRenderTarget) override;
    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void Reset() override; // 풀에서 재사용 시 초기화

    CLONE(CBullet)
};
