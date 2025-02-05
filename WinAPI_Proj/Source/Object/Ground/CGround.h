#pragma once
#include "GameObject.h"


class CGround : public GameObject
{
private:
    TILE_COLLIDE_TYPE m_eCollideType; // 충돌 타입 필드
    
public:

    TILE_COLLIDE_TYPE GetCollideType() { return m_eCollideType; }
    void SetCollideType(TILE_COLLIDE_TYPE _type){ m_eCollideType = _type; }


    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    
	virtual void Start();
	virtual void Update();
	virtual void Render(HDC _dc);
	

	virtual void Save(FILE* _file);
	virtual void Load(FILE* _file);

public:
	CGround();
	virtual ~CGround();
	
	CLONE(CGround)


	friend class CGroundPrefab;

};

