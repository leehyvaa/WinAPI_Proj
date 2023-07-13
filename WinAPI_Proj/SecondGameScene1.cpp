#include "pch.h"
#include "SecondGameScene1.h"
#include "GameObject.h"
#include "GPlayer.h"

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
	
	GameObject* g_player = new GPlayer(Vec2(9, 9), Vec2(5, 5));


	AddObject(g_player, GROUP_TYPE::PLAYER); 
}

void SecondGameScene1::Exit()
{
}

