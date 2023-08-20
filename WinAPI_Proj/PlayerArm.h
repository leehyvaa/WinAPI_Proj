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


    virtual void Update() override;
    virtual void Render(HDC _dc) override;

public:

    void CreateHook();
    void Update_Animation();
    void SetOwner(SPlayer* _owner) { owner = _owner; }

public:
    PlayerArm();
    ~PlayerArm();

    CLONE(PlayerArm)
};

