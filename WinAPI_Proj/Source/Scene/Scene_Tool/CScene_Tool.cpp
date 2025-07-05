#include "pch.h"
#include "CScene_Tool.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CKeyMgr.h"
#include "CTile.h"

#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"

#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CUIMgr.h"
#include "CTexture.h"
#include "CTextUI.h"
#include "CGround.h"
#include "CWall.h"
#include "CBackGround.h"
#include "CCollider.h"
#include "CMonster.h"
#include "resource.h"
#include "Object/Ground/CSkylineCar.h"
#include "Object/Trigger/CTrigger.h"


CScene_Tool::CScene_Tool()
	: m_pTexUI(nullptr)
	, m_pPanelUI(nullptr)
	, m_iImgIndex(0)
	, m_vTilePos(Vec2(0, 0))
	, m_iImgTileX(-1)
	, m_iImgTileY(-1)
	, m_iImgTileIdx(-1)
    , m_iLastBotRightTileIdx(-1)
	, m_vImgTilePos(Vec2(0, 0))
	, m_eToolMode(TOOL_MODE::TEXTURE_MODE)
	, m_eGroundType(GROUND_TYPE::NORMAL)
	, m_bErase(false)
	, m_bSecondTex(false)
	, m_pModeText(nullptr)
	, m_iCurrentTriggerIndex(-1)
	, m_eCurrentMonsterType(MON_TYPE::SHOOTER)
	, m_pHelpText(nullptr)
	, m_bDraggingClearArea(false)
	, m_iTriggerAreaClickCount(0)
	, m_iWallAreaClickCount(0)
	, m_iWallAreaP1_TileIdx(-1)
	, m_iTriggerAreaP1_TileIdx(-1)
	, m_iCurrentCarIndex(-1)
{
    for (int i = 0; i < 5; ++i)
        m_arrTriggers[i] = nullptr;
    for (int i = 0; i < 10; ++i)
        m_arrCars[i] = nullptr;
}

CScene_Tool::~CScene_Tool()
{
}


