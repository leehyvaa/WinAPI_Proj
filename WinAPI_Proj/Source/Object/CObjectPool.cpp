#include "pch.h"
#include "CObjectPool.h"

CObjectPool::CObjectPool()
{
}

CObjectPool::~CObjectPool()
{
    Clear();
}

void CObjectPool::Init()
{
    // 초기화 코드
}

template<typename T>
void CObjectPool::CreatePool(const wstring& _strKey, int _iPoolSize)
{
    // 이미 풀이 존재하는지 확인
    if (m_mapPools.find(_strKey) != m_mapPools.end())
        return;
        
    vector<GameObject*>& pool = m_mapPools[_strKey];
    
    // 요청된 수만큼 객체 생성하여 풀에 추가
    for (int i = 0; i < _iPoolSize; ++i)
    {
        T* pObj = new T(); // 타입에 맞는 오브젝트 생성
        pObj->SetName(_strKey); // 키를 이름으로 설정
        pObj->SetActive(false); // 비활성화 상태로 설정
        pool.push_back(pObj);
    }
}

GameObject* CObjectPool::GetObject(const wstring& _strKey)
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
            pObj->SetActive(true); // 활성화
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
    if (!_pObj)
        return;
        
    // 이미 풀에 반환된 상태(비활성화)면 무시
    if (!_pObj->IsActive())
        return;
        
    _pObj->SetActive(false);
    
    // 해당 오브젝트가 어느 풀에 속하는지는 이름으로 판단
    // 오브젝트의 이름이 풀의 키와 일치해야 함
}

void CObjectPool::Clear()
{
    // 모든 풀의 오브젝트 정리
    for (auto& pair : m_mapPools)
    {
        for (GameObject* pObj : pair.second)
        {
            delete pObj;
        }
        pair.second.clear();
    }
    
    m_mapPools.clear();
}