#pragma once
#include "CState.h"


class CDeadState: public CState
{
public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();

public:
    CDeadState();
    ~CDeadState();
};
