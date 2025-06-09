#include "CNormalGround.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CCore.h"
#include "CSceneMgr.h"

CNormalGround::CNormalGround()
{
	CreateCollider();
}

CNormalGround::~CNormalGround()
{
}



void CNormalGround::OnCollisionEnter(CCollider* _pOther)
{

}

void CNormalGround::OnCollision(CCollider* _pOther)
{
}

void CNormalGround::OnCollisionExit(CCollider* _pOther)
{
}


