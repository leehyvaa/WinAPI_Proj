#pragma once
#include "GameObject.h"

class CObjectPool
{
    SINGLE(CObjectPool)

private:
    // 오브젝트 타입별로 풀 관리
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
            T* pObj = new T(); // 타입에 맞는 오브젝트 생성
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

    // 풀 가져오기
    map<wstring, vector<GameObject*>>& GetPools() { return m_mapPools; }
};

/*
// 더 간단하고 직관적인 방식
 GameObject* pObj = CObjectPool::GetInst()->GetObject(L"Bullet");
 if (pObj) {
     // 오브젝트 초기 설정
     pObj->SetPos(playerPos + Vec2(0, -20.f));
     pObj->SetDirection(playerDir);

     // 씬에 추가
     CSceneMgr::GetInst()->GetCurScene()->AddObject(pObj,
 GROUP_TYPE::PROJECTILE);
 }
 */


/*
// 풀 초기화 (게임 시작 시)
CObjectPool::GetInst()->CreatePool<CProjectile>(L"Projectile", 30);

// 오브젝트 사용
void UseProjectile()
{
    // 풀에서 오브젝트 가져오기
    GameObject* pProjectile = CObjectPool::GetInst()->GetObject(L"Projectile");
    if (pProjectile)
    {
        // 위치, 방향 등 설정
        pProjectile->SetPos(Vec2(100.f, 100.f));
        
        // 씬에 추가
        tEvent evn = {};
        evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
        evn.lParam = (DWORD_PTR)pProjectile;
        evn.wParam = (DWORD_PTR)GROUP_TYPE::PROJECTILE;
        CEventMgr::GetInst()->AddEvent(evn);
    }
}

// 사용 후 풀에 반환 (프로젝타일이 목표물과 충돌하거나 화면 밖으로 나갔을 때)
void ReturnToPool(GameObject* pObj)
{
    tEvent evn = {};
    evn.eEvent = EVENT_TYPE::RETURN_TO_POOL;
    evn.lParam = (DWORD_PTR)pObj;
    CEventMgr::GetInst()->AddEvent(evn);
}








 1. 게임 초기화 시 풀 생성 (main.cpp 또는 Scene의 Enter 함수에서)

// 게임 시작 시 자주 사용될 오브젝트들의 풀 미리 생성
void InitObjectPools()
{
    // 총알 풀 생성 (30개 미리 생성)
    CObjectPool::GetInst()->CreatePool<CBullet>(L"Bullet", 30);
    
    // 폭발 이펙트 풀 생성 (20개 미리 생성)
    CObjectPool::GetInst()->CreatePool<CExplosionEffect>(L"Explosion", 20);
    
    // 적 캐릭터 풀 생성 (10개 미리 생성)
    CObjectPool::GetInst()->CreatePool<CEnemy>(L"Enemy", 10);
}
2. 총알 발사 함수에서 사용 예시 (Player 클래스)

void CPlayer::Fire()
{
    // 총알 사운드 재생
    CSound* pSound = CResMgr::GetInst()->FindSound(L"bulletSound");
    pSound->Play();
    
    // 풀에서 총알 가져오기
    GameObject* pBullet = CObjectPool::GetInst()->GetObject(L"Bullet");
    if (pBullet)
    {
        // 총알 위치, 방향 설정
        Vec2 vPos = GetPos();
        vPos.y -= 10.f; // 약간 앞에서 발사되도록
        
        pBullet->SetPos(vPos);
        pBullet->SetDir(GetStdDir()); // 플레이어가 바라보는 방향
        
        // 씬에 총알 추가 (이벤트 매니저를 통해)
        tEvent evn = {};
        evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
        evn.lParam = (DWORD_PTR)pBullet;
        evn.wParam = (DWORD_PTR)GROUP_TYPE::PROJECTILE;
        CEventMgr::GetInst()->AddEvent(evn);
    }
}
3. 총알 클래스에서 수명 종료 처리 예시

void CBullet::Update()
{
    // 비활성화 상태면 업데이트 안함
    if (!IsActive())
        return;
        
    // 총알 이동
    Vec2 vPos = GetPos();
    Vec2 vDir = GetDir();
    
    vPos.x += vDir.x * 600.f * fDT;
    vPos.y += vDir.y * 600.f * fDT;
    
    SetPos(vPos);
    
    // 화면 밖으로 나갔는지 체크
    if (vPos.x < 0 || vPos.x > 1280 || vPos.y < 0 || vPos.y > 720)
    {
        // 풀로 반환
        ReturnToPool();
    }
}

// 풀로 반환하는 함수
void CBullet::ReturnToPool()
{
    tEvent evn = {};
    evn.eEvent = EVENT_TYPE::RETURN_TO_POOL;
    evn.lParam = (DWORD_PTR)this;
    CEventMgr::GetInst()->AddEvent(evn);
}

// 충돌 처리
void CBullet::OnCollisionEnter(CCollider* _pOther)
{
    // 적과 충돌했을 때
    if (_pOther->GetOwner()->GetGroup() == GROUP_TYPE::MONSTER)
    {
        // 폭발 이펙트 생성
        CreateExplosion();
        
        // 적에게 데미지
        Monster* pMonster = dynamic_cast<Monster*>(_pOther->GetOwner());
        if (pMonster)
            pMonster->TakeDamage(10);
            
        // 총알 풀로 반환
        ReturnToPool();
    }
}

// 폭발 이펙트 생성
void CBullet::CreateExplosion()
{
    // 풀에서 폭발 이펙트 가져오기
    GameObject* pEffect = CObjectPool::GetInst()->GetObject(L"Explosion");
    if (pEffect)
    {
        // 총알 위치에 이펙트 생성
        pEffect->SetPos(GetPos());
        
        // 씬에 이펙트 추가
        tEvent evn = {};
        evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
        evn.lParam = (DWORD_PTR)pEffect;
        evn.wParam = (DWORD_PTR)GROUP_TYPE::EFFECT;
        CEventMgr::GetInst()->AddEvent(evn);
    }
}
4. 적 스폰 매니저에서 사용 예시

void CSpawnManager::Update()
{
    // 스폰 타이머 업데이트
    m_fSpawnTimer += fDT;
    
    // 스폰 주기마다 적 생성
    if (m_fSpawnTimer >= m_fSpawnInterval)
    {
        SpawnEnemy();
        m_fSpawnTimer = 0.f;
    }
}

void CSpawnManager::SpawnEnemy()
{
    // 풀에서 적 객체 가져오기
    GameObject* pEnemy = CObjectPool::GetInst()->GetObject(L"Enemy");
    if (pEnemy)
    {
        // 랜덤 위치 설정
        float xPos = static_cast<float>(rand() % 1200 + 40);
        pEnemy->SetPos(Vec2(xPos, -50.f));
        
        // 적 초기화 (타입 등 추가 설정)
        CEnemy* enemy = dynamic_cast<CEnemy*>(pEnemy);
        if (enemy)
        {
            int type = rand() % 3; // 랜덤 적 타입
            enemy->SetEnemyType(static_cast<ENEMY_TYPE>(type));
        }
        
        // 씬에 적 추가
        tEvent evn = {};
        evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
        evn.lParam = (DWORD_PTR)pEnemy;
        evn.wParam = (DWORD_PTR)GROUP_TYPE::MONSTER;
        CEventMgr::GetInst()->AddEvent(evn);
    }
}
5. 이펙트 클래스에서 자동 반환 처리

void CExplosionEffect::Update()
{
    // 비활성화 상태면 업데이트 안함
    if (!IsActive())
        return;
        
    // 애니메이션 업데이트
    GameObject::Update();
    
    // 애니메이션이 끝났는지 확인
    CAnimator* pAnimator = GetAnimator();
    if (pAnimator && pAnimator->GetCurrentAnimation()->IsFinish())
    {
        // 애니메이션이 끝나면 풀로 반환
        tEvent evn = {};
        evn.eEvent = EVENT_TYPE::RETURN_TO_POOL;
        evn.lParam = (DWORD_PTR)this;
        CEventMgr::GetInst()->AddEvent(evn);
    }
}

// 이펙트 초기화 (풀에서 가져올 때 호출됨)
void CExplosionEffect::Reset()
{
    GameObject::Reset();
    
    // 애니메이션 재설정
    if (GetAnimator())
    {
        GetAnimator()->Play(L"Explosion", false);
    }
}
 */