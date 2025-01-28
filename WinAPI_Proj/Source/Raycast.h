#pragma once
#include "GameObject.h"
class Raycast :
    public GameObject
{
private:
    CCollider* onCollisionRay;
    Vec2 targetPos;

public:
    Raycast();
    ~Raycast();

    virtual void Update() override;
    virtual void Render(HDC hdc) override;
    
    CCollider* GetCollisionRay() { return onCollisionRay; }
    Vec2 GetTargetPos() { return targetPos; }

private:
    void CollisionCheck();
    bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

    CLONE(Raycast)
};

