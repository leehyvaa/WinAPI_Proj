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

void CBtnUI::Render(HDC _dc)
{
	// Direct2D 렌더링이 활성화된 경우 GDI 렌더링 스킵
	ID2D1RenderTarget* pD2DRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
	if (pD2DRenderTarget != nullptr)
	{
		// Direct2D 모드에서는 RenderD2D가 호출됨
		return;
	}

	CUI::Render(_dc);

	/*BitBlt(_dc
		, (int)(vPos.x )
		, (int)(vPos.y )
		, (int)vScale.x
		, (int)vScale.y
		, m_pBtnTex->GetDC()
		, 0
		, 0
		, SRCCOPY);*/

	//BLENDFUNCTION bf = {};
	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.SourceConstantAlpha = 127; //전역적으로 적용되는 알파

	//AlphaBlend(_dc
	//, (int)(vRenderPos.x)
	//, (int)(vRenderPos.y)
	//, int(iWidth), int(iHeight)
	//, m_pBtnTex->GetDC()
	//, 0, 0, int(iWidth), int(iHeight)
	//, bf);
}

void CBtnUI::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	// 부모 CUI의 RenderD2D 호출 (버튼 배경 이미지/사각형 렌더링)
	CUI::RenderD2D(_pRenderTarget);

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