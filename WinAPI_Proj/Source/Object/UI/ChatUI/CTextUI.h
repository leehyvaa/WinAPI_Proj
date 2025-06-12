#pragma once
#include "CUI.h"
#include <vector> // 동적 배열 추가
#include <dwrite.h>

struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class CTextUI : public CUI
{
public:
    enum class TEXT_ALIGN  // 텍스트 정렬 방식 열거형
    {
        LEFT,
        CENTER,
        RIGHT
    };

private:
    int m_iFontSize;
    vector<wstring> m_vecLines; // 라인별 텍스트 저장
    TEXT_ALIGN    m_Align;     // 정렬 방식
    int           m_iLineSpace; // 라인 간격 (픽셀)
    HFONT         m_hFont;

    // DirectWrite 관련 멤버 변수
    IDWriteTextFormat* m_pTextFormat;     // DirectWrite 텍스트 포맷
    ID2D1SolidColorBrush* m_pTextBrush;   // DirectWrite 텍스트 브러시
    D2D1_COLOR_F  m_TextColor;         // Direct2D 텍스트 색상

public:
    CTextUI();
    ~CTextUI();
    CLONE(CTextUI)



    // 텍스트 관리 메서드들
    void CreateFontHandle();
    void AddLine(const std::wstring& _str);       // 단일 라인 추가
    void AddLines(const std::vector<std::wstring>& _vec); // 여러 라인 추가
    void ClearLines();                            // 모든 라인 삭제
    void SetFont(HFONT newFont) { m_hFont = newFont ;}
    void SetAlign(TEXT_ALIGN _align) { m_Align = _align; }
    void SetLineSpace(int _space) { m_iLineSpace = _space; }

    // 기존 SetText 오버로딩 (단일 문자열 → 자동 개행 분할)
    void SetText(const std::wstring& _str, wchar_t _delimiter = L'\n');
    void SetFontSize(int _size);
    int GetFontSize() const { return m_iFontSize; }

    // DirectWrite 관련 메서드
    void SetFontColor(D2D1_COLOR_F _color);
    void CreateTextFormat();
    void CreateTextBrush(ID2D1RenderTarget* _pRenderTarget);
    
    virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;
};


