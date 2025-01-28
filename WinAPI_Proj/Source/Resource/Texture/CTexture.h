#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{
private:
    HDC m_dc;
    HBITMAP m_hBit;
    BITMAP m_bitInfo;

public:
    void Load(const wstring& _strFilePath);
    void Create(UINT _iWidth, UINT _iHeight);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }
    HDC GetDC() { return m_dc; }
    

private:
    CTexture();
    virtual ~CTexture();

    //텍스처 클래스의 생성자를 private으로 해 아무데서나 생성할 수 없게 하고
    //friend 처리된 CResMgr에서만 생성할 수 있게 함
    friend class CResMgr;
};

