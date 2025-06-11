// CTextUI.cpp
#include "CTextUI.h"
#include "CCore.h"
#include "CCamera.h"
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

CTextUI::CTextUI()
    : CUI(false) // 카메라 영향 받지 않음
    , m_hFont(nullptr)
    , m_iFontSize(15)
    , m_pTextFormat(nullptr)
    , m_pTextBrush(nullptr)
    , m_TextColor(D2D1::ColorF(0.625f, 0.27f, 0.625f, 1.0f)) // 보라색
{
    m_bVisibleBox = true;
}

CTextUI::~CTextUI()
{
    // GDI 리소스 해제
    if (m_hFont)
        DeleteObject(m_hFont);
        
    // DirectWrite 리소스 해제
    if (m_pTextFormat)
    {
        m_pTextFormat->Release();
        m_pTextFormat = nullptr;
    }
    
    if (m_pTextBrush)
    {
        m_pTextBrush->Release();
        m_pTextBrush = nullptr;
    }
}

void CTextUI::AddLine(const std::wstring& _str)
{
    m_vecLines.push_back(_str);
}

void CTextUI::AddLines(const std::vector<std::wstring>& _vec)
{
    if (_vec.empty()) return;
    
    try {
        m_vecLines.insert(m_vecLines.end(), _vec.begin(), _vec.end());
    }
    catch (std::exception& e) {
        OutputDebugStringA(e.what());
    }
}
void CTextUI::ClearLines()
{
    if(this && !m_vecLines.empty())
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
        CreateTextFormat(); // DirectWrite TextFormat도 재생성
    }
}

// DirectWrite 관련 메서드 구현
void CTextUI::SetFontColor(D2D1_COLOR_F _color)
{
    m_TextColor = _color;
    
    // 기존 브러시가 있으면 해제하여 다음 렌더링 시 새로 생성
    if (m_pTextBrush)
    {
        m_pTextBrush->Release();
        m_pTextBrush = nullptr;
    }
}

void CTextUI::CreateTextFormat()
{
    // 기존 TextFormat 해제
    if (m_pTextFormat)
    {
        m_pTextFormat->Release();
        m_pTextFormat = nullptr;
    }

    IDWriteFactory* pDWriteFactory = CCore::GetInst()->GetDWriteFactory();
    if (!pDWriteFactory)
        return;

    // 텍스트 정렬 변환
    DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
    switch (m_Align)
    {
    case TEXT_ALIGN::CENTER:
        textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
        break;
    case TEXT_ALIGN::RIGHT:
        textAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
        break;
    case TEXT_ALIGN::LEFT:
    default:
        textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
        break;
    }

    // DirectWrite TextFormat 생성
    HRESULT hr = pDWriteFactory->CreateTextFormat(
        L"Arial",                      // 폰트 이름
        nullptr,                       // 폰트 컬렉션 (기본값)
        DWRITE_FONT_WEIGHT_NORMAL,     // 폰트 굵기
        DWRITE_FONT_STYLE_NORMAL,      // 폰트 스타일
        DWRITE_FONT_STRETCH_NORMAL,    // 폰트 늘림
        static_cast<float>(m_iFontSize), // 폰트 크기
        L"",                           // 로케일
        &m_pTextFormat
    );

    if (SUCCEEDED(hr) && m_pTextFormat)
    {
        // 텍스트 정렬 설정
        m_pTextFormat->SetTextAlignment(textAlignment);
        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }
}

void CTextUI::CreateTextBrush(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget || m_pTextBrush)
        return;

    // DirectWrite 텍스트 브러시 생성
    HRESULT hr = _pRenderTarget->CreateSolidColorBrush(
        m_TextColor,
        &m_pTextBrush
    );

    if (FAILED(hr))
    {
        OutputDebugStringA("CTextUI::CreateTextBrush - 텍스트 브러시 생성 실패\n");
    }
}

void CTextUI::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    // 기본 UI 렌더링 (배경/테두리)
    CUI::Render(_pRenderTarget);

    // DirectWrite 리소스 준비
    if (!m_pTextFormat)
    {
        CreateTextFormat();
    }
    
    if (!m_pTextBrush)
    {
        CreateTextBrush(_pRenderTarget);
    }

    if (!m_pTextFormat || !m_pTextBrush || m_vecLines.empty())
        return;

    Vec2 vPos = GetFinalPos();
    Vec2 vScale = GetScale();
    
    if (m_bCamAffected)
    {
        vPos = CCamera::GetInst()->GetRenderPos(vPos);
    }

    // 텍스트 라인들을 하나의 문자열로 합치기 (라인 간격 고려)
    wstring combinedText;
    for (size_t i = 0; i < m_vecLines.size(); ++i)
    {
        if (i > 0)
        {
            combinedText += L"\n";
        }
        combinedText += m_vecLines[i];
    }

    // **FIX: 스케일이 0인 경우 텍스트 내용에 기반한 적절한 크기 계산**
    if (vScale.x <= 0.0f || vScale.y <= 0.0f)
    {
        // 텍스트 라인 수에 기반한 높이 계산
        float lineCount = static_cast<float>(m_vecLines.size());
        float estimatedLineHeight = static_cast<float>(m_iFontSize + m_iLineSpace);
        
        // 가장 긴 라인을 찾아 너비 추정
        float maxLineWidth = 0.0f;
        for (const auto& line : m_vecLines)
        {
            float estimatedWidth = line.length() * m_iFontSize * 0.6f; // 평균 문자 너비 추정
            maxLineWidth = max(maxLineWidth, estimatedWidth);
        }
        
        // 최소 크기 보장 및 적절한 여백 추가
        vScale.x = max(maxLineWidth + 20.0f, 200.0f); // 최소 200픽셀 너비
        vScale.y = max(lineCount * estimatedLineHeight + 10.0f, 50.0f); // 최소 50픽셀 높이
    }

    // DirectWrite 텍스트 렌더링 영역 설정
    D2D1_RECT_F textRect = D2D1::RectF(
        vPos.x,
        vPos.y,
        vPos.x + vScale.x,
        vPos.y + vScale.y
    );


    // DirectWrite로 텍스트 렌더링
    _pRenderTarget->DrawText(
        combinedText.c_str(),
        static_cast<UINT32>(combinedText.length()),
        m_pTextFormat,
        textRect,
        m_pTextBrush
    );
}