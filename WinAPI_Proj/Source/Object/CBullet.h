#pragma once
#include "GameObject.h"

class CTexture;
class CMonster; // CBullet이 어떤 몬스터에 의해 발사되었는지 알기 위함

class CBullet : public GameObject
{
private:
    float m_fSpeed;
    float m_fRange;
    int m_iDamage;
    CTexture* m_pBulletTex;
    CMonster* m_pOwnerMonster; // 총알을 발사한 몬스터 (소유자)
    float m_fDistanceTraveled; // 총알이 이동한 거리

public:
    CBullet();
    virtual ~CBullet();

    // 소유자 몬스터에 따라 총알의 속성을 설정합니다.
    void SetBulletInfo(GameObject* _pOwner);

    // 오브젝트 풀로 총알을 반환합니다.
    void ReturnToPool();

    virtual void Update() override;
    virtual void Render(HDC _dc) override;
    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void Reset() override; // 풀에서 재사용 시 초기화

    CLONE(CBullet);
};
