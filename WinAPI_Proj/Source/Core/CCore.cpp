﻿#include "pch.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "GameObject.h"
#include "CTimeMgr.h"
#include "SPlayer.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "resource.h"
#include "CScene.h"
#include "CBackGround.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CAnimation.h"
#include "CAnimator.h"

CCore::CCore()
	:m_hWnd(0)
	, m_ptResolution{}
	, m_pFactory(nullptr)
	, m_pRenderTarget(nullptr)
	, m_pDWriteFactory(nullptr)
{
}

CCore::~CCore()
{
	ReleaseResources();
	DestroyMenu(m_hMenu);
}





int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 조절
	ChangeWindowSize(m_ptResolution, false);
	
	// 메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPIPROJ));


	CreateResources();

	// Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();


	// Sound 로드 테스트
	CResMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\BGM_Title.wav");
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	pNewSound->PlayToBGM(true);
	// pNewSound->Play();

	// pNewSound->SetPosition(50.f); //백분율, 소리 위치 설정
	pNewSound->SetVolume(20.f);


	return S_OK;
}



void CCore::Progress()
{
    CTimeMgr::StartTimer(L"Core_Progress_Total");
    
    // Manager 업데이트
    CTimeMgr::StartTimer(L"Core_Managers_Update");
    CTimeMgr::GetInst()->Update();
    CKeyMgr::GetInst()->Update();
    CCamera::GetInst()->Update();
    CTimeMgr::EndTimer(L"Core_Managers_Update");

    // 씬 업데이트, 충돌처리
    CTimeMgr::StartTimer(L"Core_Scene_Update");
    CSceneMgr::GetInst()->Update();
    CCollisionMgr::GetInst()->Update();
    CTimeMgr::EndTimer(L"Core_Scene_Update");

    // UI 이벤트 체크
    CTimeMgr::StartTimer(L"Core_UI_Update");
    CUIMgr::GetInst()->Update();
    CTimeMgr::EndTimer(L"Core_UI_Update");

    // Direct2D 렌더링 시작
    CTimeMgr::StartTimer(L"Core_D2D_Render_Total");
    if (m_pRenderTarget)
    {
        m_pRenderTarget->BeginDraw();
        
        // 배경 클리어
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        
        // 배경 렌더링
        CScene* curScene = CSceneMgr::GetInst()->GetCurScene();
        CBackGround* backGround = curScene->GetBackGround();
        if (backGround)
            backGround->Render(m_pRenderTarget);

        
        // 모든 오브젝트와 UI를 렌더링
        CSceneMgr::GetInst()->Render(m_pRenderTarget);
        CCamera::GetInst()->Render(m_pRenderTarget);

        // 프로파일링 디버그 출력 ( P 키)
        if (CKeyMgr::GetInst()->GetKeyState(KEY::P) == KEY_STATE::TAP)
        {
            CTimeMgr::RenderProfileData(m_pRenderTarget, 500);
        }

        // EndDraw()가 화면 출력까지 담당
        HRESULT hr = m_pRenderTarget->EndDraw();
        if (FAILED(hr) && hr == D2DERR_RECREATE_TARGET)
        {
            CTimeMgr::StartTimer(L"Core_D2D_Recreate");
            // 디바이스 손실 처리
            CBrushManager::GetInst()->OnDeviceLost();
            ReleaseResources();
            CreateResources();
            CTimeMgr::EndTimer(L"Core_D2D_Recreate");
        }
    }
    CTimeMgr::EndTimer(L"Core_D2D_Render_Total");

    // 이벤트 지연처리
    CEventMgr::GetInst()->Update();
    
    CTimeMgr::EndTimer(L"Core_Progress_Total");
}

void CCore::CreateResources()
{
    ReleaseResources();

    // Dx2D 팩토리 생성
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
    if (FAILED(hr)) return;

    // DirectWrite 팩토리 생성
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
    );
    if (FAILED(hr))
    {
        OutputDebugStringA("DirectWrite Factory creation failed\n");
    }

    // HWND 렌더 타겟 속성 설정
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(
        m_hWnd,
        D2D1::SizeU(static_cast<UINT32>(m_ptResolution.x), static_cast<UINT32>(m_ptResolution.y))
    );

    // HWND 렌더 타겟 생성
    hr = m_pFactory->CreateHwndRenderTarget(props, hwndProps, &m_pRenderTarget);
    if (FAILED(hr))
    {
        if (m_pFactory) { m_pFactory->Release(); m_pFactory = nullptr; }
        return;
    }
    
    // CBrushManager 초기화
    if (m_pRenderTarget)
    {
        CBrushManager::GetInst()->Initialize(m_pRenderTarget);
    }
}


void CCore::ReleaseResources()
{
    // CBrushManager 해제
    CBrushManager::GetInst()->Release();

    // 렌더 타겟 해제
    if (m_pRenderTarget) { m_pRenderTarget->Release(); m_pRenderTarget = nullptr; }

    // DirectWrite 팩토리 해제
    if (m_pDWriteFactory) { m_pDWriteFactory->Release(); m_pDWriteFactory = nullptr; }

    // Dx2D 팩토리 해제
    if (m_pFactory) { m_pFactory->Release(); m_pFactory = nullptr; }
}

void CCore::DockMenu()
{
	// 메뉴바 장착
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);

}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rect = { 0,0, static_cast<long>(_vResolution.x), static_cast<long>(_vResolution.y)};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
}