void CScene_Tool::Enter()
{
    for (int i = 0; i < 10; ++i)
    {
        if (m_arrCars[i] == nullptr)
        {
            m_arrCars[i] = new CSkylineCar;
			m_arrCars[i]->SetName(L"SkylineCar_" + to_wstring(i));
            m_arrCars[i]->SetWorldPos(Vec2(0.f, 5000.f));
            AddObject(m_arrCars[i], GROUP_TYPE::GROUND);
        }
    }
    for (int i = 0; i < 5; ++i)
    {
        if (m_arrTriggers[i] == nullptr)
        {
            m_arrTriggers[i] = new CTrigger;
            AddObject(m_arrTriggers[i], GROUP_TYPE::TRIGGER);
        }
    }
	//메뉴 장착
	CCore::GetInst()->DockMenu();


	//타일 생성
	CreateTile(5, 5);

	//UI 하나 생성
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"parentUI");
	pPanelUI->SetScale(Vec2(320.f,350.f));
	pPanelUI->SetWorldPos(Vec2(vResolution.x - pPanelUI->GetScale().x,0.f));
	AddObject(pPanelUI, GROUP_TYPE::UI);
	m_pPanelUI = static_cast<CPanelUI*>(pPanelUI);

	CBtnUI* pBtnTileTex = new CBtnUI;
	pBtnTileTex->SetName(L"ChildUI");
	pBtnTileTex->SetScale(Vec2(320.f, 288.f));
	pBtnTileTex->SetWorldPos(Vec2(0.f, 30.f));
	((CBtnUI*)pBtnTileTex)->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC>(&CScene_Tool::SetTileUIIdx));
	
	pPanelUI->AddChild(pBtnTileTex);

	m_pTexUI = pBtnTileTex;
	LoadTileTexUI(L"texture\\tile");

	CBtnUI* pBtnPrev = new CBtnUI;
	pBtnPrev->SetName(L"ChildUI");
	pBtnPrev->SetScale(Vec2(32.f, 31.f));
	pBtnPrev->SetWorldPos(Vec2(1.f,288.f));
	//pBtnUI->SetClickedCallBack(&ChangeScene,0,0);
	((CBtnUI*)pBtnPrev)->SetClickedCallBack(this,static_cast<SCENE_MEMFUNC>(&CScene_Tool::PrevTileUI));
	CTexture* pBtnTexPrev = CResMgr::GetInst()->LoadTexture(L"UI_LEFT", L"Texture\\UI\\UI_LEFT.png");
	pBtnPrev->SetTexture(pBtnTexPrev);
	pBtnTileTex->AddChild(pBtnPrev);
	


	CBtnUI* pBtnNext = pBtnPrev->Clone();
	pBtnNext->SetWorldPos(Vec2(283.f, 288.f));
	((CBtnUI*)pBtnNext)->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC>(&CScene_Tool::NextTileUI));
	CTexture* pBtnTexNext = CResMgr::GetInst()->LoadTexture(L"UI_RIGHT", L"Texture\\UI\\UI_RIGHT.png");
	pBtnNext->SetTexture(pBtnTexNext);
	pBtnTileTex->AddChild(pBtnNext);


	CBtnUI* pBtnSave = pBtnPrev->Clone();
	pBtnSave->SetWorldPos(Vec2(140.f, 288.f));
	((CBtnUI*)pBtnSave)->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC>(&CScene_Tool::SaveTileData));
	//CTexture* pBtnTexNext = CResMgr::GetInst()->LoadTexture(L"UI_RIGHT", L"Tool\\Button\\UI_RIGHT.png");
	//pBtnNext->SetTexture(pBtnTexNext);
	pBtnTileTex->AddChild(pBtnSave);


	//UI 클론 하나 추가
	/*CUI* pClonepPanel = pPanelUI->Clone();
	pClonepPanel->SetPos(pClonepPanel->GetPos() + Vec2(-100.f, 0.f));
	((CBtnUI*)pClonepPanel->GetChildUI()[0])->SetClickedCallBack(&ChangeScene, 0, 0);

	AddObject(pClonepPanel, GROUP_TYPE::UI);
	

	m_pUI = pClonepPanel;*/

    // 텍스트 UI 초기화
    m_textureHelp.clear();
    m_groundHelp.clear();
    m_triggerHelp.clear();
    m_spawnHelp.clear();
    m_commonHelp.clear();
    
    m_pModeText = nullptr;  
    m_pHelpText = nullptr;
    m_pHelpSubText = nullptr;
    

    // 모드 텍스트 박스 생성
    m_pModeText = new CTextUI();
    m_pModeText->SetWorldPos(Vec2(900, 0));
    m_pModeText->SetAlign(CTextUI::TEXT_ALIGN::CENTER);
    m_pModeText->SetLineSpace(5);
    m_pModeText->SetVisibleBox(false);
    m_pModeText->SetFontSize(20);
    AddObject(m_pModeText, GROUP_TYPE::UI);


    // 설명서 텍스트 박스 생성
    m_pHelpText = new CTextUI();
    m_pHelpText->SetWorldPos(Vec2(0, 0));
    m_pHelpText->SetAlign(CTextUI::TEXT_ALIGN::LEFT);
    m_pHelpText->SetLineSpace(5);
    m_pHelpText->SetVisibleBox(false);
    m_pHelpText->SetFontSize(20);
    AddObject(m_pHelpText, GROUP_TYPE::UI);


    m_pHelpSubText = new CTextUI();
    m_pHelpSubText->SetWorldPos(Vec2(400, 0));
    m_pHelpSubText->SetAlign(CTextUI::TEXT_ALIGN::LEFT);
    m_pHelpSubText->SetLineSpace(5);
    m_pHelpSubText->SetVisibleBox(false);
    m_pHelpSubText->SetFontSize(20);
    AddObject(m_pHelpSubText, GROUP_TYPE::UI);


    // 기존 초기화 코드...

    // 모드별 설명 텍스트 초기화
    m_textureHelp = {
        L"[텍스처 모드]",
        L"1 - 배경 레이어",
        L"2 - 전경 레이어",
        L"BACK - 지우기",
        L"좌클릭 - 타일 배치",
        L"우클릭 - 타일 복사",
    };

    m_groundHelp = {
        L"[지형 모드]",
        L"1 - 이동 가능 지형",
        L"2 - 이동 불가 지형",
        L"3 - 데미지 지형",
        L"4 - 즉사 지형",
        L"BACK - 지우기",
        L"좌클릭 - 지형 콜라이더 위치 지정(지형의 왼쪽 위 지점에 클릭)",
        L"우클릭 - 지형 콜라이더 위치 지정(지형의 오른쪽 아래 지점에 클릭),",
        L"ENTER - 좌우 클릭으로 지정한 지형을 완성시키기",
    };

    m_triggerHelp = {
        L"[트리거 모드]",
        L"숫자 1~5 - 작업할 트리거 선택",
        L"좌클릭(두 번) - 트리거 영역 생성/조절",
        L"우클릭(두 번) - 트리거에 종속될 벽 영역 지정",
        L"M 키 - 몬스터 스폰 위치 지정",
        L"ENTER - 작업 완료",
        L"BACKSPACE - 현재 트리거 데이터 삭제",
    };

    m_carHelp = {
        L"[스카이라인 카 모드]",
        L"숫자 1-9, 0 - 작업할 카 선택",
        L"좌클릭 - 카 이동 경로 지정",
        L"ENTER - 경로 지정 완료",
        L"BACKSPACE - 현재 카 경로 데이터 삭제",
    };
    m_spawnHelp = {
        L"[스폰 모드]",
        L"1 - 플레이어 시작 위치 설정",
        L"2 - 씬 클리어 영역 설정",
        L"좌클릭 - 플레이어 위치 설정 (1번 모드)",
        L"드래그 - 클리어 영역 설정 (2번 모드)",
        L"CTRL+S - 스폰 데이터 저장",
        L"CTRL+L - 스폰 데이터 로드",
    };

    m_commonHelp = {
        L"[조작법]",
        L"F1 - 텍스처 그리기",
        L"F2 - 지형 생성",
        L"F3 - 트리거 생성",
        L"F4 - 스카이라인 카 생성",
  L"F9 - 플레이어 스폰,클리어 영역",
        L"",
        L"F6 - 타일 그리드 표시",
        L"F7 - 그라운드 타입 표시",
        L"F8 - 그라운드 완성 처리 디버깅",
        L"",
        L"CTRL - 타일맵 불러오기",
        L"ESC - 시작 화면으로"
    };
    
    // 공통 설명 표시
    m_pHelpText->AddLines(m_commonHelp);



	/*CBackGround* backGround2 = new CBackGround;
	backGround2->SetPos(Vec2(0, 0));
	backGround2->SetTexture(back2);
	backGround2->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround2, GROUP_TYPE::BACKGROUND);*/
	

	CBackGround* backGround = new CBackGround;
	backGround->SetWorldPos(Vec2(0, 0));
	CTexture* back = CResMgr::GetInst()->LoadTexture(L"TutorialBack", L"texture\\background\\Forest_Mountain2.png");
	
	backGround->SetTexture(back);
	backGround->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround, GROUP_TYPE::BACKGROUND);

	SetBackGround(backGround);
	

	//카메라 Look 지정
	CCamera::GetInst()->SetLookAt(vResolution/2.f);
}

void CScene_Tool::Exit()
{
    // 포인터 초기화를 DeleteAll() 전에 수행
    m_pModeText = nullptr;
    m_pHelpText = nullptr;
    m_pHelpSubText = nullptr;
    
    CCore::GetInst()->DivideMenu();
    CScene::Exit();
}

