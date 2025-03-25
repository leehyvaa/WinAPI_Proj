#include "pch.h"
#include "CObjectPool.h"

#include "CEventMgr.h"

CObjectPool::CObjectPool()
{
}

CObjectPool::~CObjectPool()
{
    ClearPool();
}

void CObjectPool::Init()
{
    // 초기화 코드
}



GameObject* CObjectPool::GetPoolObject(const wstring& _strKey)
{
    // 해당 키의 풀이 존재하는지 확인
    auto iter = m_mapPools.find(_strKey);
    if (iter == m_mapPools.end())
        return nullptr;
        
    vector<GameObject*>& pool = iter->second;
    
    // 비활성화된 오브젝트 찾기
    for (GameObject* pObj : pool)
    {
        if (!pObj->IsActive())
        {
            pObj->Reset(); // 상태 초기화
            return pObj;
        }
    }
    
    // 모든 오브젝트가 사용 중이면 새로 생성
    GameObject* pPrototype = pool[0]; // 첫 번째 오브젝트를 프로토타입으로 사용
    GameObject* pNewObj = pPrototype->Clone(); // 복제
    pNewObj->SetActive(true);
    pool.push_back(pNewObj); // 풀에 추가
    
    return pNewObj;
}

void CObjectPool::ReturnObject(GameObject* _pObj)
{
    if (!_pObj || !_pObj->IsManagedByPool())
        return;
        
    _pObj->SetActive(false);
    // 해당 오브젝트가 어느 풀에 속하는지는 이름으로 판단
    // 오브젝트의 이름이 풀의 키와 일치해야 함
}


void CObjectPool::ClearPool()
{
    // 모든 풀의 오브젝트 정리
    for (auto& pair : m_mapPools)
    {
        for (GameObject* pObj : pair.second)
        {
            DeleteObject(pObj);
        }
        pair.second.clear();
    }
    
    m_mapPools.clear();
}

void CObjectPool::ClearPoolByKey(const wstring& _strKey)
{
    auto iter = m_mapPools.find(_strKey);
    if (iter != m_mapPools.end())
    {
        for (GameObject* pObj : iter->second)
        {
            DeleteObject(pObj);
        }
        iter->second.clear();
        m_mapPools.erase(iter);
    }
}
