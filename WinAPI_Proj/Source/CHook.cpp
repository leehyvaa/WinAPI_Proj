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
        // 필요하다면 충돌체 크기와 오프셋 재설정
        GetCollider()->SetOffsetPos(Vec2());
        GetCollider()->SetScale(Vec2(20.f, 20.f));
    }
    
    if (GetParent())
    {
        SetPos(GetParent()->GetPos());
        
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

    PlayerArm* pArm = static_cast<PlayerArm*>(GetParent());
    SPlayer* player = static_cast<SPlayer*>(pArm->GetParent());
    
	switch (hookState)
	{
	case HOOK_STATE::FLYING:
	{
		vPos.x = vPos.x + m_fSpeed * GetDir().x * fDT * 2;
		vPos.y = vPos.y + m_fSpeed * GetDir().y * fDT * 2;
        m_fMaxRange = player->GetWireMaxRange();
		//거리가 제한거리이상 벗어나면 without리턴으로 변환
		if ((GetPos() - pArm->GetPos()).Length() > m_fMaxRange)
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
		Vec2 newDir = pArm->GetPos() - GetPos();
		newDir.Normalize();

		vPos.x = vPos.x + m_fSpeed * newDir.x * fDT * 3;
		vPos.y = vPos.y + m_fSpeed * newDir.y * fDT * 3;

		//플레이어한테 도달하면 삭제
		if ((GetPos() - pArm->GetPos()).Length() < 30.f && !IsDead())
		{
		    ReturnToPool();
		    SetDead(true);
			player->SetHookRemove(nullptr);

		}
	}break;
	case HOOK_STATE::RETURN_WITHOUT:
	{
		Vec2 newDir = pArm->GetPos() - GetPos();
		newDir.Normalize();

		vPos.x = vPos.x + m_fSpeed * newDir.x * fDT * 3;
		vPos.y = vPos.y + m_fSpeed * newDir.y * fDT * 3;

		//플레이어한테 도달하면 삭제
		if ((GetPos() - pArm->GetPos()).Length() < 30.f && !IsDead())
		{
		    ReturnToPool();
		    SetDead(true);
			player->SetHookRemove(nullptr);
		}
	}break;
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
	Vec2 pos2 = CCamera::GetInst()->GetRenderPos(GetParent()->GetPos());

	SelectGDI p(_dc, PEN_TYPE::BLUE);

	MoveToEx(_dc, pos1.x, pos1.y, nullptr);
	LineTo(_dc, pos2.x, pos2.y);

}

void CHook::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
    
    cout << "Hook collision with: " << static_cast<int>(pOtherObj->GetGroup()) << endl;
    
    if (pOtherObj->GetGroup() == GROUP_TYPE::GROUND)
    {
        cout << "Hook collided with ground, current state: " << static_cast<int>(hookState) << endl;
        
        if (hookState == HOOK_STATE::FLYING)
        {
            GROUND_TYPE groundType = static_cast<CGround*>(pOtherObj)->GetGroundType();
            
            if (groundType == GROUND_TYPE::NORMAL)
            {
                hookState = HOOK_STATE::GRAB;
                cout << "Hook state changed to GRAB" << endl;
            }
            else if (groundType == GROUND_TYPE::UNWALKABLE)
            {
                hookState = HOOK_STATE::RETURN_WITHOUT;
                cout << "Hook state changed to RETURN_WITHOUT" << endl;
            }
        }
    }
}