void CScene_Tool::Update()
{
	CScene::Update();
    m_pModeText->ClearLines();
    if (m_pHelpSubText && !m_textureHelp.empty() && !m_groundHelp.empty())
        {
            m_pHelpSubText->ClearLines();
            
            // 현재 모드의 설명만 표시
            switch(m_eToolMode) {
            case TEXTURE_MODE:
                m_pHelpSubText->AddLines(m_textureHelp);
                break;
            case GROUND_MODE:
                m_pHelpSubText->AddLines(m_groundHelp);
                break;
            case TRIGGER_MODE:
                m_pHelpSubText->AddLines(m_triggerHelp);
                break;
            case CAR_MODE:
                m_pHelpSubText->AddLines(m_carHelp);
                break;
            case SPAWN_MODE:
                m_pHelpSubText->AddLines(m_spawnHelp);
                break;
            }
        }

	if (KEY_TAP(KEY::ESC))
		ChangeScene(SCENE_TYPE::START);

    static wstring mode;
    static wstring subMode = L"None";
    static wstring write = L"writing";

	switch (m_eToolMode)
	{
	case TEXTURE_MODE:
	{
        mode = L"TextureMode";
		SetTileUIIdx();

		if(!m_pPanelUI->IsMouseOn())
			SetTileIdx();
	        
		if (KEY_TAP(KEY::KEY_1))
		{
            subMode = L"BackTexture";
			m_bSecondTex = false;
		}
		if (KEY_TAP(KEY::KEY_2))
		{
            subMode = L"FrontTexture";
			m_bSecondTex = true;
		}

	}
	break;
	case GROUND_MODE:
    {
        mode = L"GroundMode";

        if (KEY_TAP(KEY::KEY_1))
        {
            subMode = L"NORMALGROUND";
            m_eGroundType = GROUND_TYPE::NORMAL;
        }
        if (KEY_TAP(KEY::KEY_2))
        {
            subMode = L"UNWALKABLE";
            m_eGroundType = GROUND_TYPE::UNWALKABLE;
        }
        if (KEY_TAP(KEY::KEY_3))
        {
            subMode = L"DAMAGEZONE";
            m_eGroundType = GROUND_TYPE::DAMAGEZONE;
        }
        if (KEY_TAP(KEY::KEY_4))
        {
            subMode = L"DEADZONE";
            m_eGroundType = GROUND_TYPE::DEADZONE;
        }
	        
        if (KEY_HOLD(KEY::LBUTTON))
        {
            SettingTopLeftGround();
        }
        if (KEY_TAP(KEY::RBUTTON))
        {
            SettingBotRightGround();
        }
	    if (KEY_TAP(KEY::ENTER))
	    {
	        const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
            static_cast<CTile*>(vecTile[m_iLastTopLeftTileIdx])->SetBotRightTileIdx(m_iLastBotRightTileIdx);
	    }

 
    }
    break;
	case TRIGGER_MODE:
	       mode = L"TriggerMode";
	       subMode = L"Trigger " + to_wstring(m_iCurrentTriggerIndex + 1);
	       UpdateTriggerMode();
		break;
	   case CAR_MODE:
	       mode = L"CarMode";
	       subMode = L"Car " + to_wstring(m_iCurrentCarIndex);
	       UpdateCarMode();
	       break;
	   case SPAWN_MODE:
       {
           mode = L"SpawnMode";
           subMode = UpdateSpawnMode();
       }
       break;
	default:
		break;
	}




	//if (KEY_TAP(KEY::LSHIFT))
	//{
	//	//저장해 놓은 m_pUI를 포커싱
	//	//CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	//	SaveTileData();
	//}

	if (KEY_TAP(KEY::CTRL))
	{
		LoadTileData();
	}
    if (KEY_TAP(KEY::BACK))
    {
        m_bErase = !m_bErase;
        if (m_bErase)
            write = L"Erase";
        else
            write = L"Write";
    }

	if (KEY_TAP(KEY::F1))
	{
		m_eToolMode = TOOL_MODE::TEXTURE_MODE;
	    m_strCurTexFolder = L"texture\\tile";
		LoadTileTexUI(L"texture\\tile");
        m_pPanelUI->SetActive(true);
	}
	if (KEY_TAP(KEY::F2))
		m_eToolMode = TOOL_MODE::GROUND_MODE;
	if (KEY_TAP(KEY::F3))
    {
		m_eToolMode = TOOL_MODE::TRIGGER_MODE;
	    m_strCurTexFolder = L"texture\\enemySample";
		LoadTileTexUI(L"texture\\enemySample");
        m_pPanelUI->SetActive(true);
    }
	if (KEY_TAP(KEY::F4)) {
		m_eToolMode = TOOL_MODE::CAR_MODE;
		m_pPanelUI->SetActive(false);
	}
	if (KEY_TAP(KEY::F9))
		m_eToolMode = TOOL_MODE::SPAWN_MODE;

	   if (m_eToolMode == TOOL_MODE::CAR_MODE)
	   {
	       if (KEY_TAP(KEY::KEY_1)) m_iCurrentCarIndex = 1; if (KEY_TAP(KEY::KEY_2)) m_iCurrentCarIndex = 2;
	       if (KEY_TAP(KEY::KEY_3)) m_iCurrentCarIndex = 3; if (KEY_TAP(KEY::KEY_4)) m_iCurrentCarIndex = 4;
	       if (KEY_TAP(KEY::KEY_5)) m_iCurrentCarIndex = 5; if (KEY_TAP(KEY::KEY_6)) m_iCurrentCarIndex = 6;
	       if (KEY_TAP(KEY::KEY_7)) m_iCurrentCarIndex = 7; if (KEY_TAP(KEY::KEY_8)) m_iCurrentCarIndex = 8;
	       if (KEY_TAP(KEY::KEY_9)) m_iCurrentCarIndex = 9; if (KEY_TAP(KEY::KEY_0)) m_iCurrentCarIndex = 0;
	   }
	   else if (m_eToolMode == TOOL_MODE::TRIGGER_MODE)
	   {
	       if (KEY_TAP(KEY::KEY_1)) m_iCurrentTriggerIndex = 0;
	       if (KEY_TAP(KEY::KEY_2)) m_iCurrentTriggerIndex = 1;
	       if (KEY_TAP(KEY::KEY_3)) m_iCurrentTriggerIndex = 2;
	       if (KEY_TAP(KEY::KEY_4)) m_iCurrentTriggerIndex = 3;
	       if (KEY_TAP(KEY::KEY_5)) m_iCurrentTriggerIndex = 4;

	       if (KEY_TAP(KEY::KEY_1) || KEY_TAP(KEY::KEY_2) || KEY_TAP(KEY::KEY_3) || KEY_TAP(KEY::KEY_4) || KEY_TAP(KEY::KEY_5))
	       {
	           m_iTriggerAreaClickCount = 0, m_iWallAreaClickCount = 0;
	       }
	   }

    vector<wstring> modeText =
        {
	    mode,
	    subMode,
	    write,
	    };

   m_pModeText->AddLines(modeText);
}

