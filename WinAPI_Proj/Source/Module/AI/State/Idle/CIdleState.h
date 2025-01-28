#pragma once
#include "CState.h"


class CIdleState :
    public CState
{
private:



public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();

public:
    CIdleState();
    ~CIdleState();
};

