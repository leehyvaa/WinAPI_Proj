#include"pch.h"
#include "CGroundPrefab.h"
#include "CGround.h"

CGround* CGroundPrefab::CreateGround(GROUND_TYPE _type, Vec2 _pos1, Vec2 _pos2)
{
	CGround* pGround = nullptr;

	switch (_type)
	{
	case GROUND_TYPE::GROUND:
	{
		pGround = new CGround;
		pGround->SetName(L"Ground");
		pGround->SetPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x-_pos1.x ),abs(_pos2.y - _pos1.y)));

	

		/*AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);*/

		
	}
	break;
	case GROUND_TYPE::NONGROUND:
	{
		pGround = new CGround;
		pGround->SetName(L"NonGround");
		pGround->SetPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x - _pos1.x), abs(_pos2.y - _pos1.y)));

	}
		break;
	case GROUND_TYPE::DAMAGEZONE:
	{
		pGround = new CGround;
		pGround->SetName(L"DamageZone");
		pGround->SetPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x - _pos1.x), abs(_pos2.y - _pos1.y)));

	}
		break;

	case GROUND_TYPE::DEADZONE:
	{
		pGround = new CGround;
		pGround->SetName(L"DeadZone");
		pGround->SetPos(_pos1);
		pGround->SetScale(Vec2(abs(_pos2.x - _pos1.x), abs(_pos2.y - _pos1.y)));

	}
		break;
	}

	assert(pGround);
	return pGround;

}
