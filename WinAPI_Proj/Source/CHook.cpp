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
	, m_pChainD2DBitmap(nullptr)
	, m_bChainD2DCached(false)
{
    SetGroup(GROUP_TYPE::HOOK);
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(20.f, 20.f));
	CreateAnimator();
    
#pragma region 후크 애니메이션
	//텍스쳐 로딩
	CTexture* pTexRight = CResMgr::GetInst()->LoadTexture(L"GrabTex_Right", L"texture\\player\\Grab_Right.png");
	pChainTex= CResMgr::GetInst()->LoadTexture(L"Chain", L"texture\\player\\Chain.png");
	

	//애니메이션 로딩
	//GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");



	//RIGHT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_GRAB", pTexRight,
		Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 1.5f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_GRABBING", pTexRight,
		Vec2(0.f, 200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 1.5f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB", pTexRight,
		Vec2(0.f, 400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 1.5f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB", pTexRight,
		Vec2(0.f, 600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 1.5f, Vec2(0.f, 0.f));
	GetAnimator()->CreateAnimation(L"SNB_GRAB_RIGHT_FLYING", pTexRight,
		Vec2(0.f, 800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 1.7f, Vec2(0.f, 0.f));

    

	//RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_GRAB")->Save(L"animation\\grab_right_grab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_GRABBING")->Save(L"animation\\grab_right_grabbing.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB")->Save(L"animation\\grab_right_return_withgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB")->Save(L"animation\\grab_right_return_withoutgrab.anim");
	GetAnimator()->FindAnimation(L"SNB_GRAB_RIGHT_FLYING")->Save(L"animation\\grab_right_flying.anim");
    

	GetAnimator()->Play(L"SNB_GRAB_RIGHT_FLYING", true);
#pragma endregion




}

CHook::~CHook()
{
    ReleaseChainD2DBitmap();
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
    
    // 상태 추적 변수 초기화
    m_bIsFacingRightPrev = m_bIsFacingRight;
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
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_FLYING", true);
		break;
	case HOOK_STATE::GRAB:
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_GRAB", true);
		break;
	case HOOK_STATE::GRABBING:
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_GRABBING", true);
		break;
	case HOOK_STATE::RETURN_WITH:
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_RETURN_WITHGRAB", true);
		break;
	case HOOK_STATE::RETURN_WITHOUT:
			GetAnimator()->Play(L"SNB_GRAB_RIGHT_RETURN_WITHOUTGRAB", true);
		break;
	default:
		break;
	}

}

void CHook::Update_State()
{
	if (prevState == curState && m_bIsFacingRightPrev == m_bIsFacingRight)
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
	    LookAt(m_vTargetPos);

		// 플레이어한테 도달하면 삭제
		if ((GetWorldPos() - pArm->GetWorldPos()).Length() < 70.f && !IsDead())
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
		if ((GetWorldPos() - pArm->GetWorldPos()).Length() < 100.f && !IsDead())
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

/*
void CHook::Render(HDC _dc)
{
    // Direct2D 활성화 시 GDI 렌더링 스킵 (하이브리드 렌더링 패턴)
    if (CCore::GetInst()->GetD2DRenderTarget())
        return;
    
	Component_Render(_dc);

    // 체인 그리기
    if (!m_pOwnerArm) return;
    
    // 필요한 정보 세팅
    Vec2 dir = GetWorldPos() - GetOwnerArm()->GetWorldPos();
    dir.Normalize();
    
    Vec2 vHookWorldPos = GetWorldPos();
    Vec2 vArmWorldPos = m_pOwnerArm->GetWorldPos() + dir * 25.f;

    Vec2 vHookRenderPos = CCamera::GetInst()->GetRenderPos(vHookWorldPos);
    Vec2 vArmRenderPos;


    if (hookState == HOOK_STATE::GRAB)
    {
        if (GetIsFacingRight())
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(-7.f,0.f);
        else
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(7.f,0.f);
    }
    else
    {
        if (GetIsFacingRight())
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(0.f,10.f);
        else
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(0.f,10.f);
    }
    

        
    Vec2 vDir = vHookRenderPos - vArmRenderPos;
    float fDistance = vDir.Length()-5.f;

    
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
    // ColorMatrix colorMatrix = {
    //     1.2f, 0.0f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 1.2f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 1.2f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     0.15f, 0.15f, 0.15f, 0.0f, 1.0f
    // };
    
    // ColorMatrix를 ImageAttributes에 적용
    //imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
    
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
*/

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

void CHook::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;
        
    // 애니메이션 컴포넌트 Direct2D 렌더링 (다른 오브젝트처럼)
    GetAnimator()->RenderD2D(_pRenderTarget);

    // 체인 그리기
    if (!m_pOwnerArm || !pChainTex) 
        return;
    
    // 필요한 정보 세팅 (기존 로직과 동일)
    Vec2 dir = GetWorldPos() - GetOwnerArm()->GetWorldPos();
    dir.Normalize();
    
    Vec2 vHookWorldPos = GetWorldPos();
    Vec2 vArmWorldPos = m_pOwnerArm->GetWorldPos() + dir * 25.f;

    Vec2 vHookRenderPos = CCamera::GetInst()->GetRenderPos(vHookWorldPos);
    Vec2 vArmRenderPos;

    if (hookState == HOOK_STATE::GRAB)
    {
        if (GetIsFacingRight())
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(-7.f, 0.f);
        else
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(7.f, 0.f);
    }
    else
    {
        if (GetIsFacingRight())
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(0.f, 10.f);
        else
            vArmRenderPos = CCamera::GetInst()->GetRenderPos(vArmWorldPos) + Vec2(0.f, 10.f);
    }
    
    Vec2 vDir = vHookRenderPos - vArmRenderPos;
    float fDistance = vDir.Length() - 5.f;
    
    if (fDistance < 1.f) return; // 거리가 매우 짧으면 그리지 않음
    
    vDir.Normalize();
    
    // 체인 D2D 비트맵 캐싱
    if (!m_bChainD2DCached)
    {
        CacheChainD2DBitmap(_pRenderTarget);
    }
    
    if (!m_pChainD2DBitmap)
        return;
    
    float fLinkWidth = static_cast<float>(pChainTex->Width());
    float fLinkHeight = static_cast<float>(pChainTex->Height());
    float fLinkLength = fLinkWidth;
    
    if (fLinkLength <= 0) return;
    
    // 회전각도 라디안 계산
    float fAngleRad = atan2(vDir.y, vDir.x);
    float fAngleDeg = fAngleRad * (180.f / 3.1415926535f);
    
    // 사슬 크기 배율
    const float fScaleFactor = 0.3f;
    float fScaledLinkWidth = fLinkWidth * fScaleFactor;
    float fScaledLinkHeight = fLinkHeight * fScaleFactor;
    float fScaledLinkLength = fLinkLength * fScaleFactor;
    
    // 축소된 크기로 그릴 링크 개수 계산
    int iNumLinks = static_cast<int>(fDistance / fScaledLinkLength);
    
    // 원본 변환 행렬 저장
    D2D1_MATRIX_3X2_F originalTransform;
    _pRenderTarget->GetTransform(&originalTransform);
    
    // 링크 반복 렌더링
    for (int i = 0; i < iNumLinks; ++i)
    {
        // 현재 링크의 중심 위치 계산
        Vec2 vLinkCenterPos = vArmRenderPos + vDir * (fScaledLinkLength * (static_cast<float>(i) + 0.5f));
        
        // SRT 변환 행렬 생성 (Scale -> Rotate -> Translate)
        D2D1_MATRIX_3X2_F matScale = D2D1::Matrix3x2F::Scale(fScaleFactor, fScaleFactor);
        D2D1_MATRIX_3X2_F matRotation = D2D1::Matrix3x2F::Rotation(fAngleDeg);
        D2D1_MATRIX_3X2_F matTranslation = D2D1::Matrix3x2F::Translation(vLinkCenterPos.x, vLinkCenterPos.y);
        
        _pRenderTarget->SetTransform(matScale * matRotation * matTranslation);
        
        // 중심점 기준으로 이미지 렌더링
        D2D1_RECT_F destRect = D2D1::RectF(
            -fLinkWidth / 2.f,
            -fLinkHeight / 2.f,
            fLinkWidth / 2.f,
            fLinkHeight / 2.f
        );
        
        _pRenderTarget->DrawBitmap(
            m_pChainD2DBitmap, 
            destRect, 
            1.0f, 
            D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
        );
    }
    
    // 변환 행렬 복원
    _pRenderTarget->SetTransform(originalTransform);
}

void CHook::CacheChainD2DBitmap(ID2D1RenderTarget* _pRenderTarget)
{
    if (!pChainTex || !_pRenderTarget)
        return;

    // 기존 비트맵 해제
    ReleaseChainD2DBitmap();

    // PNG 파일의 경우 Direct2D 비트맵을 직접 사용 (알파 채널 지원)
    ID2D1Bitmap* pSourceD2DBitmap = pChainTex->GetD2DBitmap();
    if (pSourceD2DBitmap)
    {
        // 원본 Direct2D 비트맵을 직접 참조 (참조 카운트 증가)
        m_pChainD2DBitmap = pSourceD2DBitmap;
        m_pChainD2DBitmap->AddRef();
        m_bChainD2DCached = true;
    }
}

void CHook::ReleaseChainD2DBitmap()
{
    if (m_pChainD2DBitmap)
    {
        m_pChainD2DBitmap->Release();
        m_pChainD2DBitmap = nullptr;
    }
    m_bChainD2DCached = false;
}
