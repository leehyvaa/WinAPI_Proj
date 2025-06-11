#include "CUI.h"

#include "CKeyMgr.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CCore.h"
#include <d2d1.h>
#include <wincodec.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Gdiplus.lib")
CUI::CUI(bool _bCamAff)
	:m_pParentUI(nullptr)
	,m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_pTex(nullptr)
	, m_iTexIndex(-1)
    , m_bVisibleBox(true)
    , m_BorderColor(D2D1::ColorF(D2D1::ColorF::Green))
{
}

CUI::CUI(const CUI& _origin)
	:GameObject(_origin)
	,m_pParentUI(nullptr)
	,m_bCamAffected(_origin.m_bCamAffected)
	,m_bMouseOn(false)
	,m_bLbtnDown(false)
	, m_pTex(nullptr)
	, m_iTexIndex(-1)
{
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); i++)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}

}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}



void CUI::Update()
{

	// 자식 ui 업데이트
	Update_Child();
}

void CUI::FinalUpdate()
{
	GameObject::FinalUpdate();

	//ui의 최종 좌표를 구한다.
	m_vFinalPos = GetWorldPos();

	if (GetParentUI())
	{
		Vec2 vParentPos = GetParentUI()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	//UI Mouse 체크
	MouseOnCheck();

	
	
	FinalUpdate_Child();

}

void CUI::Render(HDC _dc)
{
	// Direct2D 렌더링이 활성화된 경우 GDI 렌더링 스킵
	ID2D1RenderTarget* pD2DRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
	if (pD2DRenderTarget != nullptr)
	{
		// Direct2D 모드에서는 RenderD2D가 호출됨
		return;
	}

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_pTex != nullptr)
	{
		UINT iWidth = m_pTex->Width();
		UINT iHeight = m_pTex->Height();

		TransparentBlt(_dc
			, static_cast<int>(vPos.x)
			, static_cast<int>(vPos.y)
			, iWidth, iHeight
			, m_pTex->GetDC()
			, 0, 0, iWidth, iHeight, RGB(0, 0, 0));
	}
	else if(m_bVisibleBox)
	{
		// GDI 모드에서는 기본 사각형만 그리기 (Direct2D 우선 사용)
		HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 128));
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		
		HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
		
		Rectangle(_dc
			, (int)vPos.x, (int)vPos.y
			, (int)(vPos.x + vScale.x), (int)(vPos.y + vScale.y));
			
		SelectObject(_dc, hOldBrush);
		SelectObject(_dc, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);
	}

	// 자식 UI 렌더링
	Render_Child(_dc);
}

void CUI::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_pTex != nullptr)
	{
		// CTexture에서 캐시된 D2D 비트맵을 직접 가져와서 렌더링
		ID2D1Bitmap* pD2DBitmap = m_pTex->GetD2DBitmap();
		if (pD2DBitmap)
		{
			UINT iWidth = m_pTex->Width();
			UINT iHeight = m_pTex->Height();

			D2D1_RECT_F destRect = D2D1::RectF(
				vPos.x,
				vPos.y,
				vPos.x + iWidth,
				vPos.y + iHeight
			);

			_pRenderTarget->DrawBitmap(
				pD2DBitmap,
				destRect,
				1.0f,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
			);
		}
	}
	else if (m_bVisibleBox)
	{
		// 텍스처가 없는 경우 Direct2D로 사각형 그리기
		ID2D1SolidColorBrush* pBrush = nullptr;
		ID2D1SolidColorBrush* pBorderBrush = nullptr;

		// 박스 색상 설정 (임시로 기본 색상 사용)
		D2D1_COLOR_F boxColor = D2D1::ColorF(D2D1::ColorF::LightGray, 0.5f);
		D2D1_COLOR_F borderColor = D2D1::ColorF(D2D1::ColorF::Green);

		if (m_bLbtnDown)
		{
			borderColor = D2D1::ColorF(D2D1::ColorF::LightGreen);
		}

		_pRenderTarget->CreateSolidColorBrush(boxColor, &pBrush);
		_pRenderTarget->CreateSolidColorBrush(borderColor, &pBorderBrush);

		if (pBrush && pBorderBrush)
		{
			D2D1_RECT_F rect = D2D1::RectF(
				vPos.x,
				vPos.y,
				vPos.x + vScale.x,
				vPos.y + vScale.y
			);

			// 배경 채우기
			_pRenderTarget->FillRectangle(rect, pBrush);

			// 테두리 그리기
			_pRenderTarget->DrawRectangle(rect, pBorderBrush, 1.0f);
		}

		if (pBrush) pBrush->Release();
		if (pBorderBrush) pBorderBrush->Release();
	}

	// 자식 UI 렌더링
	Render_Child_D2D(_pRenderTarget);
}


void CUI::Render_Child_D2D(ID2D1RenderTarget* _pRenderTarget)
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->RenderD2D(_pRenderTarget);
	}
}


void CUI::Update_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}



void CUI::Render_Child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->Render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	}

	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}
