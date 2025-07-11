#include"pch.h"
#include "CGroundPrefab.h"
#include "CGround.h"
#include "CNormalGround.h"
#include "CDamagedGround.h"

CGround* CGroundPrefab::CreateGround(GROUND_TYPE _type, Vec2 _pos1, Vec2 _pos2)
{
	CGround* pGround = nullptr;

	switch (_type)
	{
	case GROUND_TYPE::NORMAL:
	{
		pGround = new CNormalGround;
		pGround->SetName(L"Ground");
		pGround->SetWorldPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x-_pos1.x ),abs(_pos2.y - _pos1.y)));



		/*AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);*/


	}
	break;
	case GROUND_TYPE::DAMAGEZONE:
	{
		pGround = new CDamagedGround;
		pGround->SetName(L"DamageZone");
		pGround->SetWorldPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x - _pos1.x), abs(_pos2.y - _pos1.y)));

	}
		break;

	case GROUND_TYPE::DEADZONE:
	{
		pGround = new CNormalGround;  // DEADZONE은 즉시 밀어내는 방식이므로 CNormalGround 사용
		pGround->SetName(L"DeadZone");
		pGround->SetWorldPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x - _pos1.x), abs(_pos2.y - _pos1.y)));

	}
		break;
	}

	assert(pGround);
	return pGround;

}
