#pragma once
#include "GameObject.h"
class CUI :
    public GameObject
{
private:
    vector<CUI*> m_vecChildUI;
    CUI* m_pParentUI;

    Vec2 m_vFinalPos;

public:
   
    Vec2 GetFinalPos() { return m_vFinalPos; }
     
    
    CUI* GetParent() { return m_pParentUI; }
    void AddChild(CUI* _pUI) {m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this;}



private:
    void Update_Child();
    void FinalUpdate_Child();
    void Render_Child(HDC _dc);

public:
    virtual void MouseOn();

    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();



public:
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render(HDC _dc);


    CUI();
    virtual ~CUI();
    CLONE(CUI);
};

