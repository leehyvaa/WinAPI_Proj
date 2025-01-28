#include "pch.h"
#include "CScene_Start.h"
#include "GameObject.h"
#include "CMonster.h"
#include "Player.h"
#include "CCore.h"
CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}


void CScene_Start::Enter()
{
	//오브젝트 추가
	/*GameObject* pObj = new Player;
	pObj->SetPos(Vec2(640.f,384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::DEFAULT);
	
	
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	float fTerm = (vResolution.x - (50.f * 2)) / 4.f;
	CMonster* pMonsterObj = nullptr;
	

	for (int i = 0; i < 5; i++)
	{
		pMonsterObj = new CMonster;
		pMonsterObj->SetPos(Vec2(50.f + (float)i*fTerm, 50.f));
		pMonsterObj->SetCenterPos(pMonsterObj->GetPos());
		pMonsterObj->SetScale(Vec2(50, 50));
		AddObject(pMonsterObj, GROUP_TYPE::DEFAULT);
	}*/
}

void CScene_Start::Exit()
{
}

