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
    if (!pool.empty())
    {
        GameObject* pPrototype = pool[0]; 
        GameObject* pNewObj = pPrototype->Clone(); 
        pNewObj->SetActive(true); // 새로 만든 객체도 활성화
        pNewObj->SetManagedByPool(true); // 풀 관리 객체임을 명시 (Clone 시 복사되지 않을 경우 대비)
        pool.push_back(pNewObj); // 풀에 추가
        return pNewObj;
    }

    
    return nullptr;
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
            pObj->SetManagedByPool(false);
            DeleteObject(pObj);
        }
        pair.second.clear();
    }
    
    m_mapPools.clear();
}

// 소멸자에서 쓰여선 안됨
void CObjectPool::ClearPoolByKey(const wstring& _strKey)
{
    auto iter = m_mapPools.find(_strKey);
    if (iter != m_mapPools.end())
    {
        for (GameObject* pObj : iter->second)
        {
            pObj->SetManagedByPool(false);
            DeleteObject(pObj);
        }
        iter->second.clear();
        m_mapPools.erase(iter);
    }
}
