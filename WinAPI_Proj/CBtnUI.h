#pragma once
#include "CUI.h"
class CBtnUI :
    public CUI
{
private:


public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();



    CLONE(CBtnUI)
    CBtnUI();
    ~CBtnUI();


};

