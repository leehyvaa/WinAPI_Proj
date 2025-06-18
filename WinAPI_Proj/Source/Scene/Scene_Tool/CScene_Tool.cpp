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
#include "CBackGround.h"
#include "CMonster.h"
#include "resource.h"


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
    , m_pHelpText(nullptr)
    , m_vPlayerSpawnPos(Vec2(0, 0))
    , m_vSceneClearStartPos(Vec2(0, 0))
    , m_vSceneClearEndPos(Vec2(0, 0))
    , m_bPlayerSpawnSet(false)
    , m_bSceneClearSet(false)
    , m_bDraggingClearArea(false)
{
}

CScene_Tool::~CScene_Tool()
{
}


void CScene_Tool::Enter()
{
    
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

    m_monsterHelp = {
        L"[몬스터 모드]",
        L"좌클릭 - 몬스터 배치 (지도 캔버스 영역)",
        L"우측 상단 - 몬스터 타입 선택 UI",
        L"CTRL+S - 몬스터 데이터 저장",
        L"CTRL+L - 몬스터 데이터 로드",
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
        L"F4 - 몬스터 생성",
        L"F5 - 플레이어 스폰,클리어 영역",
        L"",
        L"F8 - 타일 그리드 표시",
        L"F9 - 그라운드 타입 표시",
        L"F10 - 그라운드 완성 처리 디버깅",
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
            case MONSTER_MODE:
                m_pHelpSubText->AddLines(m_monsterHelp);
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
    case MONSTER_MODE:
       {
           mode = L"MonsterMode";
   
           if (!m_pPanelUI->IsMouseOn())
           {
               // 몬스터 배치 처리
               HandleMonsterPlacement();
           }
       }
       break;
    case SPAWN_MODE:
       {
           mode = L"SpawnMode";
   
           static bool bSpawnMode = true; // true: 플레이어 스폰, false: 씬 클리어
   
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
       }
       break;
	case TRIGGER_MODE:
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
	}
	if (KEY_TAP(KEY::F2))
		m_eToolMode = TOOL_MODE::GROUND_MODE;
	if (KEY_TAP(KEY::F3))
		m_eToolMode = TOOL_MODE::TRIGGER_MODE;
	if (KEY_TAP(KEY::F4))
	{
		m_eToolMode = TOOL_MODE::MONSTER_MODE;
	    m_strCurTexFolder = L"texture\\enemySample";
		LoadTileTexUI(L"texture\\enemySample");
	}
	if (KEY_TAP(KEY::F5))
		m_eToolMode = TOOL_MODE::SPAWN_MODE;

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

		if (vMousePos.x < 0.f || m_iImgTileX <= iCol
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
	_wfopen_s(&pFile,_strFilePath.c_str(),L"wb");
	assert(pFile);

	//타일 가로세로 개수 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fprintf(pFile, "[TileCount]\n");
	fprintf(pFile, "%d\n", static_cast<int>(xCount));
	fprintf(pFile, "%d\n", static_cast<int>(yCount));

	fprintf(pFile,"\n");

	//모든 타일들을 개별적으로 저장할 데이터를 저장하게 함
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	//const vector<GameObject*>& vecGround = GetGroupObject(GROUP_TYPE::GROUND);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		static_cast<CTile*>(vecTile[i])->Save(pFile);
	}

	// fprintf(pFile, "[GroundCount]\n");
	// fprintf(pFile, "%d\n", static_cast<int>(GetGroundCount()));
	// for (size_t i = 0; i < vecGround.size(); i++)
	// {
	// 	static_cast<CGround*>(vecGround[i])->Save(pFile);
	// }

	// 스폰 데이터 저장 추가
	fprintf(pFile, "[SpawnData]\n");

	// 플레이어 스폰 위치 저장
	fprintf(pFile, "[PlayerSpawn]\n");
	fprintf(pFile, "%.1f\n", m_vPlayerSpawnPos.x);
	fprintf(pFile, "%.1f\n", m_vPlayerSpawnPos.y);
	fprintf(pFile, "%d\n", m_bPlayerSpawnSet ? 1 : 0);

	// 씬 클리어 영역 저장
	fprintf(pFile, "[SceneClear]\n");
	fprintf(pFile, "%.1f\n", m_vSceneClearStartPos.x);
	fprintf(pFile, "%.1f\n", m_vSceneClearStartPos.y);
	fprintf(pFile, "%.1f\n", m_vSceneClearEndPos.x);
	fprintf(pFile, "%.1f\n", m_vSceneClearEndPos.y);
	fprintf(pFile, "%d\n", m_bSceneClearSet ? 1 : 0);

	// 몬스터 스폰 데이터 저장
	fprintf(pFile, "[MonsterSpawn]\n");
	fprintf(pFile, "%zu\n", m_vecMonsterSpawnData.size());
	
	// for (size_t i = 0; i < m_vecMonsterSpawnData.size(); ++i)
	// {
	// 	const MonsterSpawnData monsterData = m_vecMonsterSpawnData[i];
	// 	fprintf(pFile, "%.1f\n", monsterData.position.x);
	// 	fprintf(pFile, "%.1f\n", monsterData.position.y);
	// 	fprintf(pFile, "%d\n", monsterData.monsterType);
	// }

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
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}


