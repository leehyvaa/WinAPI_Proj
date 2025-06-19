// Source/Object/Trigger/CTrigger.cpp
#include "pch.h"
#include "CTrigger.h"
#include "CCollider.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "CResMgr.h"
#include "func.h" // For SaveWString, LoadWString

// MonsterSpawnInfo 직렬화/역직렬화
void MonsterSpawnInfo::Save(FILE* _pFile)
{
    fwrite(&eType, sizeof(MON_TYPE), 1, _pFile);
    fwrite(&vPos, sizeof(Vec2), 1, _pFile);
}

void MonsterSpawnInfo::Load(FILE* _pFile)
{
    fread(&eType, sizeof(MON_TYPE), 1, _pFile);
    fread(&vPos, sizeof(Vec2), 1, _pFile);
}


// CTrigger 구현
CTrigger::CTrigger()
    : m_eState(TriggerState::INACTIVE)
    , m_bDataResolved(false)
{
    // 트리거는 보이지 않지만, 영역을 감지하기 위해 충돌체가 필요합니다.
    CreateCollider();
    SetGroup(GROUP_TYPE::TRIGGER);
}

CTrigger::CTrigger(const CTrigger& _origin)
    : GameObject(_origin)
    , m_eState(TriggerState::INACTIVE)
    , m_vecWallNames(_origin.m_vecWallNames)
    , m_vecMonsterSpawnInfo(_origin.m_vecMonsterSpawnInfo)
    , m_bDataResolved(false) // 복제된 객체는 새로운 씬에서 데이터를 다시 찾아야 함
{
    // 복제된 오브젝트도 충돌체가 필요합니다.
    CreateCollider();
    GetCollider()->SetScale(GetScale());
    GetCollider()->SetOffsetPos(GetScale() / 2.f);
    SetGroup(GROUP_TYPE::TRIGGER);
}


CTrigger::~CTrigger()
{
    if (!m_pSampleMonsters.empty())
    {
        for (auto* pMon : m_pSampleMonsters)
        {
            if(pMon)
            {
                // Do not delete here, CScene will handle it.
            }
        }
        m_pSampleMonsters.clear();
    }
}

void CTrigger::Update()
{
    if (!IsActive()) return;

    // 씬 로딩 후 한 번만 이름 데이터를 실제 오브젝트 포인터로 변환합니다.
    if (!m_bDataResolved)
    {
        ResolveData();
    }

    switch (m_eState)
    {
    case TriggerState::INACTIVE:
        CheckPlayerEntry();
        break;
    case TriggerState::ACTIVE:
        CheckCompletion();
        break;
    case TriggerState::COMPLETED:
        // 트리거의 역할이 끝났으므로 비활성화하여 더 이상 업데이트되지 않게 합니다.
        SetActive(false);
        break;
    }
}

void CTrigger::FinalUpdate()
{
    // 충돌체가 항상 올바른 위치에 있도록 부모의 FinalUpdate를 호출합니다.
    GameObject::FinalUpdate();
}

void CTrigger::Render(ID2D1RenderTarget* _pRenderTarget)
{
    // 맵 에디터에서 영역을 시각적으로 확인하기 위해 디버그 모드에서 렌더링합니다.
    if (CSceneMgr::GetInst()->GetCurScene()->GetDrawCollider())
    {
        Component_Render(_pRenderTarget);
    }
}

void CTrigger::ResolveData()
{
    CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
    if (!pCurScene) return;

    // 저장된 벽 이름으로 씬에서 실제 벽 오브젝트를 찾아 포인터를 연결합니다.
    m_pWalls.clear();
    for (const auto& wallName : m_vecWallNames)
    {
        GameObject* pWall = pCurScene->FindObjectByName(wallName);
        if (pWall)
        {
            m_pWalls.push_back(pWall);
            // 트리거가 활성화되기 전까지 벽은 비활성화 상태여야 합니다.
            pWall->SetActive(false);
        }
    }
    m_bDataResolved = true;
}

void CTrigger::CheckPlayerEntry()
{
    CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
    if (!pCurScene) return;

    GameObject* pPlayer = pCurScene->GetPlayer();
    CCollider* pTriggerCollider = this->GetCollider();

    if (!pPlayer || !pPlayer->GetCollider() || !pTriggerCollider) return;

    // 플레이어와 트리거 영역의 충돌을 검사합니다.
    const AABB& playerAABB = pPlayer->GetCollider()->GetAABB();
    const AABB& triggerAABB = pTriggerCollider->GetAABB();

    if (playerAABB.Max.x > triggerAABB.Min.x && playerAABB.Min.x < triggerAABB.Max.x &&
        playerAABB.Max.y > triggerAABB.Min.y && playerAABB.Min.y < triggerAABB.Max.y)
    {
        Activate();
    }
}

