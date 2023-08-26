#include "pch.h"
#include "CHook.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CAnimation.h"
CHook::CHook()
	:m_fSpeed(700)
	,m_vDir(Vec2(1.f,1.f))
{
	m_vDir.Normalize();

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(20.f, 20.f));

	CreateAnimator();
#pragma region 후크 애니메이션
	//텍스쳐 로딩
	CTexture* pTexRight = CResMgr::GetInst()->LoadTexture(L"GrabTex_Right", L"texture\\player\\Grab_Right.bmp");
	CTexture* pTexLeft = CResMgr::GetInst()->LoadTexture(L"GrabTex_Left", L"texture\\player\\Grab_Left.bmp");


	//애니메이션 로딩
	//GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");



	//RIGHT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_GRAB", pTexRight,
		Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_GRABBING", pTexRight,
		Vec2(0.f, 200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB", pTexRight,
		Vec2(0.f, 400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB", pTexRight,
		Vec2(0.f, 600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_FLYING", pTexRight,
		Vec2(0.f, 800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 1.f, Vec2(0.f, 0.f));


	//LEFT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_GRAB", pTexLeft,
		Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_GRABBING", pTexLeft,
		Vec2(0.f, 200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_RETURN_WITHGRAB", pTexLeft,
		Vec2(0.f, 400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_RETURN_WITHOUTGRAB", pTexLeft,
		Vec2(0.f, 600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 1.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_FLYING", pTexLeft,
		Vec2(0.f, 800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 1.f, Vec2(0.f, 0.f));




	//RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_GRAB")->Save(L"animation\\grab_right_grab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_GRABBING")->Save(L"animation\\grab_right_grabbing.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB")->Save(L"animation\\grab_right_return_withgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB")->Save(L"animation\\grab_right_return_withoutgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_FLYING")->Save(L"animation\\grab_right_flying.anim");


	//LEFT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_GRAB_LEFT_GRAB")->Save(L"animation\\player_left_grab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_LEFT_GRABBING")->Save(L"animation\\grab_left_grabbing.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_LEFT_RETURN_WITHGRAB")->Save(L"animation\\grab_left_return_withgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_LEFT_RETURN_WITHOUTGRAB")->Save(L"animation\\grab_left_return_withoutgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_LEFT_FLYING")->Save(L"animation\\grab_left_flying.anim");



	GetAnimator()->Play(L"SNB_GRAB_LEFT_FLYING", true);
#pragma endregion




}

CHook::~CHook()
{

}

void CHook::LookAt(Vec2 _target)
{
	Vec2 dir = _target - GetPos();
	Vec2 up = Vec2(0.f, -1.f);
	m_rotation = dir.Angle(up);

	dir.Normalize();
	m_vDir = dir;
}



void CHook::Update()
{
	Vec2 vPos = GetPos();

	vPos.x = vPos.x + m_fSpeed *m_vDir.x * fDT;
	vPos.y = vPos.y + m_fSpeed * m_vDir.y * fDT;


	


	SetPos(vPos);
}

void CHook::Render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();



	Component_Render(_dc);
}

void CHook::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}
}