// 클릭 시 현재 마우스 위치를 계산하여 해당 타일에 지정된 텍스처를 입히도록 요청한다.
void CScene_Tool::SetTileIdx()
{
	if (KEY_HOLD(KEY::RBUTTON))
	{
        int iCol = 0;
        int iRow = 0;
	    int iTileX = 0;
        if (!CalculateTileIndex(iCol, iRow,iTileX))
            return;

		if (m_iImgTileIdx < 0)
			return;

		UINT iIdx = iRow * iTileX + iCol;


		Vec2 tilePos = Vec2(static_cast<float>(iCol), static_cast<float>(iRow));

		if (tilePos == m_vTilePos)
			return;
		else
		{
			Vec2 diff = tilePos - m_vTilePos;
			diff += m_vImgTilePos;


			CTexture* tex = m_pTexUI->GetTexture();
			m_iImgTileX = static_cast<int>(tex->Width()) / TILE_SIZE;
			m_iImgTileY = static_cast<int>(tex->Height()) / TILE_SIZE;


			if (diff.x < 0.f || diff.y < 0.f ||
				diff.x >= m_iImgTileX || diff.y >= m_iImgTileY)
				return;

			int newTileIdx = static_cast<int>(diff.y) * m_iImgTileX + static_cast<int>(diff.x);


			const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

			if (m_bSecondTex)
			{
				static_cast<CTile*>(vecTile[iIdx])->SetTextureTwo(m_pTexUI->GetTexture());
				static_cast<CTile*>(vecTile[iIdx])->SetImgIdxTwo(newTileIdx);
			}
			else
			{
				static_cast<CTile*>(vecTile[iIdx])->SetTexture(m_pTexUI->GetTexture());
				static_cast<CTile*>(vecTile[iIdx])->SetImgIdx(newTileIdx);
			}
		
		}
		
		

	}
	if (KEY_TAP(KEY::LBUTTON))
	{
		DrawSelectTile();
	}

	if (KEY_HOLD(KEY::LBUTTON))
	{
		DrawSelectTile();
	}
    
}


// 타일 파일로부터 맵 데이터를 읽어오는 함수
void CScene_Tool::LoadTile(const wstring& _strFilePath)
{
    // 1. 부모 클래스의 표준 로더를 호출하여 모든 데이터를 로드
    CScene::LoadTile(_strFilePath);

    // 2. 툴씬에서 필요한 시각적 요소(벽, 샘플 몬스터)와 내부 참조 복원
    const vector<GameObject*>& vecTriggers = GetGroupObject(GROUP_TYPE::TRIGGER);

    // 3. 자동차 로드
    const vector<GameObject*>& vecCars = GetGroupObject(GROUP_TYPE::GROUND);
    int carIdx = 0;
    for (GameObject* pObj : vecCars)
    {
        if (carIdx >= 10) break;
        CSkylineCar* pCar = dynamic_cast<CSkylineCar*>(pObj);
        if (pCar)
        {
            m_arrCars[carIdx] = pCar;
            carIdx++;
        }
    }


    // m_arrTriggers 배열을 새로 로드된 트리거 객체로 다시 채움
    for (int i = 0; i < 5; ++i) m_arrTriggers[i] = nullptr;

    int idx = 0;
    for (GameObject* pObj : vecTriggers)
    {
        if (idx >= 5) break;
        CTrigger* pTrigger = dynamic_cast<CTrigger*>(pObj);
        if (pTrigger)
        {
            m_arrTriggers[idx] = pTrigger;

            // 로드된 벽 정보로 실제 CGround 객체 생성 (시각적 표시용)
            const auto& wallInfos = pTrigger->GetWallInfo();
            for (const auto& info : wallInfos)
            {
                CGround* pWall = new CGround();
                pWall->SetName(info.szName);
                pWall->SetWorldPos(info.vPos);
                pWall->SetScale(info.vScale);
                pWall->SetCollideType(TILE_COLLIDE_TYPE::SOLID);
                pWall->SetGroundType(GROUND_TYPE::UNWALKABLE);
                AddObject(pWall, GROUP_TYPE::GROUND);
                pWall->Start(); // 콜라이더 등 초기화
            }

            // 로드된 몬스터 정보로 샘플 몬스터 생성 (시각적 표시용)
            const auto& monsterInfos = pTrigger->GetMonsterSpawnInfo();
            for (const auto& info : monsterInfos)
            {
                SettingSampleMonster(info.vPos, info.eType, pTrigger);
            }
            idx++;
        }
    }
}

// 마우스 위치의 타일을 계산하고 해당 타일의 텍스처 변경 함수를 실행한다.
void CScene_Tool::DrawSelectTile()
{
    int iCol = 0;
    int iRow = 0;
    int iTileX =0;
    if (!CalculateTileIndex(iCol, iRow,iTileX)) return;
	if (m_iImgTileIdx < 0) return;
    
    // 선택된 타일의 인덱스 계산
	UINT iIdx = iRow * iTileX + iCol;
	
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

    
	if (!m_bErase)
	{
		if (m_bSecondTex)
		{
			static_cast<CTile*>(vecTile[iIdx])->SetTextureTwo(m_pTexUI->GetTexture());
			static_cast<CTile*>(vecTile[iIdx])->SetImgIdxTwo(m_iImgTileIdx);
		}
		else
		{
			static_cast<CTile*>(vecTile[iIdx])->SetTexture(m_pTexUI->GetTexture());
			static_cast<CTile*>(vecTile[iIdx])->SetImgIdx(m_iImgTileIdx);
		}
	}
	else
	{
		if (m_bSecondTex)
		{
			static_cast<CTile*>(vecTile[iIdx])->SetTextureTwo(nullptr);
			static_cast<CTile*>(vecTile[iIdx])->SetImgIdxTwo(-1);
		}
		else
		{
			static_cast<CTile*>(vecTile[iIdx])->SetTexture(nullptr);
			static_cast<CTile*>(vecTile[iIdx])->SetImgIdx(-1);
		}
	}

	m_vTilePos = Vec2(static_cast<float>(iCol), static_cast<float>(iRow));
}


// 지형의 왼쪽위 지점을 설정하는 함수
void CScene_Tool::SettingTopLeftGround()
{
    int iCol = 0;
    int iRow = 0;
    int iTileX = 0;
    if (!CalculateTileIndex(iCol, iRow,iTileX)) return;
    if (m_iImgTileIdx < 0) return;
    
    // 선택된 타일의 인덱스 계산
    UINT iIdx = iRow * iTileX + iCol;

    const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

    CTile* selectedTile = static_cast<CTile*>(vecTile[iIdx]);
    
    if (m_bErase)
    {
        selectedTile->SetGroundType(GROUND_TYPE::NONE);
        selectedTile->SetVertexPosition(VERTEX_POSITION::NONE);
        selectedTile->SetBotRightTileIdx(-1);
    }
    else
    {
        selectedTile->SetVertexPosition(VERTEX_POSITION::TOP_LEFT);
        m_iLastTopLeftTileIdx =iIdx;
        
        if (m_eGroundType == GROUND_TYPE::NORMAL)
            selectedTile->SetGroundType(GROUND_TYPE::NORMAL);
        if (m_eGroundType == GROUND_TYPE::UNWALKABLE)
            selectedTile->SetGroundType(GROUND_TYPE::UNWALKABLE);
        if (m_eGroundType == GROUND_TYPE::DAMAGEZONE)
            selectedTile->SetGroundType(GROUND_TYPE::DAMAGEZONE);
        if (m_eGroundType == GROUND_TYPE::DEADZONE)
            selectedTile->SetGroundType(GROUND_TYPE::DEADZONE);
    }
}

