#pragma once
#include "CRes.h"

// Direct2D 전방 선언
struct ID2D1Bitmap;
struct ID2D1RenderTarget;

class CTexture :
    public CRes
{
private:
    // 기존 GDI 멤버 (하위 호환성)
    HDC m_dc;
    HBITMAP m_hBit;
    BITMAP m_bitInfo;
    
    // Direct2D 멤버
    ID2D1Bitmap* m_pD2DBitmap;
    UINT m_iWidth;
    UINT m_iHeight;

public:
    void Load(const wstring& _strFilePath);
    void Create(UINT _iWidth, UINT _iHeight);

    // 기존 GDI 인터페이스 (호환성)
    HBITMAP GetHBITMAP() { return m_hBit; }
    BITMAP GetBitMapInfo() {return m_bitInfo;}
    HDC GetDC() { return m_dc; }
    
    // Direct2D 인터페이스
    ID2D1Bitmap* GetD2DBitmap() { return m_pD2DBitmap; }
    UINT Width() { return m_iWidth; }
    UINT Height() { return m_iHeight; }
    
    void Release();

private:
    // Direct2D 관련 private 메서드
    void CreateD2DBitmap(const wstring& _strFilePath);
    void CreateD2DBitmapFromSize(UINT _iWidth, UINT _iHeight);
    void ReleaseD2DResources();

private:
    CTexture();
    virtual ~CTexture();

    //텍스처 클래스의 생성자를 private으로 해 아무데서나 생성할 수 없게 하고
    //friend 처리된 CResMgr에서만 생성할 수 있게 함
    friend class CResMgr;
};

