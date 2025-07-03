#include "pch.h"
#include "CHook.h"

#include "AI.h"
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
#include "CMonster.h"
#include "Module/AI/State/Subdued/CSubduedState.h"
#include "Object/Ground/CSkylineCar.h"
 
#include "CTexture.h"
 
CHook::CHook()
	:m_fSpeed(2000)
	, hookState(HOOK_STATE::FLYING)
	, m_pChainBitmap(nullptr)
	, m_bChainCached(false)
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
    ReleaseChainBitmap();
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
	            if (GetParent())
	            {
	                Vec2 worldPos = GetWorldPos();
	                SetParent(nullptr);
	                SetWorldPos(worldPos);
	            }
	            hookState = HOOK_STATE::RETURN_WITH;
	            return;
	        }

	        // 부모가 CSkylineCar이고, 사라지는 상태라면 갈고리 회수
	        if (GetParent())
	        {
	            CSkylineCar* pCar = dynamic_cast<CSkylineCar*>(GetParent());
	            if (pCar)
	            {
	                SKYLINE_CAR_STATE carState = pCar->GetState();
	                if (carState == SKYLINE_CAR_STATE::EXPLODING || carState == SKYLINE_CAR_STATE::SPAWNING)
	                {
	                    Vec2 worldPos = GetWorldPos();
	                    SetParent(nullptr);
	                    SetWorldPos(worldPos);
	                    hookState = HOOK_STATE::RETURN_WITHOUT;
	                    return;
	                }
	            }
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

	               // 만약 움직이는 발판(CSkylineCar)에 박혔다면, 갈고리를 발판의 자식으로 설정
	               CSkylineCar* pCar = dynamic_cast<CSkylineCar*>(pOtherObj);
	               if (pCar)
	               {
	                   if (GetParent() != pCar)
	                   {
	                       Vec2 worldPos = GetWorldPos();
	                       SetParent(pCar);
	                       SetWorldPos(worldPos);
	                   }
	               }
	           }
	           else if (groundType == GROUND_TYPE::UNWALKABLE)
	           {
	               hookState = HOOK_STATE::RETURN_WITHOUT;
	           }
	       }
	   }
	   else if (pOtherObj->GetGroup() == GROUP_TYPE::MONSTER)
	   {
	       if (hookState == HOOK_STATE::FLYING)
	       {
	           CMonster* pMonster = static_cast<CMonster*>(pOtherObj);

	           // 스폰 중이거나 죽은 상태일 때는 갈고리가 통과하도록 함
	           if (pMonster->IsDead() || (pMonster->GetAI() && (pMonster->GetAI()->GetCurState() == MON_STATE::SPAWNING ||
	                                                            pMonster->GetAI()->GetCurState() == MON_STATE::DEAD)))
	           {
	               return; // 갈고리가 통과함 (충돌 무시)
	           }

	           // 제압 상태에서 처형 중인 경우에도 갈고리가 통과하도록 함
	           if (pMonster->GetAI() && pMonster->GetAI()->GetCurState() == MON_STATE::SUBDUED)
	           {
	               CSubduedState* pSubduedState = static_cast<CSubduedState*>(pMonster->GetAI()->GetState(MON_STATE::SUBDUED));
	               if (pSubduedState && pSubduedState->IsExecuted())
	               {
	                   return; // 처형 중인 몬스터는 갈고리가 통과함
	               }
	           }

	           // 일반 상태의 몬스터와 충돌하면 갈고리가 되돌아감
	           hookState = HOOK_STATE::RETURN_WITHOUT;
	       }
	   }
}

void CHook::Render(ID2D1RenderTarget* _pRenderTarget)
{
	   if (!_pRenderTarget)
	       return;
	       
	   // 애니메이션 컴포넌트 Direct2D 렌더링 (다른 오브젝트처럼)
	   GetAnimator()->Render(_pRenderTarget);

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
	   
	   // 체인 비트맵 캐싱
	   if (!m_bChainCached)
	   {
	       CacheChainBitmap(_pRenderTarget);
	   }
	   
	   if (!m_pChainBitmap)
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
	           m_pChainBitmap,
	           destRect,
	           1.0f,
	           D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	       );
	   }
	   
	   // 변환 행렬 복원
	   _pRenderTarget->SetTransform(originalTransform);
}

void CHook::CacheChainBitmap(ID2D1RenderTarget* _pRenderTarget)
{
	   if (!pChainTex || !_pRenderTarget)
	       return;

	   // 기존 비트맵 해제
	   ReleaseChainBitmap();

	   // PNG 파일의 경우 Direct2D 비트맵을 직접 사용 (알파 채널 지원)
	   ID2D1Bitmap* pSourceD2DBitmap = pChainTex->GetBitmap();
	   if (pSourceD2DBitmap)
	   {
	       // 원본 Direct2D 비트맵을 직접 참조 (참조 카운트 증가)
	       m_pChainBitmap = pSourceD2DBitmap;
	       m_pChainBitmap->AddRef();
	       m_bChainCached = true;
	   }
}

void CHook::ReleaseChainBitmap()
{
	   if (m_pChainBitmap)
	   {
	       m_pChainBitmap->Release();
	       m_pChainBitmap = nullptr;
	   }
	   m_bChainCached = false;
}
