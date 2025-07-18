﻿#pragma once
#include "CNormalGround.h"
#include <vector>


class CAnimator;

enum class SKYLINE_CAR_STATE
{
    IDLE,
    MOVING,
    EXPLODING,
    SPAWNING,
};

class CSkylineCar : public CNormalGround
{
private:
    SKYLINE_CAR_STATE m_eState;
    std::vector<Vec2> m_vecPath;
    int m_iCurrentPathIndex;
    Vec2 m_vStartPos;
    Vec2 m_vVelocity;
    float m_fSpeed;             // This will be the max speed
    float m_fCurrentSpeed;      // Current speed, used for acceleration
    float m_fAccelerationTime;  // Timer for acceleration
    bool m_bIsVertical;        // 수직/수평 여부
    float m_fExplosionTimer;    // Timer for explosion sequence

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

    void SetVertical(bool _bVertical) { m_bIsVertical = _bVertical; }
    bool IsVertical() const { return m_bIsVertical; }

    // Scene_Tool에서 사용할 함수
    void AddPathPoint(const Vec2& point);
    void ClearPath();
    const vector<Vec2>& GetPath() const { return m_vecPath; }

    // 직렬화/역직렬화
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);

    CLONE(CSkylineCar)
};
