#pragma once
#include "CGround.h"

class CNormalGround : public CGround
{
public:
	virtual void OnCollisionEnter(CCollider* _pOther) override;
	virtual void OnCollision(CCollider* _pOther) override;
	virtual void OnCollisionExit(CCollider* _pOther) override;

	CNormalGround();
	virtual ~CNormalGround();

	CLONE(CNormalGround)
};

