#include "pch.h"
#include "SelectGDI.h"
#include "CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType)
	:m_hDC(_dc)
	,m_hDefaultBrush(nullptr)
	,m_hDefaultPen(nullptr)
{
	HPEN hPen = CCore::GetInst()->GetPen(_ePenType);
	m_hDefaultPen = static_cast<HPEN>(SelectObject(_dc, hPen));
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType)
	:m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hBrush = CCore::GetInst()->GetBrush(_eBrushType);
	m_hDefaultBrush = static_cast<HBRUSH>(SelectObject(_dc, hBrush));
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hDC, m_hDefaultPen);
	SelectObject(m_hDC, m_hDefaultBrush);

}
