#pragma once
#include "CUI.h"
#include <vector> // 동적 배열 추가

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
    std::vector<std::wstring> m_vecLines; // 라인별 텍스트 저장
    TEXT_ALIGN    m_Align;     // 정렬 방식
    int           m_iLineSpace; // 라인 간격 (픽셀)
    COLORREF      m_TextColor;
    HFONT         m_hFont;

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
    void SetFontColor(COLORREF color) { m_TextColor = color; }

    virtual void Render(HDC _dc) override;
};