// 지형의 오른쪽 아래를 설정하는 함수
void CScene_Tool::SettingBotRightGround()
{
    int iCol = 0;
    int iRow = 0;
    int iTileX = 0;
    if (!CalculateTileIndex(iCol, iRow,iTileX)) return;
    if (m_iImgTileIdx < 0) return;
    
    UINT iIdx = iRow * iTileX + iCol;
    const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
    CTile* selectedTile = static_cast<CTile*>(vecTile[iIdx]);

    if (m_bErase)
    {
        selectedTile->SetVertexPosition(VERTEX_POSITION::NONE);
    }
    else
    {
        selectedTile->SetVertexPosition(VERTEX_POSITION::BOT_RIGHT);
        m_iLastBotRightTileIdx = iIdx;
    }
    
}


// 원본 텍스처의 선택한 위치의 idx를 기억하는 함수
void CScene_Tool::SetTileUIIdx()
{
	if (m_pTexUI->GetTexture() && m_pTexUI->IsLbtnDown())
	{
		CTexture* tex = m_pTexUI->GetTexture();
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
		vMousePos =vMousePos- m_pTexUI->GetFinalPos();
		vMousePos = CCamera::GetInst()->GetRenderPos(vMousePos);

		m_iImgTileX = static_cast<int>(tex->Width())/ TILE_SIZE;
		m_iImgTileY = static_cast<int>(tex->Height())/ TILE_SIZE;

		int iCol = static_cast<int>(vMousePos.x) / TILE_SIZE;
		int iRow = static_cast<int>(vMousePos.y) / TILE_SIZE;

        if (m_eToolMode == TOOL_MODE::TRIGGER_MODE)
        {
            if (m_iImgTileIdx == 0)
            {
                m_eCurrentMonsterType = MON_TYPE::SHOOTER;
            }
            else if (m_iImgTileIdx == 1)
            {
                m_eCurrentMonsterType = MON_TYPE::DEFENDER;
            }
        }

		else if (vMousePos.x < 0.f || m_iImgTileX <= iCol
			|| vMousePos.y < 0.f || m_iImgTileX <= iRow)
			return;

        // 원본텍스처에서 내가 클릭한 부분의 idx
		m_iImgTileIdx = iRow * m_iImgTileX + iCol;

		m_vImgTilePos = Vec2(iCol, iRow);
	}
}



/*
    SaveTile(파일경로)
    파일을 만들고, 씬의 타일개수를 가져와
    그 개수를 파일 앞부분에 적는다
    그리고 그 밑에다 그룹타입 Tile인 벡터의 사이즈만큼
    반복문을 돌면서 타일cpp의 Save함수를 실행
 */
void CScene_Tool::SaveTile(const wstring& _strFilePath)
{

	FILE* pFile = nullptr;
	_wfopen_s(&pFile,_strFilePath.c_str(),L"w");
	assert(pFile);

	// 1. 타일 데이터 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();
	fprintf(pFile, "[TileCount]\n");
	fprintf(pFile, "%d\n%d\n", xCount, yCount);
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	for (size_t i = 0; i < vecTile.size(); i++) {
		static_cast<CTile*>(vecTile[i])->Save(pFile);
	}

	// 2. 스폰 데이터 저장
	fprintf(pFile, "[SpawnData]\n");
	fprintf(pFile, "[PlayerSpawn]\n");
	fprintf(pFile, "%f %f %d\n", m_vPlayerSpawnPos.x, m_vPlayerSpawnPos.y, m_bPlayerSpawnSet ? 1 : 0);
	fprintf(pFile, "[SceneClear]\n");
	fprintf(pFile, "%f %f %f %f %d\n", m_vSceneClearStartPos.x, m_vSceneClearStartPos.y, m_vSceneClearEndPos.x, m_vSceneClearEndPos.y, m_bSceneClearSet ? 1 : 0);

	// 3. Car 데이터 저장
	const vector<GameObject*>& vecCars = GetGroupObject(GROUP_TYPE::GROUND);
	fprintf(pFile, "[CarCount]\n");
	fprintf(pFile, "%d\n", (int)vecCars.size());

	for (GameObject* pObj : vecCars)
	{
		CSkylineCar* pCar = dynamic_cast<CSkylineCar*>(pObj);
		if (pCar)
		{
			pCar->Save(pFile);
		}
	}


	   // 3. Trigger 데이터 저장 (씬에 있는 모든 트리거 저장)
	   const vector<GameObject*>& vecTriggers = GetGroupObject(GROUP_TYPE::TRIGGER);
	   fprintf(pFile, "[TriggerCount]\n");
	   fprintf(pFile, "%d\n", (int)vecTriggers.size());

	   for (GameObject* pObj : vecTriggers)
	   {
	       static_cast<CTrigger*>(pObj)->Save(pFile);
	   }


	fclose(pFile);
}



/*
    SaveTileDate()
    윈api ui를 사용한 파일저장 함수로
    저장시의 확장자 선택과 저장경로 등을 다룸
    만약GetSaveFileName(&ofn)) 이면 SaveTile(상대경로)	
    함수를 실행한다.
 */
void CScene_Tool::SaveTileData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};


	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Modal 방식
	if (GetSaveFileName(&ofn))
	{
		SaveTile(szName);
	}

}


// winAPI ui를 사용한 파일 불러오기 기능으로
// 타일 정보가 저장된 메모장 파일을 불러오는 함수
// 그 후에 GetOpenFileName(&ofn)) 이면 LoadTile함수로 타일 정보를 각각 가져온다.
void CScene_Tool::LoadTileData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};


	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Modal 방식
	if (GetOpenFileName(&ofn))
	{
		LoadTile(szName);
	}
}


