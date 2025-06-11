#include "CBackGround.h"
#include "CCore.h"
#include "CTexture.h"

// Direct2D 헤더 추가
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

CBackGround::CBackGround()
	:CTile::CTile()
{
	SetScale(CCore::GetInst()->GetResolution());
}

CBackGround::~CBackGround()
{
}

void CBackGround::Render(HDC _dc)
{
	// Direct2D 활성화 시 GDI 렌더링 스킵 (하이브리드 렌더링)
	if (CCore::GetInst()->GetD2DRenderTarget())
		return;

	CTexture* pTileTex = GetTexture();

	if (nullptr == pTileTex)
		return;

	Vec2 vRenderPos = GetWorldPos();
	Vec2 vScale = GetScale();

	UINT iWidth = pTileTex->Width();
	UINT iHeight = pTileTex->Height();

	TransparentBlt(_dc
		, static_cast<int>(vRenderPos.x)
		, static_cast<int>(vRenderPos.y)
		, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
		, pTileTex->GetDC()
		, 0, 0,
		iWidth, iHeight, RGB(255, 0, 255));
}

void CBackGround::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	CTexture* m_pTex = GetTexture();
	if (!m_pTex)
		return;

	// CTexture에서 D2D 비트맵 가져오기
	ID2D1Bitmap* pD2DBitmap = m_pTex->GetD2DBitmap();
	if (!pD2DBitmap)
		return;

	// 렌더 타겟 크기 가져오기 (화면 전체에 맞게 스케일링)
	D2D1_SIZE_F renderTargetSize = _pRenderTarget->GetSize();
	
	// 목적지 사각형 설정 (화면 전체)
	D2D1_RECT_F destRect = D2D1::RectF(
		0.0f,
		0.0f,
		renderTargetSize.width,
		renderTargetSize.height
	);

	// Direct2D 비트맵 렌더링
	_pRenderTarget->DrawBitmap(
		pD2DBitmap,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	);
}

