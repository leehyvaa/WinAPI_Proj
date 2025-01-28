﻿#include "pch.h"
#include "PlayerArm.h"
#include "SPlayer.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
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
		Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_RUN", pArmTexRight,
		Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 0.85f, Vec2(-18.f, 18.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_JUMP", pArmTexRight,
		Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_FALLING", pArmTexRight,
		Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_LAND", pArmTexRight,
		Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_CLIMBUP", pArmTexRight,
		Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 10, 0.85f, Vec2(-5.f, 13.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_CLIMBDOWN", pArmTexRight,
		Vec2(0.f, 2700.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 7, 0.85f, Vec2(-5.f, 18.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_CLIMBSTOP", pArmTexRight,
		Vec2(0.f, 2900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 11, 0.85f, Vec2(0.f, 3.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_SWING", pArmTexRight,
		Vec2(0.f, 2300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 3, 0.85f, Vec2(0.f, 3.f));


	//LEFT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_IDLE", pArmTexLeft,
		Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 0.85f, Vec2(20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_RUN", pArmTexLeft,
		Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 0.85f, Vec2(18.f, 18.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_JUMP", pArmTexLeft,
		Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 0.85f, Vec2(20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_FALLING", pArmTexLeft,
		Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_LAND", pArmTexLeft,
		Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_CLIMBUP", pArmTexLeft,
		Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 10, 0.85f, Vec2(5.f, 13.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_CLIMBDOWN", pArmTexLeft,
		Vec2(0.f, 2700.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 7, 0.85f, Vec2(5.f, 18.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_CLIMBSTOP", pArmTexLeft,
		Vec2(0.f, 2900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 11, 0.85f, Vec2(0.f, 3.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_LEFT_SWING", pArmTexLeft,
		Vec2(0.f, 2300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 3, 0.85f, Vec2(0.f, 3.f));



	//RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_IDLE")->Save(L"animation\\playerArm_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_RUN")->Save(L"animation\\playerArm_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_JUMP")->Save(L"animation\\playerArm_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_FALLING")->Save(L"animation\\playerArm_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_LAND")->Save(L"animation\\playerArm_right_land.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_CLIMBUP")->Save(L"animation\\playerArm_right_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_CLIMBDOWN")->Save(L"animation\\playerArm_right_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_CLIMBSTOP")->Save(L"animation\\playerArm_right_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_SWING")->Save(L"animation\\playerArm_right_swing.anim");

	//LEFT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_IDLE")->Save(L"animation\\playerArm_left_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_RUN")->Save(L"animation\\playerArm_left_run.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_JUMP")->Save(L"animation\\playerArm_left_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_FALLING")->Save(L"animation\\playerArm_left_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_LAND")->Save(L"animation\\playerArm_left_land.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_CLIMBUP")->Save(L"animation\\playerArm_left_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_CLIMBDOWN")->Save(L"animation\\playerArm_left_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_CLIMBSTOP")->Save(L"animation\\playerArm_left_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_LEFT_SWING")->Save(L"animation\\playerArm_left_swing.anim");




	GetAnimator()->Play(L"SNB_ARM_RIGHT_RUN", true);
#pragma endregion


}

PlayerArm::~PlayerArm()
{
}




void PlayerArm::Update()
{
	Vec2 vPos(GetPos());
	if (owner->GetDir() == 1)
	{
		vPos = Vec2(owner->GetPos().x, owner->GetPos().y - 80);
	}
	else
	{
		vPos = Vec2(owner->GetPos().x, owner->GetPos().y - 80);
	}
	SetPos(vPos);






	Update_Animation();

	GetAnimator()->Update();

	prevDir = dir;
	prevState= curState;
}

void PlayerArm::Render(HDC _dc)
{

	Component_Render(_dc);
}


void PlayerArm::Update_Animation()
{




	if (prevState == curState && prevDir == dir)
		return;


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
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_JUMP", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_JUMP", true);
		break;
	case PLAYER_STATE::CLIMB:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_CLIMBSTOP", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBSTOP", true);
		break;
	case PLAYER_STATE::CLIMBUP:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_CLIMBUP", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBUP", true);
		break;
	case PLAYER_STATE::CLIMBDOWN:
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_CLIMBDOWN", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBDOWN", true);
		break;
	case PLAYER_STATE::SWING:
		//LookAt 고쳐야함 기준방향으로부터의 회전으로
		LookAt(owner->GetTargetPos());
		if (dir == -1)
			GetAnimator()->Play(L"SNB_ARM_LEFT_SWING", true);
		else
			GetAnimator()->Play(L"SNB_ARM_RIGHT_SWING", true);
		break;
	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}

}

