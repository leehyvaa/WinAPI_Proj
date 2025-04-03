#pragma once
#include "GameObject.h"
class CTexture;


class CUI :
    public GameObject
{
private:
    vector<CUI*> m_vecChildUI;
    CUI* m_pParentUI;

    Vec2 m_vFinalPos;
    BRUSH_TYPE m_BoxColor;
    PEN_TYPE m_BorderColor;

    CTexture* m_pTex;
    int m_iTexIndex;

    
    bool m_bMouseOn; //마우스 UI 위에 있는지
    bool m_bLbtnDown; //UI가 왼쪽버튼이 눌린 적 있는지

protected:
    bool m_bCamAffected; //UI가 카메라에 영향을 받는 유무
    bool m_bVisibleBox;

public:
    

    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParentUI() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }


    void AddChild(CUI* _pUI) {m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this;}
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }

    CTexture* GetTexture() { return m_pTex; }
    void SetTexture(CTexture* _pTex) { m_pTex = _pTex; }
    void SetIndex(int _index) { m_iTexIndex = _index; }
    void SetBoxColor(BRUSH_TYPE color) { m_BoxColor = color; }
    void SetBorderColor(PEN_TYPE color) { m_BorderColor = color; }
    void SetVisibleBox(bool b) { m_bVisibleBox = b; }

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