// 폴더에서 타일 텍스처 파일들을 불러와서 저장하고 첫 번째 텍스처를 UI에 띄우는 함수
void CScene_Tool::LoadTileTexUI(const wstring& folderPath)
{
	m_vecTile_list.clear();
	m_iImgIndex = 0;
	m_strCurTexFolder = folderPath;

	WIN32_FIND_DATAA  data;
	wstring searchPath = CPathMgr::GetInst()->GetContentPath() + folderPath + L"\\*";
	string searchPathA = string(searchPath.begin(), searchPath.end());

	HANDLE hFind = FindFirstFileA(searchPathA.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			string fileName = data.cFileName;
			if (fileName != "." && fileName != ".." && (fileName.find(".png") != string::npos || fileName.find(".PNG") != string::npos))
			{
				m_vecTile_list.push_back(fileName);
			}
		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}

	if (m_vecTile_list.empty()) {
		m_pTexUI->SetTexture(nullptr);
		return;
	}
    ChangeTileTexUI();
}

// 현재 인덱스에 해당하는 텍스처 파일을 UI에 띄우도록 요청한다.
void CScene_Tool::ChangeTileTexUI()
{
	if (m_vecTile_list.empty() || m_iImgIndex >= m_vecTile_list.size())
		return;

	string strFileName = m_vecTile_list[m_iImgIndex];
	wstring wstrFileName(strFileName.begin(), strFileName.end());

	// 올바른 상대 경로 생성 (예: "texture\\tile\\mytile.png")
	wstring relativePath = m_strCurTexFolder + L"\\" + wstrFileName;

	// 리소스 매니저에 등록할 고유한 키를 생성 (상대경로 자체를 키로 사용하면 고유성 보장)
	wstring resKey = relativePath;

	CTexture* pTileTexture = CResMgr::GetInst()->LoadTexture(resKey, relativePath);
	m_pTexUI->SetTexture(pTileTexture);
}



void CScene_Tool::PrevTileUI()
{
	m_iImgIndex--;
	if (0 > m_iImgIndex || m_vecTile_list.size() <= m_iImgIndex)
		m_iImgIndex = static_cast<UINT>(m_vecTile_list.size()) - 1;

	ChangeTileTexUI();
}



void CScene_Tool::NextTileUI()
{
	m_iImgIndex++;
	if (0 > m_iImgIndex || m_vecTile_list.size() <= m_iImgIndex)
		m_iImgIndex = 0;

	ChangeTileTexUI();
}





bool CScene_Tool::CalculateTileIndex(int& iCol, int& iRow,int& iTileX)
{
    Vec2 vMousePos = MOUSE_POS;
    vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

    iTileX = static_cast<int>(GetTileX());
    int iTileY = static_cast<int>(GetTileY());

    iCol = static_cast<int>(vMousePos.x) / TILE_SIZE;
    iRow = static_cast<int>(vMousePos.y) / TILE_SIZE;

    if (vMousePos.x < 0.f || iTileX <= iCol
        || vMousePos.y < 0.f || iTileY <= iRow)
    {
        return false;
    }

    return true;
}




/*
    TileCountProc(윈도우핸들, message, wParam, lParam)
    맵의 타일 갯수를 지정해주는 Dialog메뉴를 열었을때
    여기에 입력한 param값들을 클래스 멤버변수인
    타일 x,y카운트에다 넣어준다.
    그리고 x,y로 이중반복을 돌려 createTile 함수까지 실행
 */
//Tile Count Window Proc
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			// ToolScene 확인
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
			pToolScene->CreateTile(iXCount, iYCount);



			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// 플레이어 스폰 위치 설정
void CScene_Tool::SetPlayerSpawnPos()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vWorldPos = vMousePos + vCamLook - vResolution / 2.f;

	CScene::SetPlayerSpawnPos(vWorldPos);

	// 디버그 메시지
	wchar_t szBuffer[256];
	swprintf_s(szBuffer, L"플레이어 스폰 위치 설정: (%.1f, %.1f)", vWorldPos.x, vWorldPos.y);
	MessageBox(nullptr, szBuffer, L"스폰 위치 설정", MB_OK);
}

// 씬 클리어 위치 설정 (드래그 영역)
void CScene_Tool::SetSceneClearPos()
{
	// 시작점과 끝점을 정규화 (왼쪽 위가 시작점, 오른쪽 아래가 끝점이 되도록)
	float minX = min(m_vSceneClearStartPos.x, m_vSceneClearEndPos.x);
	float minY = min(m_vSceneClearStartPos.y, m_vSceneClearEndPos.y);
	float maxX = max(m_vSceneClearStartPos.x, m_vSceneClearEndPos.x);
	float maxY = max(m_vSceneClearStartPos.y, m_vSceneClearEndPos.y);

	CScene::SetSceneClearPos(Vec2(minX, minY), Vec2(maxX, maxY));

	// 디버그 메시지
	wchar_t szBuffer[256];
	swprintf_s(szBuffer, L"씬 클리어 영역 설정: (%.1f, %.1f) ~ (%.1f, %.1f)",
		minX, minY, maxX, maxY);
	MessageBox(nullptr, szBuffer, L"클리어 영역 설정", MB_OK);
}



