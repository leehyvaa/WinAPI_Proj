#pragma once

#include "CRes.h"
#include <unordered_map>
#include <d2d1.h>

// Direct2D 전방 선언
struct ID2D1Bitmap;
struct ID2D1RenderTarget;

class CTexture :
    public CRes
{
private:
    // Direct2D 멤버 (PNG 파일의 알파 채널 지원)
    ID2D1Bitmap* m_pBitmap;
    UINT m_iWidth;
    UINT m_iHeight;

    // 분할된 비트맵 캐시 (Direct2D 기반)
    unordered_map<wstring, ID2D1Bitmap*> m_mapSlicedBitmaps;

public:
    void Load(const wstring& _strFilePath);
    void Create(UINT _iWidth, UINT _iHeight);
    

    // Direct2D 인터페이스
    ID2D1Bitmap* GetBitmap() { return m_pBitmap; }
    UINT Width() { return m_iWidth; }
    UINT Height() { return m_iHeight; }

    bool IsValid() const { return (m_pBitmap != nullptr && m_iWidth > 0 && m_iHeight > 0); }

    // 분할된 비트맵 검색/캐싱 함수
    ID2D1Bitmap* GetSlicedBitmap(const std::wstring& _strKey, const D2D1_RECT_F& _srcRect, const D2D1_SIZE_F& _dstSize);

    void Release();

private:
    // Direct2D 관련 private 메서드
    void CreateBitmap(const wstring& _strFilePath);
    void CreateBitmapFromSize(UINT _iWidth, UINT _iHeight);
    void ReleaseResources();

private:
    CTexture();
    virtual ~CTexture();

    //텍스처 클래스의 생성자를 private으로 해 아무데서나 생성할 수 없게 하고
    //friend 처리된 CResMgr에서만 생성할 수 있게 함
    friend class CResMgr;
};