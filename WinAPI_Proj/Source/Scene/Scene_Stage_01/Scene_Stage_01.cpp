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
#include "CAnimator.h"
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
#include "Object/UI/DamageEffect/CDamageEffectUI.h"
#include "Object/Trigger/CTrigger.h" // ADDED

Scene_Stage_01::Scene_Stage_01()
    : m_bPlayerDeathMode(false)
    , m_fDeathSceneTimer(1.0f)  // 1초로 초기화
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


    
    Vec2 clearStartPos = GetSceneClearStartPos();
    Vec2 clearEndPos = GetSceneClearEndPos();

    // 플레이어가 스테이지 클리어
    Vec2 playerPos = GetPlayer()->GetWorldPos();
    if (playerPos.x >= clearStartPos.x && playerPos.x <= clearEndPos.x &&
        playerPos.y >= clearStartPos.y && playerPos.y <= clearEndPos.y)
    {
        ChangeScene(SCENE_TYPE::START); 
    }

    
    // 플레이어 죽음 감지 및 처리
    SPlayer* pPlayer = static_cast<SPlayer*>(GetPlayer());
    if (pPlayer && pPlayer->GetState() == PLAYER_STATE::DEAD)
    {
        if (!m_bPlayerDeathMode)
        {
            m_bPlayerDeathMode = true;
            SetBackGround(nullptr);
        }
        
        // 죽음 애니메이션이 완료되면 1초 후 타이틀 씬으로 전환
        if (pPlayer->IsDeathAnimationCompleted())
        {
            m_fDeathSceneTimer -= fDT;
            if (m_fDeathSceneTimer <= 0.f)
            {
                ChangeScene(SCENE_TYPE::START);
            }
        }
    }
}

void Scene_Stage_01::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (m_bPlayerDeathMode)
    {
        SPlayer* pPlayer = static_cast<SPlayer*>(GetPlayer());
        if (pPlayer && pPlayer->GetAnimator())
        {
            pPlayer->GetAnimator()->Render(_pRenderTarget);
        }
        
        CCamera::GetInst()->Render(_pRenderTarget);
    }
    else
    {
        CScene::Render(_pRenderTarget);
    }
}

void Scene_Stage_01::Enter()
{
    CScene::Enter();
    
    // 죽음 모드 플래그 및 타이머 초기화
    m_bPlayerDeathMode = false;
    m_fDeathSceneTimer = 1.0f;
    
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

    
    // 타일 로딩 (상대 경로 사용)
    LoadTile(L"Tile\\0624_11");
    // 불러온 타일 정보를 바탕으로 땅 생성
    CreateGround();

    // 트리거 및 벽 생성, 연결
    const vector<GameObject*>& vecTriggers = GetGroupObject(GROUP_TYPE::TRIGGER);
    for (GameObject* pObj : vecTriggers)
    {
        CTrigger* pTrigger = dynamic_cast<CTrigger*>(pObj);
        if (pTrigger)
        {
            // 1. 로드된 벽 정보로 실제 CGround 객체 생성
            const auto& wallInfos = pTrigger->GetWallInfo();
            for (const auto& info : wallInfos)
            {
                CGround* pWall = new CGround();
                pWall->SetName(info.szName);
                pWall->SetWorldPos(info.vPos);
                pWall->SetScale(info.vScale);
                pWall->SetCollideType(TILE_COLLIDE_TYPE::SOLID);
                pWall->SetGroundType(GROUND_TYPE::UNWALKABLE);
                pWall->SetActive(false); // 처음에는 비활성화
                AddObject(pWall, GROUP_TYPE::GROUND);
            }
            
            // 2. 생성된 벽 객체와 트리거 연결
            pTrigger->ResolveData();
        }
    }
   

 //오브젝트 추가
	GameObject* player = new SPlayer();
	player->SetName(L"Player");
	player->SetWorldPos(GetPlayerSpawnPos()); //700,3000
	AddObject(player, GROUP_TYPE::PLAYER);
	RegisterPlayer(player);

	// 플레이어의 데미지 이펙트 UI를 씬에 등록
	SPlayer* pPlayer = static_cast<SPlayer*>(player);
	if (pPlayer && pPlayer->GetDamageEffectUI())
	{
		CDamageEffectUI* pDamageEffect = pPlayer->GetDamageEffectUI();
		
		// 화면 전체 크기로 설정 (오버레이 효과)
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		pDamageEffect->SetWorldPos(Vec2(0.f, 0.f));
		pDamageEffect->SetScale(Vec2(static_cast<float>(vResolution.x), static_cast<float>(vResolution.y)));
		
		// UI 시스템에 등록하여 렌더링 활성화
		AddObject(pDamageEffect, GROUP_TYPE::UI);
	}

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

	

	// 그룹간 충돌 체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::PROJ_MONSTER);
	   CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::TRIGGER); // 플레이어와 트리거 충돌 체크 // ADDED
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::HOOK);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::GROUND, GROUP_TYPE::HOOK);
	   
	   // 카메라 위치 지정
    CCamera::GetInst()->SetLookAt(vResolution/2.f);
    CCamera::GetInst()->SetTarget(player);

	// 스타트 함수 호출
	Start();
}

void Scene_Stage_01::Exit()
{
	CCollisionMgr::GetInst()->Reset();
	ShowCursor(true);
    CCamera::GetInst()->SetTarget(nullptr);
    CScene::Exit();
}



// 원본 텍스처의 선택한 위치의 idx를 기억하는 함수
void Scene_Stage_01::LoadTile(const wstring& _strRelativePath)
{
    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;
    CScene::LoadTile(strFilePath);
}
