#pragma once
#include "CGround.h"

class CNormalGround :
    public CGround
{
private:



public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

	CNormalGround();
	virtual ~CNormalGround();

	CLONE(CNormalGround)

};

