#include "CPanelUI.h"

#include "CKeyMgr.h"
#include "CCore.h"
#include <d2d1.h>

CPanelUI::CPanelUI()
	:CUI(false)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::Update()
{
}


void CPanelUI::Render(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	// 부모 CUI의 RenderD2D 호출 (패널 배경 렌더링)
	CUI::Render(_pRenderTarget);

	// 필요시 패널에 특별한 효과 추가 (예: 그림자, 테두리 강화 등)
	// 현재는 기본 CUI 렌더링만 사용하여 심플하게 유지
	// 자식 UI들은 CUI::Render_Child_D2D()에서 자동 처리됨
}

void CPanelUI::MouseOn()
{
	if (IsLbtnDown())
	{
		Vec2 vDiff = MOUSE_POS - m_vDragStart;
		Vec2 vCurPos = GetWorldPos();
		vCurPos += vDiff;
		SetWorldPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{


	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}