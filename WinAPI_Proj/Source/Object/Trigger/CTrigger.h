// Source/Object/Trigger/CTrigger.h
#pragma once
#include "GameObject.h"
#include "Prefab/CMonPrefab.h" // For MON_TYPE

// Forward declarations
class CMonster;
struct ID2D1RenderTarget;

// 트리거의 현재 상태를 나타내는 열거형
enum class TriggerState
{
    INACTIVE, // 플레이어의 진입을 기다리는 비활성 상태
    ACTIVE,   // 플레이어가 진입하여 벽이 올라오고 몬스터와 전투중인 상태
    COMPLETED // 모든 몬스터가 처치되어 완료된 상태
};

// 트리거가 스폰할 몬스터의 정보를 담는 구조체
struct MonsterSpawnInfo
{
    MON_TYPE eType;
    Vec2 vPos;

    void Save(FILE* _pFile);
    void Load(FILE* _pFile);
};

class CTrigger : public GameObject
{
private:
    TriggerState m_eState;

    // 맵 파일에서 로드될 데이터
    vector<wstring> m_vecWallNames;       // 제어할 벽 오브젝트들의 이름
    vector<MonsterSpawnInfo> m_vecMonsterSpawnInfo; // 스폰할 몬스터들의 정보

    // 런타임에 채워질 포인터
    vector<GameObject*> m_pWalls;           // 실제 벽 GameObject 포인터
    vector<CMonster*> m_pSpawnedMonsters; // 이 트리거가 스폰한 몬스터들의 포인터

    vector<GameObject*> m_pSampleMonsters;
    bool m_bDataResolved; // 이름 데이터를 포인터로 변환했는지 확인하는 플래그

public:
    CTrigger();
    CTrigger(const CTrigger& _origin);
    virtual ~CTrigger() override;

    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;

    // 씬의 모든 오브젝트가 로드된 후, 이름으로 벽을 찾아 포인터를 연결하는 함수
    void ResolveData();

private:
    // 트리거 상태에 따른 로직 함수
    void Activate();
    void Complete();
    void CheckPlayerEntry();
    void CheckCompletion();

public:
    // --- 맵 에디터용 함수 ---
    void AddWallName(const wstring& _name) { m_vecWallNames.push_back(_name); }
    void AddMonsterSpawnInfo(const MonsterSpawnInfo& _info) { m_vecMonsterSpawnInfo.push_back(_info); }
    const vector<wstring>& GetWallNames() const { return m_vecWallNames; }
    void ClearData();
    void AddSampleMonster(GameObject* _pMon) { m_pSampleMonsters.push_back(_pMon); }

    
    // 맵 파일 저장을 위한 직렬화/역직렬화 함수
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);

    CLONE(CTrigger)
};