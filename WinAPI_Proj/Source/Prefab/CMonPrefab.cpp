#include"pch.h"
#include "CMonPrefab.h"
#include "CMonster.h"
#include "AI.h"

#include"CIdleState.h"
#include"CTraceState.h"
#include "CRigidBody.h"
#include "Monster/CShooterMonster.h"
#include "Shooter/CAimingState.h"
#include "Spawning/CSpawningState.h"

CMonster* CMonPrefab::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::SHOOTER:
	{
		    pMon = new CShooterMonster;
		    pMon->SetWorldPos(_vPos);
		    tMonInfo info = {};
		    info.fAtt = 1.f;
		    info.fAttRange = 2000.f;
		    info.fDetectRange = 2000.f;
		    info.fHP = 1.f;
		    info.fSpeed = 0.f;
		    info.iDir = 0;
	        
		    pMon->SetMonInfo(info);
	        pMon->SetName(L"Monster");
	        
	}
		break;

	case MON_TYPE::DEFENDER:
		break;

	}


    
    CreateObject(pMon, GROUP_TYPE::MONSTER);
    
	assert(pMon);
	return pMon;
}
