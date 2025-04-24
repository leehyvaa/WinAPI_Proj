#pragma once
#include "GameObject.h"

class PlayerArm;
class SPlayer;
class CTexture;


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
    Vec2 m_vTargetPos;
    CTexture* pChainTex;
    PLAYER_STATE curState;
    PLAYER_STATE prevState;
    HOOK_STATE hookState;
    bool m_bGroundCollision;
    GROUND_TYPE m_eCollidedGroundType;
    PlayerArm* m_pOwnerArm;
    
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

    void SetTargetPos(Vec2 _pos) { m_vTargetPos = _pos; }
    
    void ReturnToPool();
    virtual void Reset() override;

    void SetOwnerArm(PlayerArm* _pArm) { m_pOwnerArm = _pArm; }
    PlayerArm* GetOwnerArm() { return m_pOwnerArm; }
    
    virtual void LookAt(Vec2 _target) override;
    virtual void OnCollisionEnter(CCollider* _pOther);
    CLONE(CHook);
};