// 폴더에서 타일 텍스처 파일들을 불러와서 저장하고 첫 번째 텍스처를 UI에 띄우는 함수
void CScene_Tool::LoadTileTexUI(const wstring& folderPath)
{
    m_vecTile_list.clear(); 
    m_iImgIndex = 0;        

    WIN32_FIND_DATAA  data;

    // 동적 경로 사용
    wstring path = CPathMgr::GetInst()->GetContentPath();
    path += folderPath + L"\\*";
    
    string path2 = string().assign(path.begin(), path.end());
    
    //m_vecTile_list에 텍스처파일들의 이름을 전부 넣기
    try {
        HANDLE hFind = FindFirstFileA(path2.c_str(), &data); // 첫번째 파일 찾아 핸들 리턴
        if (hFind == INVALID_HANDLE_VALUE)
            throw std::runtime_error("FindFirstFile 실패"); // 예외처리 

        while (FindNextFileA(hFind, &data))
        {
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) &&  // 파일이라면
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) // 시스템파일은 제외
            {
                // PNG 파일만 필터링
                std::string fileName = std::string(data.cFileName);
                if (fileName.find(".png") != std::string::npos || fileName.find(".PNG") != std::string::npos)
                {
                    m_vecTile_list.push_back(fileName);
                }
            }
        }
        FindClose(hFind); //핸들 닫아주기 
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << "\n";
        cout << "툴 에러";
    }

    //출력으로 확인하기 
    std::cout << "파일리스트" << "\n";
    for (std::string str : m_vecTile_list)
    {
        std::cout << str << "\n";
    }

    //불러올 타일이 없으면 중지
    if (m_vecTile_list.empty())
    {
        m_pTexUI->SetTexture(nullptr); // 목록이 비었으면 텍스처를 null로 설정
        return;
    }

    path = CPathMgr::GetInst()->GetRelativePath(path.c_str());
    path.pop_back();
    path2 = m_vecTile_list[m_iImgIndex];
    path += wstring().assign(path2.begin(), path2.end());

    CTexture* pTileTexture = CResMgr::GetInst()->LoadTexture(L"TILE0", path.c_str());
    m_pTexUI->SetTexture(pTileTexture);

    ChangeTileTexUI();
}

// 현재 인덱스에 해당하는 텍스처 파일을 UI에 띄우도록 요청한다.
void CScene_Tool::ChangeTileTexUI()
{
    // 현재 모드에 맞는 폴더 경로를 가져오기
    wstring fullPath = CPathMgr::GetInst()->GetContentPath() + m_strCurTexFolder + L"\\";

    // 목록에서 현재 인덱스에 해당하는 파일 이름을 가져오기
    string strFileName = m_vecTile_list[m_iImgIndex];
    wstring wstrFileName(strFileName.begin(), strFileName.end());
    fullPath += wstrFileName;

    // 리소스 매니저에 등록할 고유한 키를 생성
    wstring resKey = m_strCurTexFolder + L"\\" + wstrFileName;

    // 상대 경로로 텍스처를 로드
    wstring relativePath = CPathMgr::GetInst()->GetRelativePath(fullPath.c_str());
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

    m_vPlayerSpawnPos = vWorldPos;
    m_bPlayerSpawnSet = true;

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

    m_vSceneClearStartPos = Vec2(minX, minY);
    m_vSceneClearEndPos = Vec2(maxX, maxY);
    m_bSceneClearSet = true;

    // 디버그 메시지
    wchar_t szBuffer[256];
    swprintf_s(szBuffer, L"씬 클리어 영역 설정: (%.1f, %.1f) ~ (%.1f, %.1f)",
               minX, minY, maxX, maxY);
    MessageBox(nullptr, szBuffer, L"클리어 영역 설정", MB_OK);
}



