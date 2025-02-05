#include "CUI.h"

#include "CKeyMgr.h"
#include "CCamera.h"
#include "SelectGDI.h"
#include "CTexture.h"
CUI::CUI(bool _bCamAff)
	:m_pParentUI(nullptr)
	,m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_pTex(nullptr)
	, m_iTexIndex(-1)
    , m_bVisibleBox(true)
    , m_BorderColor(PEN_TYPE::GREEN)
{
}

CUI::CUI(const CUI& _origin)
	:GameObject(_origin)
	,m_pParentUI(nullptr)
	,m_bCamAffected(_origin.m_bCamAffected)
	,m_bMouseOn(false)
	,m_bLbtnDown(false)
	, m_pTex(nullptr)
	, m_iTexIndex(-1)
{
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); i++)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}

}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}



void CUI::Update()
{

	// 자식 ui 업데이트
	Update_Child();
}

void CUI::FinalUpdate()
{
	GameObject::FinalUpdate();

	//ui의 최종 좌표를 구한다.
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	//UI Mouse 체크
	MouseOnCheck();

	
	
	FinalUpdate_Child();

}

void CUI::Render(HDC _dc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	


	if (m_pTex != nullptr)
	{
		UINT iWidth = m_pTex->Width();
		UINT iHeight = m_pTex->Height();


		TransparentBlt(_dc
			, static_cast<int>(vPos.x)
			, static_cast<int>(vPos.y)
			, iWidth, iHeight
			, m_pTex->GetDC()
			, 0, 0, iWidth, iHeight, RGB(0, 0, 0));


	}
	else if(m_bVisibleBox)
	{
        SelectGDI select(_dc, m_BoxColor);
        SelectGDI select2(_dc, m_BorderColor);


		if (m_bLbtnDown)
		{
            SelectGDI select(_dc, PEN_TYPE::GREEN);
			Rectangle(_dc, static_cast<int>(vPos.x), static_cast<int>(vPos.y), static_cast<int>(vPos.x + vScale.x), static_cast<int>(vPos.y + vScale.y));
		}
		else
		{
			Rectangle(_dc, static_cast<int>(vPos.x), static_cast<int>(vPos.y), static_cast<int>(vPos.x + vScale.x), static_cast<int>(vPos.y + vScale.y));
		}
	}

	//자식 ui 렌더
	Render_Child(_dc);
}


void CUI::Update_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}



void CUI::Render_Child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->Render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	}

	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}
