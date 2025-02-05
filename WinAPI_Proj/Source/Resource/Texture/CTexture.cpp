#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

CTexture::CTexture()
	:m_hBit(0)
	,m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
} 

void CTexture::Load(const wstring& _strFilePath)
{
	// 로딩한 데이터를 비트맵으로 생성
	m_hBit = static_cast<HBITMAP>(LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0,
                                            LR_CREATEDIBSECTION | LR_LOADFROMFILE));
	assert(m_hBit);

	//비트맵과 연결할 dc
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	//비트맵과 dc 연결
	HBITMAP hPrevBit = static_cast<HBITMAP>(SelectObject(m_dc, m_hBit));
	DeleteObject(hPrevBit);

	//비트맵 정보를 구조체멤버에 저장
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
