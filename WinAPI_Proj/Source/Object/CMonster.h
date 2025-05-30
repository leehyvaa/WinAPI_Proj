#pragma once
#include "GameObject.h"
class CShooterHead;
class AI;

struct tMonInfo
{
    float fSpeed;
    float fHP;
    float fDetectRange;
    float fAttRange;
    float fAtt;
    float fAttackCooldown; 
    int iDir;
};


class CMonster :
    public GameObject
{
private:
    bool m_bOnGround;
    float m_fLastAttackTime; // 초기값 설정 (게임 시작 시 바로 공격 가능하도록)

protected:
    tMonInfo m_tInfo;
    AI* m_pAI;
    CShooterHead* m_pHead;

private:
    void SetMonInfo(const tMonInfo& _info){m_tInfo = _info;}


    
public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    const tMonInfo& GetInfo() { return m_tInfo; }
    AI* GetAI(){return m_pAI;}
    void SetAI(AI* _AI);
    

    virtual void Start();
    virtual void Update();
    virtual void Render(HDC _dc);
    virtual void FinalUpdata();
    virtual void Reset();

    void SetHead(CShooterHead* head){m_pHead = head;}
    CShooterHead* GetHead(){return m_pHead;}
    
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    
    CLONE(CMonster)

    CMonster();
    virtual ~CMonster();

    friend class CMonPrefab;
};

