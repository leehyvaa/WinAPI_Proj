#pragma once
#include "CUI.h"

#include "CScene.h"
#include "GameObject.h"
//일반 함수포인터
typedef void(*BTN_FUNC)(DWORD_PTR, DWORD_PTR);


//멤버함수 함수포인터
//이 때 아래의 클래스를 부모클래스를 적으면 자식 클래스의 멤버함수도 받을 수가 있다.
//따라서 클래스 설계시 프로그램의 모든 라이브러리를 자식으로 가지는 클래스 하나를 선언해 두면
//이 클래스 멤버함수 포인터로 모든 함수를 받을 수 있다.
//멤버함수를 사용할땐 그 클래스의 헤더를 위에 참조받아야 한다 전방선언만으론 안됨
typedef void(CScene::* SCENE_MEMFUNC)(void);
typedef void(GameObject::* OBJECT_MEMFUNC)(void);

class CTexture;

class CBtnUI :
    public CUI
{
private:
    BTN_FUNC m_pFunc;
    DWORD_PTR m_param1;
    DWORD_PTR m_param2;

    SCENE_MEMFUNC m_pSceneFunc;
    CScene* m_pSceneInst;

    CTexture* m_pBtnTex;
    int m_iTexIndex;

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();


    virtual void Render(HDC _dc);
    virtual void Update();


    void SetTexture(CTexture* _pTex) { m_pBtnTex = _pTex; }
    void SetIndex(int _index) { m_iTexIndex = _index; }

    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
    {
        m_pFunc = _pFunc;
        m_param1 = _param1;
        m_param2 = _param2;

    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);



    CLONE(CBtnUI)
    CBtnUI(const CBtnUI& _origin);
    CBtnUI();
    ~CBtnUI();


};

