#pragma once
#include "GameObject.h"
class CUI :
    public GameObject
{
private:
    vector<CUI*> m_vecChildUI;
    CUI* m_pParentUI;

    Vec2 m_vFinalPos;

    bool m_bCamAffected; //UI�� ī�޶� ������ �޴� ����
    bool m_bMouseOn; //���콺 UI ���� �ִ���
    bool m_bLbtnDown; //UI�� ���ʹ�ư�� ���� �� �ִ���

public:
   
    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }


    void AddChild(CUI* _pUI) {m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this;}
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }


private:
    void Update_Child();
    void FinalUpdate_Child();
    void Render_Child(HDC _dc);

    void MouseOnCheck();


public:
    virtual void MouseOn();

    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();



public:
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render(HDC _dc);



    virtual CUI* Clone() = 0;
    CUI(bool _bCamAff);
    CUI(const CUI& _origin);

    virtual ~CUI();
    

    friend class CUIMgr;
};

