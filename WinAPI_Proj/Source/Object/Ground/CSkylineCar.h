#pragma once
#include "CGround.h"
#include <vector>


class CAnimator;

enum class SKYLINE_CAR_STATE
{
    IDLE,
    MOVING,
    EXPLODING,
    SPAWNING,
};

class CSkylineCar : public CGround
{
private:
    SKYLINE_CAR_STATE m_eState;
    std::vector<Vec2> m_vecPath;
    int m_iCurrentPathIndex;
    Vec2 m_vStartPos;
    Vec2 m_vVelocity;
    float m_fSpeed;

private:
    void SetupAnimations();
    void Update_Animation();

    void Update_Idle();
    void Update_Moving();
    void Update_Exploding();
    void Update_Spawning();

public:
    CSkylineCar();
    virtual ~CSkylineCar();

    virtual void Start() override;
    virtual void Update() override;
    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void OnCollision(CCollider* _pOther) override;
    virtual void OnCollisionExit(CCollider* _pOther) override;
 
    virtual void Reset() override;

    void SetPath(const std::vector<Vec2>& _path);
    void SetStartPos(const Vec2& _pos) { m_vStartPos = _pos; SetWorldPos(_pos); }
    SKYLINE_CAR_STATE GetState() const { return m_eState; }

    // Scene_Tool에서 사용할 함수
    void AddPathPoint(const Vec2& point) { m_vecPath.push_back(point); }
    void ClearPath();
    const vector<Vec2>& GetPath() const { return m_vecPath; }

    // 직렬화/역직렬화
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);

    CLONE(CSkylineCar)
};
