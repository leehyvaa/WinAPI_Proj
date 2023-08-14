#pragma once
#include "GameObject.h"


class CGround : public GameObject
{
private:

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);


	virtual void Start();
	virtual void Update();
	virtual void Render(HDC _dc);
	

public:
	CGround();
	virtual ~CGround();
	
	CLONE(CGround)


	friend class CGroundPrefab;

};

