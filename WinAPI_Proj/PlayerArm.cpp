#include "pch.h"
#include "PlayerArm.h"
#include "SPlayer.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CHook.h"
#include "CCollider.h"

PlayerArm::PlayerArm()
	:m_fSpeed(1000)
	,owner(nullptr)
	,m_rotation(0.f)
{
	CreateAnimator();
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 5.f));
	GetCollider()->SetScale(Vec2(10.f, 10.f));

	

#pragma region  플레이어 Arm 애니메이션
	//텍스쳐 로딩
	CTexture* pArmTexRight = CResMgr::GetInst()->LoadTexture(L"ArmTex_Right", L"texture\\player\\Arm_Right.bmp");
	CTexture* pArmTexLeft = CResMgr::GetInst()->LoadTexture(L"ArmTex_Left", L"texture\\player\\Arm_Left.bmp");



	//애니메이션 로딩
	//GetAnimator()->LoadAnimation(L"animation\\playerArm_right_idle.anim");



	//RIGHT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_IDLE", pArmTexRight,
		Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 0.85f, Vec2(-20.f, -12.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_RUN", pArmTexRight,
		Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 0.85f, Vec2(-18.f, -22.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_JUMP", pArmTexRight,
		Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 0.85f, Vec2(-20.f, -12.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_FALLING", pArmTexRight,
		Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(-20.f, -12.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_LAND", pArmTexRight,
		Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(-20.f, -12.f));


	//LEFT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_IDLE", pArmTexLeft,
		Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 0.85f, Vec2(20.f, -12.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_RUN", pArmTexLeft,
		Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 0.85f, Vec2(18.f, -22.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_JUMP", pArmTexLeft,
		Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 0.85f, Vec2(20.f, -12.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_FALLING", pArmTexLeft,
		Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(20.f, -12.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_LAND", pArmTexLeft,
		Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(20.f, -12.f));



	//RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_IDLE")->Save(L"animation\\playerArm_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_RUN")->Save(L"animation\\playerArm_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_JUMP")->Save(L"animation\\playerArm_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_FALLING")->Save(L"animation\\playerArm_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_LAND")->Save(L"animation\\playerArm_right_land.anim");

	//LEFT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_IDLE")->Save(L"animation\\playerArm_left_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_RUN")->Save(L"animation\\playerArm_left_run.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_JUMP")->Save(L"animation\\playerArm_left_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_FALLING")->Save(L"animation\\playerArm_left_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_LAND")->Save(L"animation\\playerArm_left_land.anim");




	GetAnimator()->Play(L"SNB_ARM_RIGHT_RUN", true);
#pragma endregion


}

PlayerArm::~PlayerArm()
{
}




void PlayerArm::Update()
{
	Vec2 vPos(GetPos());
	vPos = Vec2(owner->GetPos().x, owner->GetPos().y-40);
	SetPos(vPos);







	Update_Animation();

	GetAnimator()->Update();

	prevDir = dir;
	curState = prevState;
}

void PlayerArm::Render(HDC _dc)
{

	Component_Render(_dc);
}

void PlayerArm::CreateHook()
{
	Vec2 vWirePos = GetPos();
	vWirePos.y -= GetScale().y / 2.f;

	CHook* pHook = new CHook;
	pHook->SetName(L"Hook");
	pHook->SetPos(vWirePos);
	pHook->SetScale(Vec2(11.f, 11.f));
	pHook->SetDir(Vec2(0.f, -1.f));

	CreateObject(pHook, GROUP_TYPE::PROJ_PLAYER);
	//CreateObject 함수에 포지션, 방향, 스케일을 설정해주는 인자를 넣어야함
}

void PlayerArm::Update_Animation()
{




	//if (prevState == curState && prevDir == dir)
	//	return;


	switch (curState)
	{
	case PLAYER_STATE::IDLE:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_IDLE", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_IDLE", true);
		break;
	case PLAYER_STATE::RUN:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_RUN", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_RUN", true);
		break;
	case PLAYER_STATE::ATTACK:

		break;

	case PLAYER_STATE::JUMP:
		
		break;

	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}

}

