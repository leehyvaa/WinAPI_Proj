#include "pch.h"
#include "Scene_Stage_01.h"

#include "GameObject.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CMonster.h"
#include "SPlayer.h"
#include "PlayerArm.h"
#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CGround.h"
#include "CBackGround.h"
#include "CResMgr.h"
#include "MouseCursor.h"

Scene_Stage_01::Scene_Stage_01()

{
}

Scene_Stage_01::~Scene_Stage_01()
{
}


void Scene_Stage_01::Update()
{

	//�θ�Ŭ������ update�� virtual�� �ƴϱ� ������
	CScene::Update();




	if (KEY_TAP(KEY::ESC))
		ChangeScene(SCENE_TYPE::START);

	if (KEY_TAP(KEY::P))
	{
		cout << MOUSE_POS.x <<" "<<MOUSE_POS.y <<endl;
	}
	
}

void Scene_Stage_01::Render(HDC _dc)
{
	CScene::Render(_dc);
}

void Scene_Stage_01::Enter()
{
	//CCamera::GetInst()->FadeIn(2.f);

	//�� ���� ��Ȳ������ AddObject �ص� ������ 
	//Update ��Ȳ������ CreateObject������ �̺�Ʈ�� ������Ʈ ����
	Vec2 vResolution = CCore::GetInst()->GetResolution();



	//������Ʈ �߰�
	/*GObject* pObj = new GObject;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::DEFAULT);*/
	
	GameObject* player = new SPlayer();
	player->SetName(L"Player");
	player->SetPos(Vec2(700.f,3000.f));
	//player->SetScale(Vec2(100.f, 100.f));
	AddObject(player, GROUP_TYPE::PLAYER);
	RegisterPlayer(player);



	GameObject* playerArm = new PlayerArm();
	playerArm->SetName(L"PlayerArm");
	playerArm->SetPos(player->GetPos());
	((PlayerArm*)playerArm)->SetOwner((SPlayer*)player);
	((SPlayer*)player)->SetArm((PlayerArm*)playerArm);


	AddObject(playerArm, GROUP_TYPE::PLAYERARM);




	//���콺Ŀ��
	ShowCursor(false);

	GameObject* cursor = new MouseCursor();
	cursor->SetName(L"Cursor");
	cursor->SetPos(player->GetPos());
	AddObject(cursor, GROUP_TYPE::Ray);



	//Ŭ���Լ� ���� ���� ������Ʈ ���� ���� �÷��̾ �����Ѵٸ� �Ʒ��� ���� ���
	/*GameObject* pOtherPlayer = new SPlayer(*(SPlayer*)player);
	pOtherPlayer->SetPos(Vec2(200.f, 200.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	//CCamera::GetInst()->SetTarget(player);



	//���� ��ġ �⺻��� 1
	/*int iMonCount = 1;
	CMonster* pMonsterObj = nullptr;

	AI* pAI = new AI;
	pAI->AddState(new CIdleState);
	pAI->AddState(new CTraceState);

	for (size_t i = 0; i < iMonCount; i++)
	{
		pMonsterObj = new CMonster();
		pMonsterObj->SetName(L"Monster");
		pMonsterObj->SetScale(Vec2(100.f, 100.f));
		pMonsterObj->SetPos(vResolution /2.f - Vec2(0.f, 300.f));
		pMonsterObj->SetAI(pAI);

		AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	}*/

	


	//Ŭ�� �Լ��� ����� �� ������ ���� ����
	/*GameObject* pOtherMonster = pMonsterObj->Clone();
	pOtherMonster->SetPos(Vec2(500.f, 400.f));
	AddObject(pOtherMonster, GROUP_TYPE::MONSTER);*/


	//������ ���� ��ġ
	//CMonster* pMon = CMonPrefab::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	//CreateObject((GameObject*)pMon, GROUP_TYPE::MONSTER);
	//AddObject(pMon, GROUP_TYPE::MONSTER);



	//�� ��ü ��ġ
	/*GameObject* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(400.f, 900.f));
	pGround->SetScale(Vec2(32.f, 32.f));
	AddObject(pGround, GROUP_TYPE::GROUND);*/


	//CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::GROUND, Vec2(400.f, 500.f), Vec2(600.f, 600.f));
	//AddObject((GameObject*)pGround2, GROUP_TYPE::GROUND);

	//Ÿ�� �ε�
	LoadTile(L"Tile\\test11");


	//�׷찣 �浹 üũ
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::GROUND, GROUP_TYPE::PROJ_PLAYER);


	//ī�޶� ��ġ ����
	CCamera::GetInst()->SetLookAt(vResolution/2.f);
	CCamera::GetInst()->SetTarget(player);




	CBackGround* backGround = new CBackGround;
	backGround->SetPos(Vec2(0, 0));
	CTexture* back = CResMgr::GetInst()->LoadTexture(L"TutorialBack", L"texture\\background\\Forest_Mountain2.bmp");
	
	backGround->SetTexture(back);
	
	backGround->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround, GROUP_TYPE::BACKGROUND);

	SetBackGround(backGround);




	//��ŸƮ �Լ� ȣ��
	Start();
}

void Scene_Stage_01::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInst()->Reset();
	ShowCursor(true);


}