void CTrigger::Activate()
{
    if (m_eState != TriggerState::INACTIVE) return;
    m_eState = TriggerState::ACTIVE;

    // 1. 벽을 활성화하여 길을 막습니다.
    for (GameObject* pWall : m_pWalls)
    {
        if (pWall) pWall->SetActive(true);
    }

    // 2. 몬스터를 스폰합니다.
    m_pSpawnedMonsters.clear();
    for (const auto& spawnInfo : m_vecMonsterSpawnInfo)
    {
        // 몬스터 프리팹을 사용해 몬스터를 생성하고 씬에 등록합니다.
        CMonster* pNewMon = CMonPrefab::CreateMonster(spawnInfo.eType, spawnInfo.vPos);
        if (pNewMon)
        {
            m_pSpawnedMonsters.push_back(pNewMon);
        }
    }
}

void CTrigger::CheckCompletion()
{
    // 스폰된 몬스터 리스트를 역순으로 순회하며 죽었는지 확인합니다.
    for (int i = (int)m_pSpawnedMonsters.size() - 1; i >= 0; --i)
    {
        CMonster* pMon = m_pSpawnedMonsters[i];
        if (!pMon || pMon->IsDead())
        {
            // 죽은 몬스터는 추적 리스트에서 제거합니다.
            m_pSpawnedMonsters.erase(m_pSpawnedMonsters.begin() + i);
        }
    }

    // 추적 리스트가 비었다면 모든 몬스터가 처치된 것입니다.
    if (m_pSpawnedMonsters.empty())
    {
        Complete();
    }
}

void CTrigger::Complete()
{
    if (m_eState != TriggerState::ACTIVE) return;
    m_eState = TriggerState::COMPLETED;

    // 벽을 비활성화하여 길을 엽니다.
    for (GameObject* pWall : m_pWalls)
    {
        if (pWall) pWall->SetActive(false);
    }
}

void CTrigger::ClearData()
{
    // Wall names are cleared by the scene tool which holds the wall objects.
    // The trigger only holds names.
    m_vecWallNames.clear();
    m_vecMonsterSpawnInfo.clear();

    // Delete sample monsters
    for (auto* pMon : m_pSampleMonsters) {
        DeleteObject(pMon);
    }
    m_pSampleMonsters.clear();

    // Reset trigger area
    SetWorldPos(Vec2(0, 0));
    SetScale(Vec2(0, 0));
    GetCollider()->SetScale(Vec2(0, 0));
    GetCollider()->SetOffsetPos(Vec2(0, 0));
                           
}

void CTrigger::Save(FILE* _pFile)
{
    // GameObject 기본 정보 저장
    SaveWString(GetName(), _pFile);
    Vec2 vPos = GetWorldPos();
    Vec2 vScale = GetScale();
    fwrite(&vPos, sizeof(Vec2), 1, _pFile);
    fwrite(&vScale, sizeof(Vec2), 1, _pFile);
    
    // 벽 이름 리스트 저장
    size_t wallCount = m_vecWallNames.size();
    fwrite(&wallCount, sizeof(size_t), 1, _pFile);
    for(const auto& name : m_vecWallNames)
    {
        SaveWString(name, _pFile);
    }

    // 몬스터 스폰 정보 리스트 저장
    size_t monsterCount = m_vecMonsterSpawnInfo.size();
    fwrite(&monsterCount, sizeof(size_t), 1, _pFile);
    for(auto& info : m_vecMonsterSpawnInfo)
    {
        info.Save(_pFile);
    }
}

void CTrigger::Load(FILE* _pFile)
{
    // GameObject 기본 정보 로드
    wstring name;
    LoadWString(name, _pFile);
    SetName(name);
    
    Vec2 vPos, vScale;
    fread(&vPos, sizeof(Vec2), 1, _pFile);
    fread(&vScale, sizeof(Vec2), 1, _pFile);
    SetWorldPos(vPos);
    SetScale(vScale);
    GetCollider()->SetScale(vScale);
    GetCollider()->SetOffsetPos(vScale / 2.f);

    // 벽 이름 리스트 로드
    size_t wallCount = 0;
    fread(&wallCount, sizeof(size_t), 1, _pFile);
    m_vecWallNames.resize(wallCount);
    for(size_t i = 0; i < wallCount; ++i)
    {
        LoadWString(m_vecWallNames[i], _pFile);
    }

    // 몬스터 스폰 정보 리스트 로드
    size_t monsterCount = 0;
    fread(&monsterCount, sizeof(size_t), 1, _pFile);
    m_vecMonsterSpawnInfo.resize(monsterCount);
    for(size_t i = 0; i < monsterCount; ++i)
    {
        m_vecMonsterSpawnInfo[i].Load(_pFile);
    }
}