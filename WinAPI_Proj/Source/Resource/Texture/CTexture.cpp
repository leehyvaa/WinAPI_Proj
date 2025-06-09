#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

// Direct2D 헤더
#include <d2d1.h>
#include <wincodec.h>
#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Gdiplus.lib")

CTexture::CTexture()
    : m_hBit(0)
    , m_dc(0)
    , m_bitInfo{}
    , m_pD2DBitmap(nullptr)
    , m_iWidth(0)
    , m_iHeight(0)
{
}

CTexture::~CTexture()
{
    Release();
}

void CTexture::Load(const wstring& _strFilePath)
{
    // Direct2D 우선 시도
    CreateD2DBitmap(_strFilePath);
    
    // 하위 호환성을 위한 GDI 리소스도 생성
    if (m_pD2DBitmap)
    {
        // GDI+로 마젠타 배경 보정된 핸들 생성 (기존 호환성)
        Gdiplus::Bitmap gdiBmp(_strFilePath.c_str());
        Gdiplus::Color bgColor(255, 255, 0, 255); // RGB(255,0,255)
        gdiBmp.GetHBITMAP(bgColor, &m_hBit);

        // DC 생성 및 비트맵 연결
        m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
        SelectObject(m_dc, m_hBit);
        GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
    }
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
    // Direct2D 비트맵 생성
    CreateD2DBitmapFromSize(_iWidth, _iHeight);
    
    // 하위 호환성을 위한 GDI 리소스도 생성
    HDC maindc = CCore::GetInst()->GetMainDC();
    m_hBit = CreateCompatibleBitmap(maindc, _iWidth, _iHeight);
    m_dc = CreateCompatibleDC(maindc);

    HBITMAP hOldBit = static_cast<HBITMAP>(SelectObject(m_dc, m_hBit));
    DeleteObject(hOldBit);

    GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

void CTexture::CreateD2DBitmap(const wstring& _strFilePath)
{
    ID2D1RenderTarget* pRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
    if (!pRenderTarget)
        return;

    // WIC Factory 정적 관리 (성능 최적화)
    static IWICImagingFactory* s_pWICFactory = nullptr;
    if (!s_pWICFactory)
    {
        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&s_pWICFactory)
        );
        if (FAILED(hr))
            return;
    }

    // WIC 디코더로 이미지 로딩
    IWICBitmapDecoder* pDecoder = nullptr;
    HRESULT hr = s_pWICFactory->CreateDecoderFromFilename(
        _strFilePath.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );

    if (FAILED(hr))
        return;

    // 첫 번째 프레임 가져오기
    IWICBitmapFrameDecode* pFrameDecode = nullptr;
    hr = pDecoder->GetFrame(0, &pFrameDecode);
    if (FAILED(hr))
    {
        pDecoder->Release();
        return;
    }

    // 이미지 크기 가져오기
    UINT width, height;
    pFrameDecode->GetSize(&width, &height);
    m_iWidth = width;
    m_iHeight = height;

    // WIC → D2D 포맷 컨버터
    IWICFormatConverter* pConverter = nullptr;
    hr = s_pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr))
    {
        pFrameDecode->Release();
        pDecoder->Release();
        return;
    }

    // 32bpp BGRA 포맷으로 변환 (Direct2D 호환)
    hr = pConverter->Initialize(
        pFrameDecode,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0,
        WICBitmapPaletteTypeMedianCut
    );

    if (FAILED(hr))
    {
        pConverter->Release();
        pFrameDecode->Release();
        pDecoder->Release();
        return;
    }

    // Direct2D 비트맵 생성
    hr = pRenderTarget->CreateBitmapFromWicBitmap(
        pConverter,
        nullptr,
        &m_pD2DBitmap
    );

    // 리소스 해제
    pConverter->Release();
    pFrameDecode->Release();
    pDecoder->Release();
}

void CTexture::CreateD2DBitmapFromSize(UINT _iWidth, UINT _iHeight)
{
    ID2D1RenderTarget* pRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
    if (!pRenderTarget)
        return;

    m_iWidth = _iWidth;
    m_iHeight = _iHeight;

    // 빈 비트맵 생성 (렌더 타겟용)
    D2D1_SIZE_U size = D2D1::SizeU(_iWidth, _iHeight);
    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );

    HRESULT hr = pRenderTarget->CreateBitmap(size, nullptr, 0, props, &m_pD2DBitmap);
}

void CTexture::ReleaseD2DResources()
{
    if (m_pD2DBitmap)
    {
        m_pD2DBitmap->Release();
        m_pD2DBitmap = nullptr;
    }
}

void CTexture::Release()
{
    // Direct2D 리소스 해제
    ReleaseD2DResources();
    
    // 기존 GDI 리소스 해제
    if (m_dc)
    {
        DeleteDC(m_dc);
        m_dc = nullptr;
    }
    if (m_hBit)
    {
        DeleteObject(m_hBit);
        m_hBit = nullptr;
    }
    
    // 크기 정보 초기화
    m_iWidth = 0;
    m_iHeight = 0;
    m_bitInfo = {};
}
