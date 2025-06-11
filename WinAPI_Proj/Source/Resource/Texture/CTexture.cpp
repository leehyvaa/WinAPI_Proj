
#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

// Direct2D 헤더
#include <d2d1.h>
#include <wincodec.h>
#include <gdiplus.h>

using namespace Gdiplus;

// FUTURE: GDI+ 비트맵 추가 멤버 변수 필요
// 향후 성능 개선을 위해 원본 GDI+ 비트맵도 캐싱하는 것을 고려할 수 있음

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
    if (m_pD2DBitmap) // D2D 비트맵 로드가 성공했을 경우 GDI도 시도
    {
        // GDI+ 비트맵을 로드하고 HBITMAP 생성 (픽셀 데이터 수정 없음)
        Gdiplus::Bitmap gdiBmp(_strFilePath.c_str());
        if (gdiBmp.GetLastStatus() == Ok)
        {
            gdiBmp.GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &m_hBit); // 배경색은 중요하지 않음
        }
        else // GDI+ 로드 실패 시 m_hBit, m_dc 등을 초기화하거나 오류 처리
        {
            m_hBit = nullptr; 
            // m_dc는 생성되지 않았으므로 별도 처리 필요 없음
            // m_bitInfo도 초기화된 상태로 둠
            // D2D 비트맵만 로드된 상태이므로, 너비/높이는 D2D 비트맵 기준으로 설정됨 (CreateD2DBitmap에서)
        }

        // m_hBit이 유효한 경우에만 DC 생성 및 비트맵 연결
        if (m_hBit) 
        {
            m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
            SelectObject(m_dc, m_hBit);
            GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
        }
        // 너비와 높이는 m_pD2DBitmap이 있다면 해당 값으로 CreateD2DBitmap에서 이미 설정되었을 것이고,
        // m_hBit만 있다면 여기서 설정 (또는 gdiBmp에서 가져온 값으로 설정)
        // 현재 로직에서는 CreateD2DBitmap이 먼저 호출되므로, m_iWidth, m_iHeight는 D2D 기준으로 설정되어 있음.
        // GDI+ 로드 성공 시에도 D2D와 크기가 같다고 가정. 만약 다를 수 있다면 추가 로직 필요.
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

    // Direct2D 비트맵 생성 전, 마젠타 처리를 위해 WIC 비트맵을 생성
    IWICBitmap* pWICBitmap = nullptr;
    hr = s_pWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnDemand, &pWICBitmap);

    if (SUCCEEDED(hr))
    {
        IWICBitmapLock* pLock = nullptr;
        WICRect rcLock = { 0, 0, (INT)m_iWidth, (INT)m_iHeight };

        // 픽셀 데이터 접근을 위해 비트맵을 잠금
        hr = pWICBitmap->Lock(&rcLock, WICBitmapLockWrite, &pLock);
        if (SUCCEEDED(hr))
        {
            UINT cbBufferSize = 0;
            UINT cbStride = 0;
            BYTE* pData = nullptr;

            pLock->GetStride(&cbStride);
            pLock->GetDataPointer(&cbBufferSize, &pData);

            // 32bpp PBGRA 포맷은 BGRA 순서이므로, 그에 맞게 색상 값 추출
            for (UINT y = 0; y < m_iHeight; ++y)
            {
                DWORD* pPixel = (DWORD*)(pData + y * cbStride);
                for (UINT x = 0; x < m_iWidth; ++x)
                {
                    // 픽셀 포맷은 32bppPBGRA (Premultiplied Blue, Green, Red, Alpha)
                    // 메모리 순서는 BGRA 입니다.
                    BYTE b = (pPixel[x] >> 0) & 0xFF;
                    BYTE g = (pPixel[x] >> 8) & 0xFF;
                    BYTE r = (pPixel[x] >> 16) & 0xFF;
                    
                    if (r == 255 && g == 0 && b == 255)
                    {
                        // 마젠타 픽셀: 알파와 색상 모두 0으로 만들어 완전 투명 처리
                        pPixel[x] = 0;
                    }
                }
            }
            pLock->Release();
        }

        // 수정된 WIC 비트맵으로부터 Direct2D 비트맵 생성
        hr = pRenderTarget->CreateBitmapFromWicBitmap(
            pWICBitmap,
            nullptr,
            &m_pD2DBitmap
        );

        pWICBitmap->Release();
    }


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

