#pragma once
#include "GameObject.h"

class SPlayer;

class PlayerArm : public GameObject
{

private:
    float m_fSpeed;
    float m_rotation;
    SPlayer *m_pOwner;
    int m_iDir;
    int m_iPrevDir;
    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;
    PLAYER_CLIMB_STATE m_ePrevClimbState;

    virtual void Update() override;
    virtual void Render(HDC _dc) override;

public:
    void Update_Animation();
    void Update_ClimbAnimation();
    void SetOwner(SPlayer *_owner) { m_pOwner = _owner; }
    void SetPrevDir(int _dir) { m_iPrevDir = _dir; }
    void SetDir(int _dir) { m_iDir = _dir; }
    void SetState(PLAYER_STATE _state) { m_eCurState = _state; }
    void SetPrevState(PLAYER_STATE _state) { m_ePrevState = _state; }

    SPlayer* GetOwner() { return m_pOwner; }

public:
    PlayerArm();
    ~PlayerArm();

    CLONE(PlayerArm)
};
