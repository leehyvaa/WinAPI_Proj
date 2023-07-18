#include "pch.h"
#include "SecondGameScene1.h"
#include "GameObject.h"
#include "GPlayer.h"
#include "GBoss.h"

SecondGameScene1::SecondGameScene1()
{
}

SecondGameScene1::~SecondGameScene1()
{
}


void SecondGameScene1::Enter()
{
	//오브젝트 추가
	/*GObject* pObj = new GObject;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::DEFAULT);*/
	
	GameObject* player = new GPlayer(Vec2(9, 9), Vec2(5, 5));
	GameObject* boss = new GBoss(Vec2(300, 300), Vec2(5, 5));
	
	player->SetTarget(boss);

	AddObject(player, GROUP_TYPE::PLAYER);
	AddObject(boss, GROUP_TYPE::MONSTER);
	
}

void SecondGameScene1::Exit()
{
}

