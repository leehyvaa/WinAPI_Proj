﻿
#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

// Direct2D 헤더
#include <d2d1.h>
#include <wincodec.h>

// PNG 파일 지원으로 마이그레이션 완료
// - 알파 채널을 지원하는 PNG 파일 사용
// - 마젠타 색상 투명 처리 로직 제거 (PNG 알파 채널 사용)
// - Direct2D 기반 텍스처 로딩 및 캐싱



CTexture::CTexture()
    : m_pBitmap(nullptr)
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
    // Direct2D로 PNG 파일 로딩 (알파 채널 지원)
    CreateBitmap(_strFilePath);

    // 텍스처 로딩 실패 시 로그 출력
    if (!m_pBitmap || m_iWidth == 0 || m_iHeight == 0)
    {
        // 디버깅: 파일 로딩 실패 로그
        string debugPath = string(_strFilePath.begin(), _strFilePath.end());
        OutputDebugStringA(("CTexture::Load - FAILED to load: " + debugPath + "\n").c_str());

        // 로딩 실패 시 기본값으로 초기화
        m_iWidth = 0;
        m_iHeight = 0;
        return;
    }

}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
    // Direct2D 비트맵 생성
    CreateBitmapFromSize(_iWidth, _iHeight);
}

void CTexture::CreateBitmap(const wstring& _strFilePath)
{
    ID2D1RenderTarget* pRenderTarget = CCore::GetInst()->GetRenderTarget();
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

    // PNG 파일은 알파 채널을 지원하므로 마젠타 색상 변환 없이 직접 Direct2D 비트맵 생성
    hr = pRenderTarget->CreateBitmapFromWicBitmap(
        pConverter,
        nullptr,
        &m_pBitmap
    );

    // 리소스 해제
    pConverter->Release();
    pFrameDecode->Release();
    pDecoder->Release();
}

void CTexture::CreateBitmapFromSize(UINT _iWidth, UINT _iHeight)
{
    ID2D1RenderTarget* pRenderTarget = CCore::GetInst()->GetRenderTarget();
    if (!pRenderTarget)
        return;

    m_iWidth = _iWidth;
    m_iHeight = _iHeight;

    // 빈 비트맵 생성 (렌더 타겟용)
    D2D1_SIZE_U size = D2D1::SizeU(_iWidth, _iHeight);
    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );

    HRESULT hr = pRenderTarget->CreateBitmap(size, nullptr, 0, props, &m_pBitmap);
}

void CTexture::ReleaseResources()
{
    if (m_pBitmap)
    {
        m_pBitmap->Release();
        m_pBitmap = nullptr;
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
    ID2D1RenderTarget* pRenderTarget = CCore::GetInst()->GetRenderTarget();
    if (!pRenderTarget || !m_pBitmap)
        return nullptr;

    // 목적지 크기
    UINT dstWidth = static_cast<UINT>(_dstSize.width);
    UINT dstHeight = static_cast<UINT>(_dstSize.height);

    // 임시 렌더 타겟 생성 (분할된 비트맵을 그릴 용도)
    ID2D1BitmapRenderTarget* pBitmapRenderTarget = nullptr;
    HRESULT hr = pRenderTarget->CreateCompatibleRenderTarget(
        D2D1::SizeF(static_cast<float>(dstWidth), static_cast<float>(dstHeight)),
        &pBitmapRenderTarget
    );

    if (FAILED(hr))
        return nullptr;

    // 임시 렌더 타겟에 원본 텍스처의 일부분을 그리기
    pBitmapRenderTarget->BeginDraw();
    pBitmapRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f)); // 투명한 배경

    // 목적지 사각형 (전체 크기)
    D2D1_RECT_F destRect = D2D1::RectF(0.0f, 0.0f, _dstSize.width, _dstSize.height);

    // 원본 텍스처의 지정된 영역을 목적지 크기로 그리기
    pBitmapRenderTarget->DrawBitmap(
        m_pBitmap,
        destRect,
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
        _srcRect
    );

    hr = pBitmapRenderTarget->EndDraw();
    if (FAILED(hr))
    {
        pBitmapRenderTarget->Release();
        return nullptr;
    }

    // 렌더 타겟에서 비트맵 가져오기
    ID2D1Bitmap* pSlicedBitmap = nullptr;
    hr = pBitmapRenderTarget->GetBitmap(&pSlicedBitmap);

    pBitmapRenderTarget->Release();

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
    ReleaseResources();
    

    // 크기 정보 초기화
    m_iWidth = 0;
    m_iHeight = 0;
}