void CScene_Tool::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    // 1. 배경 렌더링
    CBackGround* bg = GetBackGround();
    if (bg) bg->Render(_pRenderTarget);

    // 2. 타일 및 그리드 렌더링 (RenderTile 함수 호출)
    RenderTile(_pRenderTarget);

    // 3. 툴씬 전용 오브젝트 렌더링
    for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
    {
        GROUP_TYPE eType = static_cast<GROUP_TYPE>(i);
        // 타일은 RenderTile에서 이미 그렸으므로 건너뜀
        if (eType == GROUP_TYPE::TILE || eType == GROUP_TYPE::BACKGROUND) continue;

        const vector<GameObject*>& vecObjects = GetGroupObject(eType);
        for (GameObject* pObj : vecObjects)
        {
            if (!pObj || pObj->IsDead()) continue;

            // 툴씬에서는 GROUND, TRIGGER, MONSTER, CAR를 항상 렌더링 (IsActive 무시)
            if (eType == GROUP_TYPE::GROUND || eType == GROUP_TYPE::TRIGGER || eType == GROUP_TYPE::MONSTER || eType == GROUP_TYPE::GROUND) {}
            else if (!pObj->IsActive()) continue; // 나머지 그룹은 Active일 때만

            // 실제 렌더링 로직
            pObj->Render(_pRenderTarget);
            pObj->Component_Render(_pRenderTarget);
        }
    }

    // 자동차 경로 렌더링
    for (int i = 0; i < 10; ++i)
    {
        CSkylineCar* pCar = m_arrCars[i];
        if (pCar && !pCar->GetPath().empty())
        {
            const auto& path = pCar->GetPath();
            ID2D1SolidColorBrush* pBrush = (i == m_iCurrentCarIndex) ? CCore::GetInst()->GetBrush(BrushType::GREEN) : CCore::GetInst()->GetBrush(BrushType::RED);

            for (size_t j = 0; j < path.size(); ++j)
            {
                Vec2 p1_render = CCamera::GetInst()->GetRenderPos(path[j]);
                D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(p1_render.x, p1_render.y), 5.f, 5.f);
                _pRenderTarget->FillEllipse(ellipse, pBrush);

                if (j + 1 < path.size())
                {
                    Vec2 p2_render = CCamera::GetInst()->GetRenderPos(path[j + 1]);
                    _pRenderTarget->DrawLine(D2D1::Point2F(p1_render.x, p1_render.y), D2D1::Point2F(p2_render.x, p2_render.y), pBrush, 2.f);
                }
            }
        }
    }

    // 스폰 위치 표시 (빨간 원)
    if (m_bPlayerSpawnSet)
    {
        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPlayerSpawnPos);
        ID2D1SolidColorBrush* pBrush = CCore::GetInst()->GetBrush(BrushType::RED);
        if (pBrush)
        {
            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(vRenderPos.x, vRenderPos.y), 20.0f, 20.0f);
            _pRenderTarget->FillEllipse(ellipse, pBrush);
            ID2D1SolidColorBrush* pOutlineBrush = CCore::GetInst()->GetCustomBrush(0.5f, 0, 0);
            if(pOutlineBrush) _pRenderTarget->DrawEllipse(ellipse, pOutlineBrush, 2.0f);
        }
    }

    // 클리어 영역 표시 (파란 사각형)
    if (m_bSceneClearSet)
    {
        Vec2 vRenderStartPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearStartPos);
        Vec2 vRenderEndPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearEndPos);
        ID2D1SolidColorBrush* pBrush = CCore::GetInst()->GetCustomBrush(0, 0, 1, 0.3f);

        if (pBrush)
        {
            D2D1_RECT_F rect = D2D1::RectF(
                vRenderStartPos.x,
                vRenderStartPos.y,
                vRenderEndPos.x,
                vRenderEndPos.y
            );
            _pRenderTarget->FillRectangle(rect, pBrush);
            ID2D1SolidColorBrush* pOutlineBrush = CCore::GetInst()->GetBrush(BrushType::BLUE);
            if(pOutlineBrush) _pRenderTarget->DrawRectangle(rect, pOutlineBrush, 2.0f);
        }
    }

    // 드래그 중인 클리어 영역 표시
    if (m_bDraggingClearArea)
    {
        Vec2 vRenderStartPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearStartPos);
        Vec2 vRenderEndPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearEndPos);
        ID2D1SolidColorBrush* pBrush = CCore::GetInst()->GetCustomBrush(1, 1, 0, 0.3f);

        if (pBrush)
        {
            D2D1_RECT_F rect = D2D1::RectF(
                min(vRenderStartPos.x, vRenderEndPos.x),
                min(vRenderStartPos.y, vRenderEndPos.y),
                max(vRenderStartPos.x, vRenderEndPos.x),
                max(vRenderStartPos.y, vRenderEndPos.y)
            );
            _pRenderTarget->FillRectangle(rect, pBrush);
            ID2D1SolidColorBrush* pOutlineBrush = CCore::GetInst()->GetCustomBrush(1, 0.65f, 0); // Orange
            if(pOutlineBrush) _pRenderTarget->DrawRectangle(rect, pOutlineBrush, 2.0f);
        }
    }

    // 마지막으로 UI 렌더링
    const auto& uiGroup = GetUIGroup();
    for(auto* pUI : uiGroup) if(pUI && !pUI->IsDead() && pUI->IsActive()) pUI->Render(_pRenderTarget);
}

void CScene_Tool::SettingSampleMonster(Vec2 pos, MON_TYPE eType, CTrigger* pOwnerTrigger)
{
    CMonster* obj = new CMonster();
    obj->SetWorldPos(pos);
    obj->CreateAnimator();

    if (eType == MON_TYPE::SHOOTER)
    {
        CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"RifleManTex", L"texture\\enemy\\rifleman\\RifleMan.png");
        obj->GetAnimator()->CreateAnimation(L"RIFLEMAN_IDLE_SAMPLE", pTex, Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 15, 2.f, Vec2(0.f, -64.f));
        obj->GetAnimator()->Play(L"RIFLEMAN_IDLE_SAMPLE", true);
    }

    obj->SetName(L"SampleMonster");
    AddObject(obj, GROUP_TYPE::MONSTER);
    pOwnerTrigger->AddSampleMonster(obj);
}

