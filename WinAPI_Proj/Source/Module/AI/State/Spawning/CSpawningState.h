#pragma once
#include "CState.h"


class CSpawningState : public CState
{
private:



public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();

public:
    CSpawningState();
    ~CSpawningState();
};
