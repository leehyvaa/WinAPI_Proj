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


void CBackGround::Render(ID2D1RenderTarget* _pRenderTarget)
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