void CScene_Tool::UpdateTriggerMode()
{
    if (m_iCurrentTriggerIndex != -1)
    {
        CTrigger* pCurrentTrigger = m_arrTriggers[m_iCurrentTriggerIndex];
        if (!pCurrentTrigger) return;

        // LBUTTON: 트리거 영역 설정 (타일 기반)
        if (KEY_TAP(KEY::LBUTTON) && !m_pPanelUI->IsMouseOn())
        {
            int iCol, iRow, iTileX;
            if (CalculateTileIndex(iCol, iRow, iTileX)) // 마우스 위치의 타일 정보 계산
            {
                int iCurrentTileIdx = iRow * iTileX + iCol;
                const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

                if (m_iTriggerAreaClickCount % 2 == 0)
                {
                    // 첫 번째 클릭: 시작 타일 인덱스 저장
                    m_iTriggerAreaP1_TileIdx = iCurrentTileIdx;
                }
                else
                {
                    // 두 번째 클릭: 영역 계산 및 트리거 설정
                    if (m_iTriggerAreaP1_TileIdx != -1)
                    {
                        // 두 타일의 월드 좌표를 가져옴
                        Vec2 vPos1 = vecTile[m_iTriggerAreaP1_TileIdx]->GetWorldPos();
                        Vec2 vPos2 = vecTile[iCurrentTileIdx]->GetWorldPos();

                        // 영역의 왼쪽 위, 오른쪽 아래 좌표 계산
                        // 오른쪽 아래 좌표는 타일 크기만큼 더해줘서 전체 타일을 포함하도록 함
                        Vec2 vTopLeft(min(vPos1.x, vPos2.x), min(vPos1.y, vPos2.y));
                        Vec2 vBotRight(max(vPos1.x, vPos2.x) + TILE_SIZE, max(vPos1.y, vPos2.y) + TILE_SIZE);

                        // 계산된 값으로 트리거의 위치와 크기 설정
                        Vec2 vScale = vBotRight - vTopLeft;
                        pCurrentTrigger->SetWorldPos(vTopLeft);
                        pCurrentTrigger->SetScale(vScale);
                        pCurrentTrigger->GetCollider()->SetScale(vScale);
                        pCurrentTrigger->GetCollider()->SetOffsetPos(vScale / 2.f);
                        m_iTriggerAreaP1_TileIdx = -1; // 다음 생성을 위해 리셋
                    }
                }
                m_iTriggerAreaClickCount++;
            }
        }

        // RBUTTON: 벽 생성 (기존 코드 유지)
        if (KEY_TAP(KEY::RBUTTON) && !m_pPanelUI->IsMouseOn()) // 벽 생성 로직
        {
            int iCol, iRow, iTileX;
            if (CalculateTileIndex(iCol, iRow, iTileX))
            {
                int iCurrentTileIdx = iRow * iTileX + iCol;
                const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

                if (m_iWallAreaClickCount % 2 == 0)
                {
                    // 첫 번째 클릭: 타일 인덱스 저장
                    m_iWallAreaP1_TileIdx = iCurrentTileIdx;
                }
                else
                {
                    // 두 번째 클릭: 벽 생성
                    if (m_iWallAreaP1_TileIdx != -1)
                    {
                        Vec2 vPos1 = vecTile[m_iWallAreaP1_TileIdx]->GetWorldPos();
                        Vec2 vPos2 = vecTile[iCurrentTileIdx]->GetWorldPos();
                        Vec2 vTopLeft(min(vPos1.x, vPos2.x), min(vPos1.y, vPos2.y));
                        Vec2 vBotRight(max(vPos1.x, vPos2.x) + TILE_SIZE, max(vPos1.y, vPos2.y) + TILE_SIZE);

                        Vec2 vWallPos = vTopLeft;
                        Vec2 vWallScale = vBotRight - vTopLeft;

                        CWall* pWall = new CWall();
                        pWall->SetWorldPos(vWallPos);
                        pWall->SetScale(vWallScale);

                        // 벽 타입과 방향 설정
                        pWall->SetWallType(L"Gate1"); // 기본값
                        pWall->SetHorizontal(vWallScale.x > vWallScale.y); // 가로가 더 길면 수평

                        wstring wallName = L"TriggerWall_" + to_wstring(m_iCurrentTriggerIndex) + L"_" + to_wstring(m_iWallAreaClickCount / 2);
                        pWall->SetName(wallName);
                        AddObject(pWall, GROUP_TYPE::GROUND);

                        tWallInfo info;
                        info.szName = wallName;
                        info.vPos = vWallPos;
                        info.vScale = vWallScale;
                        pCurrentTrigger->AddWallInfo(info);

                        m_iWallAreaP1_TileIdx = -1; // 다음 생성을 위해 리셋
                    }
                }
                m_iWallAreaClickCount++;
            }
        }

        // M Key: 몬스터 스폰 위치 지정
        if (KEY_TAP(KEY::M) && !m_pPanelUI->IsMouseOn())
        {
            Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
            MonsterSpawnInfo info;
            info.vPos = vMousePos;
            info.eType = m_eCurrentMonsterType;
            pCurrentTrigger->AddMonsterSpawnInfo(info);
            SettingSampleMonster(vMousePos, m_eCurrentMonsterType, pCurrentTrigger);
        }

        // ENTER Key: 트리거 완성
        if (KEY_TAP(KEY::ENTER))
        {
            m_iCurrentTriggerIndex = -1;
        }

        // BACKSPACE Key: 트리거 데이터 삭제
        if (KEY_TAP(KEY::BACK))
        {
            // 1. 트리거에 등록된 벽(Ground) 오브젝트를 씬에서 삭제
            const vector<tWallInfo>& wallInfos = pCurrentTrigger->GetWallInfo();
            for (const auto& info : wallInfos)
            {
                GameObject* pWall = FindObjectByName(info.szName);
                if (pWall)
                {
                    DeleteObject(pWall);
                }
            }
            pCurrentTrigger->ClearData();
        }
    }
}

void CScene_Tool::UpdateCarMode()
{
	if (m_iCurrentCarIndex == -1) return;

	CSkylineCar* currentCar = m_arrCars[m_iCurrentCarIndex];
	if (!currentCar) return;

	// 좌클릭으로 경로 포인트 추가
	if (KEY_TAP(KEY::LBUTTON) && !m_pPanelUI->IsMouseOn())
	{
		Vec2 mousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		currentCar->AddPathPoint(mousePos);
	}

	// Enter로 경로 지정 완료
	if (KEY_TAP(KEY::ENTER))
	{
		m_iCurrentCarIndex = -1; // 선택 해제
	}

	// Backspace로 현재 선택된 차의 경로 삭제
	if (KEY_TAP(KEY::BACK))
	{
		currentCar->ClearPath();
	}
}

wstring CScene_Tool::UpdateSpawnMode()
{
    static bool bSpawnMode = true; // true: 플레이어 스폰, false: 씬 클리어
    static wstring subMode = L"PlayerSpawn";

    if (KEY_TAP(KEY::KEY_1))
    {
        subMode = L"PlayerSpawn";
        bSpawnMode = true;
    }
    if (KEY_TAP(KEY::KEY_2))
    {
        subMode = L"SceneClear";
        bSpawnMode = false;
    }

    if (!m_pPanelUI->IsMouseOn())
    {
        if (bSpawnMode)
        {
            // 플레이어 스폰 위치는 클릭으로 설정
            if (KEY_TAP(KEY::LBUTTON))
            {
                SetPlayerSpawnPos();
            }
        }
        else
        {
            // 씬 클리어 위치는 드래그로 영역 설정
            if (KEY_TAP(KEY::LBUTTON))
            {
                // 드래그 시작
                Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
                Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
                Vec2 vResolution = CCore::GetInst()->GetResolution();
                m_vSceneClearStartPos = vMousePos + vCamLook - vResolution / 2.f;
                m_bDraggingClearArea = true;
            }

            if (KEY_HOLD(KEY::LBUTTON) && m_bDraggingClearArea)
            {
                // 드래그 중
                Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
                Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
                Vec2 vResolution = CCore::GetInst()->GetResolution();
                m_vSceneClearEndPos = vMousePos + vCamLook - vResolution / 2.f;
            }

            if (KEY_AWAY(KEY::LBUTTON) && m_bDraggingClearArea)
            {
                // 드래그 완료
                SetSceneClearPos();
                m_bDraggingClearArea = false;
            }
        }
    }

    return subMode;
}
