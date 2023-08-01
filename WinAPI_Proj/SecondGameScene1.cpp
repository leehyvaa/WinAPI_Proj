#include "pch.h"
#include "SecondGameScene1.h"

#include "GameObject.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CMonster.h"
#include "SPlayer.h"

#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
SecondGameScene1::SecondGameScene1()
{
}

SecondGameScene1::~SecondGameScene1()
{
}


void SecondGameScene1::Update()
{
	//부모클래스의 update가 virtual이 아니기 때문에
	CScene::Update();


	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}

void SecondGameScene1::Enter()
{
	


	//오브젝트 추가
	/*GObject* pObj = new GObject;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::DEFAULT);*/
	
	GameObject* player = new SPlayer();
	player->SetPos(Vec2(100.f, 100.f));
	player->SetScale(Vec2(100.f, 100.f));
	AddObject(player, GROUP_TYPE::PLAYER);

	//클로함수 없이 만든 오브젝트 복사 만약 플레이어를 복사한다면 아래와 같이 사용
	/*GameObject* pOtherPlayer = new SPlayer(*(SPlayer*)player);
	pOtherPlayer->SetPos(Vec2(200.f, 200.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/


	GameObject* monster = new CMonster();
	monster->SetName(L"Monster");
	monster->SetPos(Vec2(500.f, 200.f));
	monster->SetScale(Vec2(100.f, 100.f));
	AddObject(monster, GROUP_TYPE::MONSTER);


	//클론 함수를 사용한 더 간단한 복사
	GameObject* pOtherMonster = monster->Clone();
	pOtherMonster->SetPos(Vec2(500.f, 400.f));
	AddObject(pOtherMonster, GROUP_TYPE::MONSTER);

	//그룹간 충돌 체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);

}

void SecondGameScene1::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}

