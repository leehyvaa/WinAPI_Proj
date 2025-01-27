#include "pch.h"
#include "CScene_Title.h"
#include "CBackGround.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCollisionMgr.h"
#include "MouseCursor.h"

#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CUIMgr.h"

void ChangeScene(DWORD_PTR a, DWORD_PTR b);
void GameOver(DWORD_PTR a, DWORD_PTR b);



CScene_Title::CScene_Title()
{
}

CScene_Title::~CScene_Title()
{
}

void CScene_Title::Update()
{
	CScene::Update();

}



void CScene_Title::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();






	//ī�޶� ��ġ ����
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	CCamera::GetInst()->FadeIn(3.f);




	//��׶���
	CBackGround* backGround = new CBackGround;
	backGround->SetPos(Vec2(0, 0));
	CTexture* back = CResMgr::GetInst()->LoadTexture(L"TitleBack", L"texture\\background\\Title.bmp");

	backGround->SetTexture(back);
	backGround->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround, GROUP_TYPE::BACKGROUND);

	SetBackGround(backGround);


	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"TitleUI");
	pPanelUI->SetScale(Vec2(300.f, 300.f));
	pPanelUI->SetPos(Vec2(800.f, 700.f));
	CTexture* TitleUI = CResMgr::GetInst()->LoadTexture(L"UI_RECT", L"Texture\\UI\\UI_RECT.bmp");
	((CPanelUI*)pPanelUI)->SetTexture(TitleUI);
	
	
	AddObject(pPanelUI, GROUP_TYPE::UI);


	CBtnUI* pBtnStart = new CBtnUI;
	pBtnStart->SetName(L"Btn_Scene_Title");
	pBtnStart->SetScale(Vec2(300.f, 100.f));
	pBtnStart->SetPos(Vec2(50.f, 0.f));
	pBtnStart->SetClickedCallBack(&ChangeScene, 2, 0);

	CTexture* pBtnGameStart = CResMgr::GetInst()->LoadTexture(L"UI_GameStart", L"Texture\\UI\\UI_GameStart.bmp");
	pBtnStart->SetTexture(pBtnGameStart);
	pPanelUI->AddChild(pBtnStart);



	CBtnUI* pBtnMap = pBtnStart->Clone();
	pBtnMap->SetPos(Vec2(50.f, 100.f));
	pBtnMap->SetClickedCallBack(&ChangeScene, 1, 0);
	CTexture* pBtnTexMap = CResMgr::GetInst()->LoadTexture(L"UI_MapEditor", L"Texture\\UI\\UI_MapEditor.bmp");
	pBtnMap->SetTexture(pBtnTexMap);
	pPanelUI->AddChild(pBtnMap);


	CBtnUI* pBtnQuit = pBtnStart->Clone();
	pBtnQuit->SetPos(Vec2(50.f, 200.f));
	pBtnQuit->SetClickedCallBack(&GameOver,0,0);
	CTexture* pBtnTexQuit = CResMgr::GetInst()->LoadTexture(L"UI_GameQuit", L"Texture\\UI\\UI_GameQuit.bmp");
	pBtnQuit->SetTexture(pBtnTexQuit);
	pPanelUI->AddChild(pBtnQuit);


	Start();
}

void CScene_Title::Exit()
{
	CCollisionMgr::GetInst()->Reset();

	DeleteAll();


}






void ChangeScene(DWORD_PTR a, DWORD_PTR b)
{
	ChangeScene((SCENE_TYPE)a);
}

void GameOver(DWORD_PTR a, DWORD_PTR b)
{

	DestroyWindow(CCore::GetInst()->GetMainHwnd());
}

