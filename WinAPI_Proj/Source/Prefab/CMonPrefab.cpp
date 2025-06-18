#include"pch.h"
#include "CMonPrefab.h"
#include "CMonster.h"
#include "AI.h"
#include "CObjectPool.h"

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
		pMon = dynamic_cast<CShooterMonster*>(CObjectPool::GetInst()->GetPoolObject(L"ShooterMonsterPool"));
		if (pMon)
		{
			// 전달받은 위치 매개변수를 사용하여 몬스터 위치 설정
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
			pMon->SetActive(true);
		}
	}
		break;

	case MON_TYPE::DEFENDER:
		// TODO: DEFENDER 타입 몬스터 구현 필요
		break;
	}

	// 몬스터가 성공적으로 생성된 경우에만 오브젝트로 등록
	if (pMon)
	{
		CreateObject(pMon, GROUP_TYPE::MONSTER);
	}
	
	assert(pMon);
	return pMon;
}
