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
#include "CTimeMgr.h"
#include "CGround.h"
#include "CBackGround.h"
#include "CObjectPool.h"
#include "CResMgr.h"
#include "MouseCursor.h"
#include "Monster/CShooterMonster.h"

Scene_Stage_01::Scene_Stage_01()
{

}

Scene_Stage_01::~Scene_Stage_01()
{
}


void Scene_Stage_01::Update()
{

	//부모클래스의 update가 virtual이 아니기 때문에
	CScene::Update();


    if (KEY_TAP(KEY::O))
    {
        
        CMonPrefab::CreateMonster(MON_TYPE::SHOOTER, GetPlayer()->GetWorldPos());
    }
        
    

	if (KEY_TAP(KEY::ESC))
		ChangeScene(SCENE_TYPE::START);

	if (KEY_TAP(KEY::P))
	{
		cout << MOUSE_POS.x <<" "<<MOUSE_POS.y <<endl;
	}

    // 클리어 영역의 시작점과 끝점 가져오기
    Vec2 clearStartPos = GetSceneClearStartPos();
    Vec2 clearEndPos = GetSceneClearEndPos();

    // 플레이어의 현재 월드 위치 가져오기
    Vec2 playerPos = GetPlayer()->GetWorldPos();
    if (playerPos.x >= clearStartPos.x && playerPos.x <= clearEndPos.x &&
        playerPos.y >= clearStartPos.y && playerPos.y <= clearEndPos.y)
    {
        // 클리어 조건 만족! 씬 전환
        ChangeScene(SCENE_TYPE::START); // 예시로 START 씬으로 전환
    }
}

void Scene_Stage_01::Render(ID2D1RenderTarget* _pRenderTarget)
{
	CScene::Render(_pRenderTarget);
}

void Scene_Stage_01::Enter()
{
    CScene::Enter();
	//CCamera::GetInst()->FadeIn(2.f);

	//씬 진입 상황에서는 AddObject 해도 되지만 
	//Update 상황에서는 CreateObject식으로 이벤트로 오브젝트 생성
	Vec2 vResolution = CCore::GetInst()->GetResolution();


    // 백그라운드 설정
    CBackGround* backGround = new CBackGround;
    backGround->SetWorldPos(Vec2(0, 0));
    CTexture* back = CResMgr::GetInst()->LoadTexture(L"TutorialBack", L"texture\\background\\Forest_Mountain2.png");
    backGround->SetTexture(back);
    backGround->SetScale(CCore::GetInst()->GetResolution());
    AddObject((GameObject*)backGround, GROUP_TYPE::BACKGROUND);
    SetBackGround(backGround);

    
    // 타일 로딩
    LoadTile(L"Tile\\test31");
    // 불러온 타일 정보를 바탕으로 땅 생성
    CreateGround();


	//오브젝트 추가
	GameObject* player = new SPlayer();
	player->SetName(L"Player");
	player->SetWorldPos(GetPlayerSpawnPos()); //700,3000
	AddObject(player, GROUP_TYPE::PLAYER);
	RegisterPlayer(player);



	// 마우스커서 추가
	ShowCursor(false);

	GameObject* cursor = new MouseCursor();
	cursor->SetName(L"Cursor");
	cursor->SetWorldPos(player->GetWorldPos());
	AddObject(cursor, GROUP_TYPE::Ray);



    // 몬스터 오브젝트 풀 생성
    CObjectPool::GetInst()->CreatePool<CShooterHead>(L"ShooterHeadPool", 5);
    CObjectPool::GetInst()->CreatePool<CShooterMonster>(L"ShooterMonsterPool", 5);


	//클론함수 없이 만든 오브젝트 복사 만약 플레이어를 복사한다면 아래와 같이 사용
	/*GameObject* pOtherPlayer = new SPlayer(*(SPlayer*)player);
	pOtherPlayer->SetPos(Vec2(200.f, 200.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	//CCamera::GetInst()->SetTarget(player);



	//몬스터 배치 기본방식 1
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

	


	//클론 함수를 사용한 더 간단한 몬스터 복사
	/*GameObject* pOtherMonster = pMonsterObj->Clone();
	pOtherMonster->SetPos(Vec2(500.f, 400.f));
	AddObject(pOtherMonster, GROUP_TYPE::MONSTER);*/


	

	//땅 물체 배치
	/*GameObject* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(400.f, 900.f));
	pGround->SetScale(Vec2(32.f, 32.f));
	AddObject(pGround, GROUP_TYPE::GROUND);*/


	//CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::GROUND, Vec2(400.f, 500.f), Vec2(600.f, 600.f));
	//AddObject((GameObject*)pGround2, GROUP_TYPE::GROUND);

	

	//그룹간 충돌 체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::PROJ_MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::HOOK);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::GROUND, GROUP_TYPE::HOOK);
    
    //카메라 위치 지정
    CCamera::GetInst()->SetLookAt(vResolution/2.f);
    CCamera::GetInst()->SetTarget(player);

	//스타트 함수 호출
	Start();
}

void Scene_Stage_01::Exit()
{
	CCollisionMgr::GetInst()->Reset();
	ShowCursor(true);
    CCamera::GetInst()->SetTarget(nullptr);
    CScene::Exit();
}



