#pragma once
#include "GameObject.h"
class AI;

struct tMonInfo
{
    float fSpeed;
    float fHP;
    float fDetectRange;
    float fAttRange;
    float fAtt;
    int iDir;
};


class CMonster :
    public GameObject
{
private:
    tMonInfo m_tInfo;
    

    AI* m_pAI;


public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    const tMonInfo& GetInfo() { return m_tInfo; }
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void SetAI(AI* _AI);

private:
    void SetMonInfo(const tMonInfo& _info){m_tInfo = _info;}

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

    virtual void Update();

    CLONE(CMonster);

    CMonster();
    ~CMonster();

    friend class CMonPrefab;
};

