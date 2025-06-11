#pragma once
#include "GameObject.h"

struct ID2D1RenderTarget; // Direct2D 전방 선언

class SPlayer;

class PlayerArm : public GameObject
{

private:

    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;
    PLAYER_CLIMB_STATE m_ePrevClimbState;

    virtual void Update() override;
    virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;

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
