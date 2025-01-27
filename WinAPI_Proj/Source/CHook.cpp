#include "pch.h"
#include "CHook.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CAnimation.h"
#include "SPlayer.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CKeyMgr.h"
#include "PlayerArm.h"
CHook::CHook()
	:m_fSpeed(2000)

	,hookState(HOOK_STATE::FLYING)
{

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(20.f, 20.f));

	CreateAnimator();
#pragma region ��ũ �ִϸ��̼�
	//�ؽ��� �ε�
	CTexture* pTexRight = CResMgr::GetInst()->LoadTexture(L"GrabTex_Right", L"texture\\player\\Grab_Right.bmp");
	CTexture* pTexLeft = CResMgr::GetInst()->LoadTexture(L"GrabTex_Left", L"texture\\player\\Grab_Left.bmp");


	//�ִϸ��̼� �ε�
	//GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");



	//RIGHT �ִϸ��̼� ����
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_GRAB", pTexRight,
		Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_GRABBING", pTexRight,
		Vec2(0.f, 200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB", pTexRight,
		Vec2(0.f, 400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB", pTexRight,
		Vec2(0.f, 600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_FLYING", pTexRight,
		Vec2(0.f, 800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, 0.f));


	//LEFT �ִϸ��̼� ����
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_GRAB", pTexLeft,
		Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_GRABBING", pTexLeft,
		Vec2(0.f, 200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_RETURN_WITHGRAB", pTexLeft,
		Vec2(0.f, 400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_RETURN_WITHOUTGRAB", pTexLeft,
		Vec2(0.f, 600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_LEFT_FLYING", pTexLeft,
		Vec2(0.f, 800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, 0.f));




	//RIGHT �ִϸ��̼� ����
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_GRAB")->Save(L"animation\\grab_right_grab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_GRABBING")->Save(L"animation\\grab_right_grabbing.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB")->Save(L"animation\\grab_right_return_withgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB")->Save(L"animation\\grab_right_return_withoutgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_FLYING")->Save(L"animation\\grab_right_flying.anim");


	//LEFT �ִϸ��̼� ����
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


void CHook::Update_Animation()
{

	

	switch (hookState)
	{
	case HOOK_STATE::FLYING:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_GRAB_LEFT_FLYING", true);
		else
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_FLYING", true);
		break;
	case HOOK_STATE::GRAB:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_GRAB_LEFT_GRAB", true);
		else
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_GRAB", true);
		break;
	case HOOK_STATE::GRABBING:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_GRAB_LEFT_GRABBING", true);
		else
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_GRABBING", true);
		break;
	case HOOK_STATE::RETURN_WITH:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_GRAB_LEFT_RETURN_WITHGRAB", true);
		else
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB", true);
		break;
	case HOOK_STATE::RETURN_WITHOUT:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_GRAB_LEFT_RETURN_WITHOUTGRAB", true);
		else
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB", true);
		break;
	default:
		break;
	}

}

void CHook::Update_State()
{
	if (prevState == curState && prevDir == dir)
		return;



}

void CHook::Update_Move()
{
	Vec2 vPos = GetPos();

	

	switch (hookState)
	{
	case HOOK_STATE::FLYING:
	{
			
		vPos.x = vPos.x + m_fSpeed * GetDir().x * fDT*2;
		vPos.y = vPos.y + m_fSpeed * GetDir().y * fDT*2;
			
		//�Ÿ��� ���ѰŸ��̻� ����� without�������� ��ȯ
		if ((GetPos() - owner->GetPos()).Length() > 600.f)
		{
			hookState = HOOK_STATE::RETURN_WITHOUT;
		}
	}
		break;
	case HOOK_STATE::GRAB:
		if (KEY_HOLD(KEY::LBUTTON) == false)
		{
			hookState = HOOK_STATE::RETURN_WITH;
		}
		break;
	case HOOK_STATE::GRABBING:
		
		break;
	case HOOK_STATE::RETURN_WITH:
	{
		Vec2 newDir = owner->GetPos() - GetPos();
		newDir.Normalize();

		vPos.x = vPos.x + m_fSpeed * newDir.x * fDT*3;
		vPos.y = vPos.y + m_fSpeed * newDir.y * fDT*3;

		//�÷��̾����� �����ϸ� ����
		if ((GetPos() - owner->GetPos()).Length() < 30.f)
		{
			DeleteObject(this);
			((PlayerArm*)owner)->GetOwner()->SetHookRemove(nullptr);

		}
	}
		

		break;
	case HOOK_STATE::RETURN_WITHOUT:
	{
		Vec2 newDir = owner->GetPos() - GetPos();
		newDir.Normalize();

		vPos.x = vPos.x + m_fSpeed * newDir.x * fDT*3;
		vPos.y = vPos.y + m_fSpeed * newDir.y * fDT*3;

		//�÷��̾����� �����ϸ� ����
		if ((GetPos() - owner->GetPos()).Length() < 30.f)
		{
			

			DeleteObject(this);
			((PlayerArm*)owner)->GetOwner()->SetHookRemove(nullptr);

		}
	}
		break;
	default:
		break;
	}

	SetPos(vPos);

}



void CHook::Update()
{
	

	Update_State();
	Update_Move();
	Update_Animation();
	


}

void CHook::Render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	

	Component_Render(_dc);


	Vec2 pos1 = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 pos2 = CCamera::GetInst()->GetRenderPos(owner->GetPos());

	SelectGDI p(_dc, PEN_TYPE::BLUE);

	MoveToEx(_dc, pos1.x,pos1.y, nullptr);
	LineTo(_dc, pos2.x,pos2.y);

}

void CHook::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Ground" && hookState == HOOK_STATE::FLYING)
	{
		hookState = HOOK_STATE::GRAB;
		//DeleteObject(this);
	}
	else if (pOtherObj->GetName() == L"NonGround" && hookState == HOOK_STATE::FLYING)
	{
		hookState = HOOK_STATE::RETURN_WITHOUT;

	}
}
