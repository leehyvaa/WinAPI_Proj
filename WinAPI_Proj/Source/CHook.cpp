#include "pch.h"
#include "CHook.h"
#include "CTimeMgr.h"
#include "CGround.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CAnimation.h"
#include "SPlayer.h"
#include "CCore.h"
#include "CEventMgr.h"
#include "SelectGDI.h"
#include "CKeyMgr.h"
#include "CObjectPool.h"
#include "PlayerArm.h"
#include <gdiplus.h>

#include "CTexture.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

CHook::CHook()
	:m_fSpeed(2000)

	, hookState(HOOK_STATE::FLYING)
{
    SetGroup(GROUP_TYPE::HOOK);
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(20.f, 20.f));
	CreateAnimator();
    
#pragma region 후크 애니메이션
	//텍스쳐 로딩
	CTexture* pTexRight = CResMgr::GetInst()->LoadTexture(L"GrabTex_Right", L"texture\\player\\Grab_Right.bmp");
	CTexture* pTexLeft = CResMgr::GetInst()->LoadTexture(L"GrabTex_Left", L"texture\\player\\Grab_Left.bmp");
	pChainTex= CResMgr::GetInst()->LoadTexture(L"Chain", L"texture\\player\\Chain.bmp");
	

	//애니메이션 로딩
	//GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");



	//RIGHT 애니메이션 생성
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


	//LEFT 애니메이션 생성
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

void CHook::ReturnToPool()
{
    DeleteObject(this);
}

void CHook::Reset()
{
    GameObject::Reset();

    // Hook 속성 초기화
    hookState = HOOK_STATE::FLYING;
    m_fSpeed = 2000.f;  // 원래 생성자에서 설정한 속도
    
    // 애니메이션 초기화
    if (GetAnimator())
    {
        if (dir == -1)
            GetAnimator()->Play(L"SNB_GRAB_LEFT_FLYING", true);
        else
            GetAnimator()->Play(L"SNB_GRAB_RIGHT_FLYING", true);
    }
    
    // 충돌체 상태 초기화 (충돌 기록 제거)
    if (GetCollider())
    {
        GetCollider()->SetActive(true);
        // 충돌체 크기와 오프셋 재설정
        GetCollider()->SetOffsetPos(Vec2());
        GetCollider()->SetScale(Vec2(20.f, 20.f));
    }
    
    if (GetParent())
    {
        SetWorldPos(GetParent()->GetWorldPos());
        
        // PlayerArm의 방향에 따라 Hook 방향 설정
        PlayerArm* pArm = dynamic_cast<PlayerArm*>(GetParent());
        if (pArm) {
            SPlayer* pPlayer = dynamic_cast<SPlayer*>(pArm->GetParent());
            if (pPlayer) {
                dir = pPlayer->GetDir();
                curState = pPlayer->GetState();
            }
        }
    }
    
    // 방향 초기화 (기본 위쪽 방향)
    SetDir(-1.f);
    
    // 상태 추적 변수 초기화
    prevDir = dir;
    prevState = curState;
}

void CHook::LookAt(Vec2 _target)
{
    GameObject::LookAt(_target);
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
	Vec2 vPos = GetWorldPos();
    
    PlayerArm* pArm = m_pOwnerArm;
    SPlayer* player = static_cast<SPlayer*>(pArm->GetParent());
    
	switch (hookState)
	{
	case HOOK_STATE::FLYING:
	{
		vPos.x = vPos.x + m_fSpeed * GetDir().x * fDT * 2;
		vPos.y = vPos.y + m_fSpeed * GetDir().y * fDT * 2;
        m_fMaxRange = player->GetWireMaxRange();
		// 거리가 제한거리이상 벗어나면 without리턴으로 변환
		if ((GetWorldPos() - pArm->GetWorldPos()).Length() > m_fMaxRange)
		{
			hookState = HOOK_STATE::RETURN_WITHOUT;
		}
	}break;
	case HOOK_STATE::GRAB:
	    {
	        if (KEY_HOLD(KEY::LBUTTON) == false)
	        {
	            hookState = HOOK_STATE::RETURN_WITH;
	            return;
	        }
	    }break;
	case HOOK_STATE::GRABBING:

		break;
	case HOOK_STATE::RETURN_WITH:
	{
		Vec2 newDir = pArm->GetWorldPos() - GetWorldPos();
		newDir.Normalize();

		vPos.x = vPos.x + m_fSpeed * newDir.x * fDT * 3;
		vPos.y = vPos.y + m_fSpeed * newDir.y * fDT * 3;

		// 플레이어한테 도달하면 삭제
		if ((GetWorldPos() - pArm->GetWorldPos()).Length() < 30.f && !IsDead())
		{
		    ReturnToPool();
		    SetDead(true);
			player->SetHookRemove(nullptr);

		}
	}break;
	case HOOK_STATE::RETURN_WITHOUT:
	{
		Vec2 newDir = pArm->GetWorldPos() - GetWorldPos();
		newDir.Normalize();

		vPos.x = vPos.x + m_fSpeed * newDir.x * fDT * 3;
		vPos.y = vPos.y + m_fSpeed * newDir.y * fDT * 3;

		// 플레이어한테 도달하면 삭제
		if ((GetWorldPos() - pArm->GetWorldPos()).Length() < 30.f && !IsDead())
		{
		    ReturnToPool();
		    SetDead(true);
			player->SetHookRemove(nullptr);
		}
	}break;
	default:
		break;
	}

	SetWorldPos(vPos);

}



