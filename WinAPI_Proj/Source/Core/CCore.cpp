#include "pch.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "GameObject.h"
#include "CTimeMgr.h"
#include "SPlayer.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "SelectGDI.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "SelectGDI.h"
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
	, m_hDC(0)
	, m_pD2DFactory(nullptr)
	, m_pRenderTarget(nullptr)
	, m_pBlackBrush(nullptr)
	, m_pRedBrush(nullptr)
	, m_pGreenBrush(nullptr)
	, m_pBlueBrush(nullptr)
	, m_pDWriteFactory(nullptr)
	, m_arrPen{}
	, m_arrBrush{}
{
}

CCore::~CCore()
{
	ReleaseD2DResources();
	ReleaseDC(m_hWnd,m_hDC); // GetDC로 만든 Dc는 릴리즈로 해제

	for (int i = 0; i < static_cast<UINT>(PEN_TYPE::END); i++)
		DeleteObject(m_arrPen[i]);

	for (int i = 0; i < static_cast<UINT>(BRUSH_TYPE::END); i++)
		DeleteObject(m_arrBrush[i]);

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

	m_hDC = GetDC(m_hWnd);

	// 더블버퍼링 용도의 텍스쳐 한장을 만든다.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer",static_cast<UINT>(m_ptResolution.x), static_cast<UINT>(m_ptResolution.y));

	
	CreateBrushPen();
	CreateD2DResources();

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

    // 순수 Direct2D 렌더링 시작
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
        {
            backGround->RenderD2D(m_pRenderTarget);
        }
        
        // 모든 오브젝트와 UI를 Direct2D로 렌더링
        CSceneMgr::GetInst()->RenderD2D(m_pRenderTarget);
        CCamera::GetInst()->RenderD2D(m_pRenderTarget);

        // 프로파일링 데이터 출력 (F10 키)
        if (CKeyMgr::GetInst()->GetKeyState(KEY::F10) == KEY_STATE::TAP)
        {
            CTimeMgr::RenderProfileDataD2D(m_pRenderTarget, 500); // Direct2D로 출력
        }

        // EndDraw()가 화면 출력까지 담당
        HRESULT hr = m_pRenderTarget->EndDraw();
        if (FAILED(hr) && hr == D2DERR_RECREATE_TARGET)
        {
            CTimeMgr::StartTimer(L"Core_D2D_Recreate");
            ReleaseD2DResources();
            CreateD2DResources();
            CTimeMgr::EndTimer(L"Core_D2D_Recreate");
        }
    }
    CTimeMgr::EndTimer(L"Core_D2D_Render_Total");

    CTimeMgr::GetInst()->Render();

    // 이벤트 지연처리
    CEventMgr::GetInst()->Update();
    
    CTimeMgr::EndTimer(L"Core_Progress_Total");
}

void CCore::CreateBrushPen()
{
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::HOLLOW)] = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::BLACK)] = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::RED)] = CreateSolidBrush(RGB(255, 0, 0));
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::MAGENTA)] = CreateSolidBrush(RGB(255, 0, 255));

    m_arrPen[static_cast<UINT>(PEN_TYPE::RED)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::GREEN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::BLUE)] = CreatePen(PS_SOLID, 1, RGB(80, 183, 220));
    
    m_arrPen[static_cast<UINT>(PEN_TYPE::PURPLE)] = CreatePen(PS_SOLID, 1, RGB(102, 0, 153));
    m_arrPen[static_cast<UINT>(PEN_TYPE::ORANGE)] = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::BIGGREEN)] = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::HOLLOW)] = CreatePen(PS_NULL, 1, 0);
}



void CCore::CreateD2DResources()
{
    ReleaseD2DResources();

    // Direct2D 팩토리 생성
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
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

    // HWND 렌더 타겟 속성 설정
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(
        m_hWnd,
        D2D1::SizeU(static_cast<UINT32>(m_ptResolution.x), static_cast<UINT32>(m_ptResolution.y))
    );

    // HWND 렌더 타겟 생성
    hr = m_pD2DFactory->CreateHwndRenderTarget(props, hwndProps, &m_pRenderTarget);
    if (FAILED(hr))
    {
        if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
        return;
    }
    
    if (m_pRenderTarget)
    {
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &m_pGreenBrush);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBlueBrush);
    }
}


void CCore::ReleaseD2DResources()
{
    if (m_pBlackBrush) { m_pBlackBrush->Release(); m_pBlackBrush = nullptr; }
    if (m_pRedBrush)   { m_pRedBrush->Release();   m_pRedBrush = nullptr; }
    if (m_pGreenBrush) { m_pGreenBrush->Release(); m_pGreenBrush = nullptr; }
    if (m_pBlueBrush)  { m_pBlueBrush->Release();  m_pBlueBrush = nullptr; }

    // 렌더 타겟 해제
    if (m_pRenderTarget) { m_pRenderTarget->Release(); m_pRenderTarget = nullptr; }

    // DirectWrite 팩토리 해제
    if (m_pDWriteFactory) { m_pDWriteFactory->Release(); m_pDWriteFactory = nullptr; }

    // Direct2D 팩토리 해제
    if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
}

void CCore::Clear(HDC _dc)
{
	CScene* curScene = CSceneMgr::GetInst()->GetCurScene();
	CBackGround* backGround = curScene->GetBackGround();
	if (backGround == nullptr)
	{
		// 배경이 없으면 기본 검은색으로 클리어
		SelectGDI gdi(_dc, BRUSH_TYPE::BLACK);
		Rectangle(_dc, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
		return;
	}

	// 하이브리드 렌더링: GDI로 배경 렌더링, Direct2D는 Progress()에서 오버레이
	Vec2 vRenderPos = backGround->GetWorldPos();
	Vec2 vScale = backGround->GetScale();
	
	if (backGround->GetTexture())
	{
		UINT iWidth = backGround->GetTexture()->Width();
		UINT iHeight = backGround->GetTexture()->Height();

		TransparentBlt(_dc
			, static_cast<int>(vRenderPos.x)
			, static_cast<int>(vRenderPos.y)
			, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
			, backGround->GetTexture()->GetDC()
			, 0, 0,
			iWidth, iHeight, RGB(255, 0, 255));
	}
	else
	{
		// 텍스처가 없으면 검은색 배경
		SelectGDI gdi(_dc, BRUSH_TYPE::BLACK);
		Rectangle(_dc, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
	}
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

HDC CCore::GetmemDC()
{
	return m_pMemTex->GetDC();
}




