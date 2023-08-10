#pragma once
#include "CUI.h"

#include "CScene.h"
#include "GameObject.h"
//�Ϲ� �Լ�������
typedef void(*BTN_FUNC)(DWORD_PTR, DWORD_PTR);


//����Լ� �Լ�������
//�� �� �Ʒ��� Ŭ������ �θ�Ŭ������ ������ �ڽ� Ŭ������ ����Լ��� ���� ���� �ִ�.
//���� Ŭ���� ����� ���α׷��� ��� ���̺귯���� �ڽ����� ������ Ŭ���� �ϳ��� ������ �θ�
//�� Ŭ���� ����Լ� �����ͷ� ��� �Լ��� ���� �� �ִ�.
//����Լ��� ����Ҷ� �� Ŭ������ ����� ���� �����޾ƾ� �Ѵ� ���漱������ �ȵ�
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

