#include "CBackGround.h"
#include "CCore.h"
#include "CTexture.h"

// Direct2D 헤더 추가
#include <d2d1.h>
#include <wincodec.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "gdiplus.lib")

CBackGround::CBackGround()
	:CTile::CTile()
	, m_pD2DBitmap(nullptr)
{
	SetScale(CCore::GetInst()->GetResolution());
}

CBackGround::~CBackGround()
{
	ReleaseD2DBitmap();
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
	{
		OutputDebugStringA("[DX2D_VERIFY] CBackGround::RenderD2D - RenderTarget is null\n");
		return;
	}

	// D2D 비트맵 캐싱이 안되어 있으면 캐싱 수행
	if (!m_pD2DBitmap)
	{
		OutputDebugStringA("[DX2D_VERIFY] CBackGround::RenderD2D - Caching D2D bitmap\n");
		CacheD2DBitmap(_pRenderTarget);
	}

	if (!m_pD2DBitmap)
	{
		OutputDebugStringA("[DX2D_VERIFY] CBackGround::RenderD2D - D2D bitmap creation failed\n");
		return;
	}

	Vec2 vRenderPos = GetWorldPos();
	Vec2 vScale = GetScale();

	// 렌더링 영역 설정
	D2D1_RECT_F destRect = D2D1::RectF(
		vRenderPos.x,
		vRenderPos.y,
		vRenderPos.x + vScale.x,
		vRenderPos.y + vScale.y
	);

	// Direct2D 비트맵 렌더링 (nearest neighbor로 픽셀 완벽 렌더링)
	_pRenderTarget->DrawBitmap(
		m_pD2DBitmap,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	);
	
	OutputDebugStringA("[DX2D_VERIFY] CBackGround::RenderD2D - Background rendered successfully\n");
}

void CBackGround::CacheD2DBitmap(ID2D1RenderTarget* _pRenderTarget)
{
	ReleaseD2DBitmap();

	CTexture* pTex = GetTexture();
	if (!pTex || !_pRenderTarget)
		return;

	// 정적 WIC Factory (CAnimation 패턴과 동일)
	static IWICImagingFactory* s_pWICFactory = nullptr;
	if (!s_pWICFactory)
	{
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory, (LPVOID*)&s_pWICFactory
		);
		if (FAILED(hr))
			return;
	}

	// GDI+ 비트맵으로 텍스처 로드
	HBITMAP hSourceBitmap = pTex->GetHBITMAP();
	if (!hSourceBitmap)
		return;

	Bitmap sourceGdiplusBitmap(hSourceBitmap, nullptr);
	
	UINT iWidth = pTex->Width();
	UINT iHeight = pTex->Height();
	Vec2 vScale = GetScale();

	// 스케일된 크기로 32비트 ARGB 비트맵 생성
	Bitmap* scaledArgbBitmap = new Bitmap(
		static_cast<int>(vScale.x),
		static_cast<int>(vScale.y),
		PixelFormat32bppARGB
	);
	Graphics frameGraphics(scaledArgbBitmap);

	// 픽셀 깨짐 방지
	frameGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	frameGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	// 투명색 지정 (마젠타)
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

	// 원본 이미지를 스케일된 크기로 투명색 적용 후 그리기
	frameGraphics.DrawImage(
		&sourceGdiplusBitmap,
		Rect(0, 0, static_cast<int>(vScale.x), static_cast<int>(vScale.y)),
		0, 0, iWidth, iHeight,
		UnitPixel,
		&imgAttr
	);

	IWICBitmap* pWICBitmap = nullptr;

	// HBITMAP 추출 및 WIC 변환
	HBITMAP hArgbBitmap = NULL;
	if (scaledArgbBitmap->GetHBITMAP(Color(0, 0, 0, 0), &hArgbBitmap) == Ok)
	{
		// HBITMAP -> WIC 변환
		HRESULT hr = s_pWICFactory->CreateBitmapFromHBITMAP(
			hArgbBitmap, nullptr, WICBitmapUsePremultipliedAlpha, &pWICBitmap
		);
		
		if (SUCCEEDED(hr))
		{
			// WIC -> D2D 비트맵 변환
			hr = _pRenderTarget->CreateBitmapFromWicBitmap(pWICBitmap, nullptr, &m_pD2DBitmap);
		}
		DeleteObject(hArgbBitmap);
	}

	if (pWICBitmap)
		pWICBitmap->Release();
	delete scaledArgbBitmap;
}

void CBackGround::ReleaseD2DBitmap()
{
	if (m_pD2DBitmap)
	{
		m_pD2DBitmap->Release();
		m_pD2DBitmap = nullptr;
	}
}
