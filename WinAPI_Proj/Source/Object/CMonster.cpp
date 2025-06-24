#include "pch.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "AI.h"
#include "func.h"
#include "Module/AI/State/Subdued/CSubduedState.h"

CMonster::CMonster()
	:m_tInfo{}
{
	CreateCollider();
    CreateRigidBody();
    CreateAnimator();
	GetCollider()->SetScale(Vec2(100.f, 100.f));
    SetGroup(GROUP_TYPE::MONSTER);

    AI* pAI = new AI;
    SetAI(pAI);
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::Start()
{
    GameObject::Start();
}

void CMonster::Update()
{

}

void CMonster::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;
    
    GameObject::Render(_pRenderTarget);
}

void CMonster::FinalUpdata()
{
}

void CMonster::Reset()
{
    GameObject::Reset();
}

void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Wire")
	{
		// 스폰 중이거나 죽은 상태일 때는 갈고리가 통과하도록 함
		if (IsDead() || (m_pAI && (m_pAI->GetCurState() == MON_STATE::SPAWNING ||
		                           m_pAI->GetCurState() == MON_STATE::DEAD)))
		{
			return; // 갈고리 충돌 무시
		}

		// 제압 상태에서 처형 중인 경우에도 갈고리가 통과하도록 함
		if (m_pAI && m_pAI->GetCurState() == MON_STATE::SUBDUED)
		{
			CSubduedState* pSubduedState = static_cast<CSubduedState*>(m_pAI->GetState(MON_STATE::SUBDUED));
			if (pSubduedState && pSubduedState->IsExecuted())
			{
				return; // 처형 중인 몬스터는 갈고리 충돌 무시
			}
		}

		m_tInfo.fHP -= 1;
		if(m_tInfo.fHP <= 0 && m_pAI && m_pAI->GetCurState() != MON_STATE::DEAD)
		{
			// 이벤트 시스템을 통해 안전하게 AI 상태 변경
			ChangeAIState(m_pAI, MON_STATE::DEAD);
		}
	}
}

void CMonster::OnCollision(CCollider* _pOther)
{
    GameObject::OnCollision(_pOther);
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
    GameObject::OnCollisionExit(_pOther);
}
