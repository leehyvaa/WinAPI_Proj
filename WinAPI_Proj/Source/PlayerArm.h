#pragma once
#include "GameObject.h"

class SPlayer;

class PlayerArm : public GameObject
{

private:
    float m_fSpeed;

    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;
    PLAYER_CLIMB_STATE m_ePrevClimbState;

    virtual void Update() override;
    virtual void Render(HDC _dc) override;

public:
    void Update_Animation();
    void Update_ClimbAnimation();

    void SetState(PLAYER_STATE _state) { m_eCurState = _state; }
    void SetPrevState(PLAYER_STATE _state) { m_ePrevState = _state; }
    
public:
    PlayerArm();
    ~PlayerArm();

    CLONE(PlayerArm)
};