ID2D1Bitmap* CTexture::GetSlicedBitmap(const std::wstring& _strKey, const D2D1_RECT_F& _srcRect, const D2D1_SIZE_F& _dstSize)
{
    // 캐시에서 기존 비트맵 검색
    auto iter = m_mapSlicedBitmaps.find(_strKey);
    if (iter != m_mapSlicedBitmaps.end())
    {
        return iter->second;
    }

    // 캐시에 없으면 새로 생성
    ID2D1RenderTarget* pRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
    if (!pRenderTarget || !m_hBit) // m_pD2DBitmap 대신 m_hBit 확인
        return nullptr;

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
            return nullptr;
    }

    // 임시 메모리 DC 생성
    HDC tempDC = CreateCompatibleDC(m_dc);
    if (!tempDC)
        return nullptr;

    // 소스 영역 크기 계산
    int srcWidth = static_cast<int>(_srcRect.right - _srcRect.left);
    int srcHeight = static_cast<int>(_srcRect.bottom - _srcRect.top);

    // 목적지 크기
    int dstWidth = static_cast<int>(_dstSize.width);
    int dstHeight = static_cast<int>(_dstSize.height);

    // 32비트 DIB 섹션 생성 (알파 채널 지원)
    BITMAPINFO bi = {};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = dstWidth;
    bi.bmiHeader.biHeight = -dstHeight; // Top-Down DIB
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    HBITMAP tempBitmap = CreateDIBSection(tempDC, &bi, DIB_RGB_COLORS, &pBits, NULL, 0);

    if (!tempBitmap)
    {
        DeleteDC(tempDC);
        return nullptr;
    }

    HBITMAP oldBitmap = static_cast<HBITMAP>(SelectObject(tempDC, tempBitmap));
    
    // 먼저 원본 영역을 그대로 임시 비트맵에 복사
    StretchBlt(tempDC, 0, 0, dstWidth, dstHeight,
               m_dc, static_cast<int>(_srcRect.left), static_cast<int>(_srcRect.top),
               srcWidth, srcHeight, SRCCOPY);

    SelectObject(tempDC, oldBitmap); // DIB 섹션에 대한 접근을 위해 선택 해제

    // 픽셀 데이터에 직접 접근하여 마젠타를 투명 처리
    UINT magentaColor = 0x00FF00FF; // 0x00RRGGBB
    DWORD* pPixel = (DWORD*)pBits;
    int pixelCount = dstWidth * dstHeight;
    for (int i = 0; i < pixelCount; ++i)
    {
        // 현재 픽셀 색상 (알파 제외)
        UINT pixelColor = pPixel[i] & 0x00FFFFFF;

        if (pixelColor == magentaColor)
        {
            // 마젠타 픽셀: 알파를 0으로 설정 (완전 투명)
            pPixel[i] = 0x00000000;
        }
        else
        {
            // 그 외 픽셀: 알파를 255로 설정 (완전 불투명)
            pPixel[i] |= 0xFF000000;
        }
    }

    // WIC 비트맵 생성
    IWICBitmap* pWICBitmap = nullptr;
    HRESULT hr = s_pWICFactory->CreateBitmapFromHBITMAP(
        tempBitmap,
        nullptr,
        WICBitmapUsePremultipliedAlpha, // Premultiplied Alpha 사용
        &pWICBitmap
    );

    // 임시 리소스 정리
    DeleteObject(tempBitmap);
    DeleteDC(tempDC);

    if (FAILED(hr))
        return nullptr;

    // WIC → D2D 포맷 컨버터
    IWICFormatConverter* pConverter = nullptr;
    hr = s_pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr))
    {
        pWICBitmap->Release();
        return nullptr;
    }

    // 32bpp PBGRA 포맷으로 변환 (Direct2D 호환, Premultiplied Alpha)
    hr = pConverter->Initialize(
        pWICBitmap,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0,
        WICBitmapPaletteTypeMedianCut
    );

    pWICBitmap->Release();

    if (FAILED(hr))
    {
        pConverter->Release();
        return nullptr;
    }

    // Direct2D 비트맵 생성
    ID2D1Bitmap* pSlicedBitmap = nullptr;
    hr = pRenderTarget->CreateBitmapFromWicBitmap(
        pConverter,
        nullptr,
        &pSlicedBitmap
    );

    pConverter->Release();

    if (FAILED(hr))
        return nullptr;

    // 캐시에 저장
    m_mapSlicedBitmaps[_strKey] = pSlicedBitmap;

    return pSlicedBitmap;
}

void CTexture::Release()
{
    // 캐시된 분할 비트맵들 해제
    for (auto& pair : m_mapSlicedBitmaps)
    {
        if (pair.second)
        {
            pair.second->Release();
        }
    }
    m_mapSlicedBitmaps.clear();

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

