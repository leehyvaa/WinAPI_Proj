#pragma once
#include "CState.h"

class CAimingState : public CState
{
public:
    CAimingState();
    ~CAimingState();
    
private:
    
public:
    virtual void Enter();
    virtual void Update();
    virtual void Exit();
};
