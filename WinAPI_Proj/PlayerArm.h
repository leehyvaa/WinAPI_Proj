#pragma once
#include "GameObject.h"

class SPlayer;




class PlayerArm :
    public GameObject
{

private:
    float m_fSpeed;
    float m_rotation;
    SPlayer* owner;
    int dir;
    int prevDir;
    PLAYER_STATE curState;
    PLAYER_STATE prevState;

    virtual void Update() override;
    virtual void Render(HDC _dc) override;

public:


    void Update_Animation();
    void SetOwner(SPlayer* _owner) { owner = _owner; }

    void SetPrevDir(int _dir) { prevDir = _dir; }
    void SetDir(int _dir) { dir = _dir; }

    void SetState(PLAYER_STATE _state) { curState=_state ; }
    void SetPrevState(PLAYER_STATE _state) {prevState = _state; }



public:
    PlayerArm();
    ~PlayerArm();

    CLONE(PlayerArm)
};

