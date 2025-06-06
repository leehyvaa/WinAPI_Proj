#include "pch.h"
#include "CTexture.h"
#include "CCore.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
CTexture::CTexture()
	:m_hBit(0)
	,m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
    Release();
} 

void CTexture::Load(const wstring& _strFilePath)
{
    // GDI+로 마젠타 배경 보정된 핸들 생성
    Gdiplus::Bitmap gdiBmp(_strFilePath.c_str());
    Gdiplus::Color bgColor(255, 255, 0, 255); // RGB(255,0,255)
    gdiBmp.GetHBITMAP(bgColor, &m_hBit);

    // DC 생성 및 비트맵 연결
    m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
    SelectObject(m_dc, m_hBit);
    GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);

	
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC maindc = CCore::GetInst()->GetMainDC();

	m_hBit = CreateCompatibleBitmap(maindc, _iWidth,_iHeight);
	m_dc = CreateCompatibleDC(maindc);

	HBITMAP hOldBit = static_cast<HBITMAP>(SelectObject(m_dc, m_hBit));
	DeleteObject(hOldBit); 

	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);

}

void CTexture::Release()
{
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
}
