﻿#pragma once
#include "CUI.h"
class CPanelUI : public CUI
{
private:
    Vec2 m_vDragStart;


public:
    virtual void Update();
    virtual void Render(HDC _dc);

    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();


    CLONE(CPanelUI)
    CPanelUI();
    ~CPanelUI();

};

