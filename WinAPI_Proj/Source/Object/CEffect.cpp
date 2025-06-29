#include "pch.h"
#include "CEffect.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CObjectPool.h"

CEffect::CEffect()
{
    CreateAnimator();
}

CEffect::~CEffect()
{
}

void CEffect::Update()
{
    GameObject::Update();
}

void CEffect::Reset()
{
    // GameObject의 Reset을 호출하여 기본적인 상태 초기화
    GameObject::Reset();
    if (GetAnimator())
    {
        GetAnimator()->Reset();
    }
}

void CEffect::PlayEffect(const wstring& _animName, bool _bRepeat)
{
    CAnimator* pAnimator = GetAnimator();
    if (!pAnimator) return;

    pAnimator->Play(_animName, _bRepeat);

    // 반복되지 않는 애니메이션은 종료 시 풀로 반환
    if (!_bRepeat)
    {
        CAnimation* pAnim = pAnimator->FindAnimation(_animName);
        if (pAnim)
        {
            pAnim->SetEndFrameEvent([this]() {
                CObjectPool::GetInst()->ReturnObject(this);
            });
        }
    }
}