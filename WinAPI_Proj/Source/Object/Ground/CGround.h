#pragma once
#include "GameObject.h"

struct ID2D1RenderTarget;

enum class COLLISION_SIDE {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

enum class WallType {
    NONE,
    HORIZONTAL,
    VERTICAL
};

enum class WallState {
    NONE,
    OPEN,
    CLOSE
};

class CGround : public GameObject
{
private:
    TILE_COLLIDE_TYPE m_eCollideType;    // 충돌 타입 필드
    GROUND_TYPE m_eGroundType;           // 그라운드 타입
    WallType m_eWallType;                // 벽 타입
public:

    TILE_COLLIDE_TYPE GetCollideType() { return m_eCollideType; }
    void SetCollideType(TILE_COLLIDE_TYPE _type){ m_eCollideType = _type; }
    GROUND_TYPE GetGroundType() { return m_eGroundType; }
    void SetGroundType(GROUND_TYPE _type){ m_eGroundType = _type; }

    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    
	virtual void Start();
	virtual void Update();
	virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;
	

public:
	CGround();
	virtual ~CGround();
	
	CLONE(CGround)


	friend class CGroundPrefab;

};



