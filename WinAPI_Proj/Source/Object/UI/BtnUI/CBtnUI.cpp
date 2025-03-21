﻿#include "CBtnUI.h"
#include "CScene_Tool.h"
#include "CTexture.h"
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

void CBtnUI::Update()
{
}

void CBtnUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
{
	m_pSceneInst = _pScene;
	m_pSceneFunc = _pSceneFunc;
}