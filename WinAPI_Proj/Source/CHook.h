#pragma once
#include "GameObject.h"

class SPlayer;



enum class HOOK_STATE
{
    FLYING,
    GRAB,
    GRABBING,
    RETURN_WITH,
    RETURN_WITHOUT,

};

class CHook :
    public GameObject
{
private:
    float m_fSpeed;
    float m_fMaxRange;
    int dir;
    int prevDir;
    PLAYER_STATE curState;
    PLAYER_STATE prevState;
    HOOK_STATE hookState;
    bool m_bGroundCollision;
    GROUND_TYPE m_eCollidedGroundType;

public:
    CHook();
    virtual ~CHook();

    virtual void Update();
    virtual void Render(HDC _dc);


    void Update_Animation();
    void Update_State();
    void Update_Move();

    void SetPrevDir(int _dir) { prevDir = _dir; }
    void SetDir(int _dir) { dir = _dir; }

    void SetHookState(HOOK_STATE _state) { hookState = _state; }
    HOOK_STATE GetHookState() { return hookState; }

    void SetState(PLAYER_STATE _state) { curState = _state; }
    void SetPrevState(PLAYER_STATE _state) { prevState = _state; }

    
    void ReturnToPool();
    virtual void Reset() override;
    
    virtual void OnCollisionEnter(CCollider* _pOther);
    CLONE(CHook);
};