void CHook::Update()
{

    if (hookState != HOOK_STATE::GRAB)
    {
        Update_State();
    }
	Update_Move();
	Update_Animation();



}

void CHook::Render(HDC _dc)
{
	Component_Render(_dc);

    // 체인 그리기
    if (!m_pOwnerArm) return;
    
    // 필요한 정보 세팅
    Vec2 vHookWorldPos = GetWorldPos();
    Vec2 vArmWorldPos = m_pOwnerArm->GetWorldPos();

    Vec2 vHookRenderPos = CCamera::GetInst()->GetRenderPos(vHookWorldPos);
    Vec2 vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos);

    Vec2 vDir = vHookRenderPos - vArmRenderPos;
    float fDistance = vDir.Length();

    
    if (fDistance < 1.f) return; // 거리가 매우 짧으면 그리지 않음
    if (!pChainTex) return; // 텍스처 없으면 그리지 않음

    vDir.Normalize();
    
    
    float fLinkWidth = static_cast<float>(pChainTex->Width());
    float fLinkHeight = static_cast<float>(pChainTex->Height());
    
    // 쇠사슬 길이 : 너비 방향
    float fLinkLength = fLinkWidth;

    if (fLinkLength <= 0) return; // 링크 길이가 0 이하면 그리지 않음

    // 회전각도 라디안 -> 각도
    float fAngleRad = atan2(vDir.y, vDir.x);
    float fAngleDeg = fAngleRad * (180.f / 3.1415926535f);
    
    // GDI+ 세팅
    Graphics graphics(_dc);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);

    // GDI+ Bitmap 객체
    Bitmap chainBitmap(pChainTex->GetHBITMAP(), nullptr);
    ImageAttributes imgAttr;
    
    // 마젠타 색상
    Color magenta(255, 0, 255);
    imgAttr.SetColorKey(magenta, magenta, ColorAdjustTypeBitmap);
    
    // 밝기 조절 RGB 1.2배 밝게, 전체밝기 0.15 추가
    ColorMatrix colorMatrix = {
        1.2f, 0.0f, 0.0f, 0.0f, 0.0f,  
        0.0f, 1.2f, 0.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.2f, 0.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  
        0.15f, 0.15f, 0.15f, 0.0f, 1.0f 
    };
    
    // ColorMatrix를 ImageAttributes에 적용
    imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
    
    // 사슬 크기 배율
    const float fScaleFactor = 0.3f;
    
    // 배율에 맞춰 크기 재계산
    float fScaledLinkWidth = fLinkWidth * fScaleFactor;
    float fScaledLinkHeight = fLinkHeight * fScaleFactor;
    
    // 체인 간격을 위한 링크 길이 계산
    float fScaledLinkLength = fLinkLength * fScaleFactor;
    
    // 축소된 크기로 그릴 링크 개수 다시 계산
    int iNumLinks = static_cast<int>(fDistance / fScaledLinkLength);

    // 링크 반복문
    for (int i = 0; i < iNumLinks; ++i)
    {
        // 현재 링크의 중심 위치 계산 - 축소된 길이 기준으로 계산
        // 팔 위치에서 시작해서 방향 * 축소된 링크 길이 * (i + 0.5) 만큼 이동
        Vec2 vLinkCenterPos = vArmRenderPos + vDir * (fScaledLinkLength * (static_cast<float>(i) + 0.5f));
        
        PointF centerPt(vLinkCenterPos.x, vLinkCenterPos.y);

        // 링크 중심으로 회전
        Matrix transformMatrix;
        transformMatrix.RotateAt(fAngleDeg, centerPt);
        graphics.SetTransform(&transformMatrix);

        // DrawImage는 좌상단 좌표 기준이므로, 중심점에서 축소된 너비/높이의 절반을 빼서 계산
        float fDrawX = vLinkCenterPos.x - fScaledLinkWidth / 2.f;
        float fDrawY = vLinkCenterPos.y - fScaledLinkHeight / 2.f;

        // 이미지 그리기
        graphics.DrawImage(
            &chainBitmap,
            RectF(fDrawX, fDrawY, fScaledLinkWidth, fScaledLinkHeight), // 그릴 위치와 크기
            0, 0, fLinkWidth, fLinkHeight, // 원본 이미지에서 가져올 영역
            UnitPixel,
            &imgAttr // 투명 처리
        );

        // 다음 그리기를 위한 리셋
        graphics.ResetTransform();
    }

}

void CHook::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
    
    if (pOtherObj->GetGroup() == GROUP_TYPE::GROUND)
    {
        if (hookState == HOOK_STATE::FLYING)
        {
            GROUND_TYPE groundType = static_cast<CGround*>(pOtherObj)->GetGroundType();
            
            if (groundType == GROUND_TYPE::NORMAL)
            {
                hookState = HOOK_STATE::GRAB;
            }
            else if (groundType == GROUND_TYPE::UNWALKABLE)
            {
                hookState = HOOK_STATE::RETURN_WITHOUT;
            }
        }
    }
}
