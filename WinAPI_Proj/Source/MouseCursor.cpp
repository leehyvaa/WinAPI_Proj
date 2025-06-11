#include "pch.h"
#include "MouseCursor.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CKeyMgr.h"
#include "CCamera.h"

MouseCursor::MouseCursor()
{
	CreateAnimator();
	CTexture * pCursor = CResMgr::GetInst()->LoadTexture(L"Cursor_Tex", L"texture\\UI\\Cursor.png");
	
	
	GetAnimator()->CreateAnimation(L"Cursor", pCursor,
		Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, 1.1f, Vec2(0.f, 0.f));
	GetAnimator()->FindAnimation(L"Cursor")->Save(L"animation\\cursor.anim");

	GetAnimator()->Play(L"Cursor", true);

}

MouseCursor::~MouseCursor()
{
}

void MouseCursor::Update()
{
	Vec2 pos = GetWorldPos();

	pos = CCamera::GetInst()->GetRealPos(MOUSE_POS);


	SetWorldPos(pos);
}

void MouseCursor::Render(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;
		
	GameObject::Render(_pRenderTarget);
}
