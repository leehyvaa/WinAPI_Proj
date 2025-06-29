#include "pch.h"
#include "CEffectMgr.h"
#include "CObjectPool.h"
#include "CAnimator.h"
#include "CEffect.h"
#include "CSceneMgr.h"
#include "CScene.h"

void CEffectManager::Init()
{
    // 오브젝트 풀에 범용 이펙트 객체 생성
    CObjectPool::GetInst()->CreatePool<CEffect>(L"EffectPool", 30);

    // 여기에 20여 종의 이펙트를 등록합니다.
    // 예시: RegisterEffect(이펙트 고유이름, 애니메이션 이름, 애니메이션 파일 경로);
    RegisterEffect(L"Explosion", L"ANIM_EXPLOSION", L"animation\\effect\\explosion.anim");
    RegisterEffect(L"HitSpark", L"ANIM_HIT_SPARK", L"animation\\effect\\hit_spark.anim");
    // ... 나머지 이펙트들 등록
}

void CEffectManager::RegisterEffect(const wstring& _effectName, const wstring& _animName, const wstring& _animPath)
{
    EffectData data = {};
    data.animName = _animName;
    data.animPath = _animPath;
    m_mapEffectData.insert({_effectName, data});
}

void CEffectManager::Play(const wstring& _effectName, Vec2 _vPos)
{
    // 1. 이펙트 정보 조회
    auto iter = m_mapEffectData.find(_effectName);
    if (iter == m_mapEffectData.end())
    {
        // 등록되지 않은 이펙트
        return;
    }
    const EffectData& data = iter->second;

    // 2. 오브젝트 풀에서 범용 이펙트 객체 가져오기
    CEffect* pEffect = dynamic_cast<CEffect*>(CObjectPool::GetInst()->GetPoolObject(L"EffectPool"));
    if (!pEffect)
    {
        // 풀에 가용 객체가 없음 (풀 크기를 늘리거나, 생성 로직 추가 필요)
        return;
    }

    // 3. 이펙트 객체 설정
    CAnimator* pAnimator = pEffect->GetAnimator();
    if (pAnimator)
    {
        // 애니메이션이 아직 로드되지 않았다면 로드
        if (pAnimator->FindAnimation(data.animName) == nullptr)
        {
            pAnimator->LoadAnimation(data.animPath);
        }
    }
    
    pEffect->SetWorldPos(_vPos);
    pEffect->PlayEffect(data.animName); // 애니메이션 재생 및 자동 반환 설정

    // 4. 씬에 등장시키기
    CreateObject(pEffect, GROUP_TYPE::EFFECT);
}