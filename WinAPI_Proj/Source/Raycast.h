#pragma once
#include "GameObject.h"
class Raycast :
    public GameObject
{
private:
    GameObject* owner;
    CCollider* onCollisionRay;
    Vec2 targetPos;
    bool m_bRender;
    float m_fMaxWireRange;
    float m_fMaxMonsterSearchRange;
    
public:
    Raycast();
    ~Raycast();
    
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
    
    CCollider* GetCollisionRay() { return onCollisionRay; }
    Vec2 GetTargetPos() { return targetPos; }
    void SetMaxWireRange(float _f) { m_fMaxWireRange = _f; }
    void SetMaxMonsterSearchRange(float _f) { m_fMaxMonsterSearchRange = _f; }
    
    void SetOwner (GameObject* _owner) { owner = _owner; }
    
private:
    void CollisionCheck();
    bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

    CLONE(Raycast)
};

