#include "pch.h"
#include "SecondGameScene1.h"

#include "GameObject.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CMonster.h"
#include "SPlayer.h"

#include "CCollisionMgr.h"

SecondGameScene1::SecondGameScene1()
{
}

SecondGameScene1::~SecondGameScene1()
{
}


void SecondGameScene1::Enter()
{
	


	//������Ʈ �߰�
	/*GObject* pObj = new GObject;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::DEFAULT);*/
	
	GameObject* player = new SPlayer(Vec2(20, 20), Vec2(5, 5));
	GameObject* monster = new CMonster();
	monster->SetPos(Vec2(400.f, 100.f));
	monster->SetScale(Vec2(100.f, 100.f));

	

	AddObject(player, GROUP_TYPE::PLAYER);
	AddObject(monster, GROUP_TYPE::MONSTER);


	//�׷찣 �浹 üũ
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	
}

void SecondGameScene1::Exit()
{
	CCollisionMgr::GetInst()->Reset();
}

