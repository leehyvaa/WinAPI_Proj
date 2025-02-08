#pragma once
#include "GameObject.h"


class CGround : public GameObject
{
private:
    TILE_COLLIDE_TYPE m_eCollideType;    // 충돌 타입 필드
    GROUND_TYPE m_eGroundType;           // 그라운드 타입
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
	virtual void Render(HDC _dc);
	

	//virtual void Save(FILE* _file);
	//virtual void Load(FILE* _file);

    void NormalTopCollisionEnter(CCollider* _pOther);
    void NormalBotCollisionEnter(CCollider* _pOther);
    void NormalRightCollisionEnter(CCollider* _pOther);
    void NormalLeftCollisionEnter(CCollider* _pOther);

    void NormalTopCollision(CCollider* _pOther);
    void NormalBotCollision(CCollider* _pOther);
    void NormalRightCollision(CCollider* _pOther);
    void NormalLeftCollision(CCollider* _pOther);

    

public:
	CGround();
	virtual ~CGround();
	
	CLONE(CGround)


	friend class CGroundPrefab;

};



