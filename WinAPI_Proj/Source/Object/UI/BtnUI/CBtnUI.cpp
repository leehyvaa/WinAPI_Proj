#include "CBtnUI.h"
#include "CScene_Tool.h"
#include "CTexture.h"
#include "CCore.h"
#include "CCamera.h"
#include <d2d1.h>
CBtnUI::CBtnUI()
	:CUI(false)
	,m_pFunc(nullptr)
	,m_param1(0)
	,m_param2(0)
	,m_pSceneInst(nullptr)
	,m_pSceneFunc(nullptr)

{
}

CBtnUI::CBtnUI(const CBtnUI& _origin)
	: CUI(_origin)
	, m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneInst(nullptr)
	, m_pSceneFunc(nullptr)

{
}

CBtnUI::~CBtnUI()
{
}


void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLbtnDown()
{
}

void CBtnUI::MouseLbtnUp()
{
}

void CBtnUI::MouseLbtnClicked()
{
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_param1,m_param2);
	}

	if (m_pSceneInst && m_pSceneFunc)
	{
		((*m_pSceneInst).*m_pSceneFunc)();
		
	}
}

void CBtnUI::Render(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	// 부모 CUI의 RenderD2D 호출 (버튼 배경 이미지/사각형 렌더링)
	CUI::Render(_pRenderTarget);

	// // 버튼 상태에 따른 추가 시각적 피드백
	// if (IsMouseOn() || IsLbtnDown())
	// {
	// 	Vec2 vPos = GetFinalPos();
	// 	Vec2 vScale = GetScale();
	//
	// 	if (m_bCamAffected)
	// 	{
	// 		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	// 	}
	//
	// 	// 호버/클릭 상태에서 테두리 강조
	// 	ID2D1SolidColorBrush* pHighlightBrush = nullptr;
	// 	D2D1_COLOR_F highlightColor;
	// 	
	// 	if (IsLbtnDown())
	// 	{
	// 		// 클릭 상태: 밝은 노란색 테두리
	// 		highlightColor = D2D1::ColorF(D2D1::ColorF::Yellow, 0.8f);
	// 	}
	// 	else if (IsMouseOn())
	// 	{
	// 		// 호버 상태: 밝은 흰색 테두리
	// 		highlightColor = D2D1::ColorF(D2D1::ColorF::White, 0.6f);
	// 	}
	//
	// 	_pRenderTarget->CreateSolidColorBrush(highlightColor, &pHighlightBrush);
	//
	// 	if (pHighlightBrush)
	// 	{
	// 		D2D1_RECT_F rect = D2D1::RectF(
	// 			vPos.x - 2,
	// 			vPos.y - 2,
	// 			vPos.x + vScale.x + 2,
	// 			vPos.y + vScale.y + 2
	// 		);
	//
	// 		// 강조 테두리 그리기 (2픽셀 두께)
	// 		_pRenderTarget->DrawRectangle(rect, pHighlightBrush, 2.0f);
	// 		
	// 		pHighlightBrush->Release();
	// 	}
	// }
}

void CBtnUI::Update()
{
}

void CBtnUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
{
	m_pSceneInst = _pScene;
	m_pSceneFunc = _pSceneFunc;
}