void CScene_Tool::Render(ID2D1RenderTarget* _pRenderTarget)
{
    CScene::Render(_pRenderTarget);

    if (!_pRenderTarget)
        return;

    // 스폰 위치 표시 (빨간 원)
    if (m_bPlayerSpawnSet)
    {
        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPlayerSpawnPos);

        ID2D1SolidColorBrush* pBrush = nullptr;
        _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.7f), &pBrush);

        if (pBrush)
        {
            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(vRenderPos.x, vRenderPos.y), 20.0f, 20.0f);
            _pRenderTarget->FillEllipse(ellipse, pBrush);

            // 테두리
            _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkRed), &pBrush);
            _pRenderTarget->DrawEllipse(ellipse, pBrush, 2.0f);

            pBrush->Release();
        }
    }

    // 클리어 영역 표시 (파란 사각형)
    if (m_bSceneClearSet)
    {
        Vec2 vRenderStartPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearStartPos);
        Vec2 vRenderEndPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearEndPos);

        ID2D1SolidColorBrush* pBrush = nullptr;
        _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.3f), &pBrush);

        if (pBrush)
        {
            D2D1_RECT_F rect = D2D1::RectF(
                vRenderStartPos.x,
                vRenderStartPos.y,
                vRenderEndPos.x,
                vRenderEndPos.y
            );
            _pRenderTarget->FillRectangle(rect, pBrush);

            // 테두리
            pBrush->Release();
            _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkBlue), &pBrush);
            _pRenderTarget->DrawRectangle(rect, pBrush, 2.0f);

            pBrush->Release();
        }
    }

    // 드래그 중인 클리어 영역 표시
    if (m_bDraggingClearArea)
    {
        Vec2 vRenderStartPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearStartPos);
        Vec2 vRenderEndPos = CCamera::GetInst()->GetRenderPos(m_vSceneClearEndPos);

        ID2D1SolidColorBrush* pBrush = nullptr;
        _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 0.3f), &pBrush);

        if (pBrush)
        {
            D2D1_RECT_F rect = D2D1::RectF(
                vRenderStartPos.x,
                vRenderStartPos.y,
                vRenderEndPos.x,
                vRenderEndPos.y
            );
            _pRenderTarget->FillRectangle(rect, pBrush);

            // 테두리
            pBrush->Release();
            _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &pBrush);
            _pRenderTarget->DrawRectangle(rect, pBrush, 2.0f);

            pBrush->Release();
        }
    }
}

// 몬스터 배치 처리 함수
void CScene_Tool::HandleMonsterPlacement()
{
    // UI 패널 영역이 아닌 곳에서만 몬스터 배치 처리
    if (m_pPanelUI->IsMouseOn())
        return;
    
    // 좌클릭 시 몬스터 배치
    if (KEY_TAP(KEY::LBUTTON))
    {
        // 마우스 위치를 월드 좌표로 변환
        Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
        Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
        Vec2 vResolution = CCore::GetInst()->GetResolution();
        Vec2 vWorldPos = vMousePos + vCamLook - vResolution / 2.f;
        SettingSampleMonster(vWorldPos);
    }
}





void CScene_Tool::SettingSampleMonster(Vec2 pos)
{
    GameObject* obj = new CMonster();
    obj->SetWorldPos(pos);
    obj->CreateAnimator();
    CTexture * pCursor = CResMgr::GetInst()->LoadTexture(L"Cursor_Tex", L"texture\\enemy\\rifleman\\RifleMan.png");
	
	AddObject(obj, GROUP_TYPE::MONSTER);
	
    obj->GetAnimator()->CreateAnimation(L"sample1", pCursor,
        Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, -64.f));
    obj->GetAnimator()->FindAnimation(L"sample1")->Save(L"animation\\sample1.anim");
    obj->GetAnimator()->Play(L"sample1", true);
}


