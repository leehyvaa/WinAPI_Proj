#pragma once
#include "GameObject.h"
#include "Monster/CShooterHead.h";

class CObjectPool
{
    SINGLE(CObjectPool)

private:
    map<wstring, vector<GameObject*>> m_mapPools;

public:
    void Init();
    
    // 미리 오브젝트 생성하여 풀 초기화
    template<typename T>
    void CreatePool(const wstring& _strKey, int _iPoolSize)
    {
        // 이미 풀이 존재하는지 확인
        if (m_mapPools.find(_strKey) != m_mapPools.end())
            return;
        
        vector<GameObject*>& pool = m_mapPools[_strKey];
    
        // 요청된 수만큼 객체 생성하여 풀에 추가
        for (int i = 0; i < _iPoolSize; ++i)
        {
            T* pObj = new T(); 
            pObj->SetName(_strKey); // 키를 이름으로 설정
            pObj->SetActive(false); // 비활성화 상태로 설정
            pObj->SetManagedByPool(true);
            pool.push_back(pObj);
        }  
    }
    
    // 풀에서 오브젝트 가져오기
    GameObject* GetPoolObject(const wstring& _strKey);
    
    // 오브젝트를 풀로 반환
    void ReturnObject(GameObject* _pObj);

    // 풀 정리
    void ClearPool();
    void ClearPoolByKey(const wstring& _strKey);

    // 풀 전체 가져오기
    map<wstring, vector<GameObject*>>& GetPools() { return m_mapPools; }
};





//
// // 폭발 이펙트 생성
// void CBullet::CreateExplosion()
// {
//     // 풀에서 폭발 이펙트 가져오기
//     GameObject* pEffect = CObjectPool::GetInst()->GetObject(L"Explosion");
//     if (pEffect)
//     {
//         // 총알 위치에 이펙트 생성
//         pEffect->SetPos(GetPos());
//         
//         // 씬에 이펙트 추가
//         tEvent evn = {};
//         evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
//         evn.lParam = (DWORD_PTR)pEffect;
//         evn.wParam = (DWORD_PTR)GROUP_TYPE::EFFECT;
//         CEventMgr::GetInst()->AddEvent(evn);
//     }
// }
// void CExplosionEffect::Update()
// {
//     // 비활성화 상태면 업데이트 안함
//     if (!IsActive())
//         return;
//         
//     // 애니메이션 업데이트
//     GameObject::Update();
//     
//     // 애니메이션이 끝났는지 확인
//     CAnimator* pAnimator = GetAnimator();
//     if (pAnimator && pAnimator->GetCurrentAnimation()->IsFinish())
//     {
//         // 애니메이션이 끝나면 풀로 반환
//         tEvent evn = {};
//         evn.eEvent = EVENT_TYPE::RETURN_TO_POOL;
//         evn.lParam = (DWORD_PTR)this;
//         CEventMgr::GetInst()->AddEvent(evn);
//     }
// }
//
// // 이펙트 초기화 (풀에서 가져올 때 호출됨)
// void CExplosionEffect::Reset()
// {
//     GameObject::Reset();
//     
//     // 애니메이션 재설정
//     if (GetAnimator())
//     {
//         GetAnimator()->Play(L"Explosion", false);
//     }
// }
//  