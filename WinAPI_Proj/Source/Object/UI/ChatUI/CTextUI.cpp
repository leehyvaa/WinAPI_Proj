// CTextUI.cpp
#include "CTextUI.h"

CTextUI::CTextUI()
    : CUI(false) // 카메라 영향 받지 않음
    , m_TextColor(RGB(0, 255, 0)) // 기본 흰색
    , m_hFont(nullptr)
    , m_iFontSize(15)
{
    m_bVisibleBox = true;
}

CTextUI::~CTextUI()
{
    if (m_hFont)
        DeleteObject(m_hFont);
}

void CTextUI::AddLine(const std::wstring& _str)
{
    m_vecLines.push_back(_str);
}

void CTextUI::AddLines(const std::vector<std::wstring>& _vec)
{
    m_vecLines.insert(m_vecLines.end(), _vec.begin(), _vec.end());
}

void CTextUI::ClearLines()
{
    m_vecLines.clear();
}

void CTextUI::SetText(const std::wstring& _str, wchar_t _delimiter)
{
    m_vecLines.clear();

    size_t start = 0;
    size_t end = _str.find(_delimiter);

    while (end != std::wstring::npos)
    {
        m_vecLines.push_back(_str.substr(start, end - start));
        start = end + 1;
        end = _str.find(_delimiter, start);
    }
    m_vecLines.push_back(_str.substr(start));
}

void CTextUI::CreateFontHandle()
{
    if (m_hFont)
        DeleteObject(m_hFont);

    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = -m_iFontSize; // 폰트 높이 설정
    lf.lfWeight = FW_NORMAL;    // 폰트 두께
    wcscpy_s(lf.lfFaceName, L"Arial"); // 기본 글꼴

    m_hFont = CreateFontIndirect(&lf);
}

void CTextUI::SetFontSize(int _size)
{
    if (m_iFontSize != _size && _size > 0)
    {
        m_iFontSize = _size;
        CreateFontHandle(); // 새 폰트 생성
    }
}

void CTextUI::Render(HDC _dc)
{
    CUI::Render(_dc); // 기본 UI 렌더
    if (!m_hFont) {
        CreateFontHandle();
    }
    // 배경 투명화 추가
    SetBkMode(_dc, TRANSPARENT); 
    SetBkColor(_dc, RGB(0, 0, 0)); 

    Vec2 vPos = GetFinalPos();
    if (m_bCamAffected)
        vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // 폰트/색상 설정
    HFONT oldFont = (HFONT)SelectObject(_dc, m_hFont);
    COLORREF oldColor = SetTextColor(_dc, m_TextColor);

    // 라인별 렌더링
    SIZE txtSize = { 0 };
    for (size_t i = 0; i < m_vecLines.size(); ++i)
    {
        const std::wstring& line = m_vecLines[i];

        // 텍스트 크기 측정
        GetTextExtentPoint32(_dc, line.c_str(), (int)line.length(), &txtSize);

        // 정렬 계산
        int x = (int)vPos.x;
        switch (m_Align)
        {
        case TEXT_ALIGN::CENTER:
            x += (GetScale().x - txtSize.cx) / 2;
            break;
        case TEXT_ALIGN::RIGHT:
            x += GetScale().x - txtSize.cx;
            break;
        }

        // Y 위치 계산 (라인 간격 적용)
        int y = (int)vPos.y + (txtSize.cy + m_iLineSpace) * (int)i;

        TextOutW(_dc, x, y, line.c_str(), (int)line.length());
    }

    // GDI 객체 복원
    SetTextColor(_dc, oldColor);
    SelectObject(_dc, oldFont);
}