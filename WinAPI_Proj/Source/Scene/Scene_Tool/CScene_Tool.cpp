#include "pch.h"
#include "CScene_Tool.h"

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
	LoadTileTexUI();

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
    m_pModeText->SetFontColor(RGB(0,0,255));
    AddObject(m_pModeText, GROUP_TYPE::UI);


    // 설명서 텍스트 박스 생성
    m_pHelpText = new CTextUI();
    m_pHelpText->SetWorldPos(Vec2(0, 0));
    m_pHelpText->SetAlign(CTextUI::TEXT_ALIGN::LEFT);
    m_pHelpText->SetLineSpace(5);
    m_pHelpText->SetVisibleBox(false);
    m_pHelpText->SetFontColor(RGB(137,0,255));
    m_pHelpText->SetFontSize(20);
    AddObject(m_pHelpText, GROUP_TYPE::UI);


    m_pHelpSubText = new CTextUI();
    m_pHelpSubText->SetWorldPos(Vec2(400, 0));
    m_pHelpSubText->SetAlign(CTextUI::TEXT_ALIGN::LEFT);
    m_pHelpSubText->SetLineSpace(5);
    m_pHelpSubText->SetVisibleBox(false);
    m_pHelpSubText->SetFontColor(RGB(137,0,255));
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

    m_commonHelp = {
        L"[조작법]",
        L"F1 - 텍스처 모드",
        L"F2 - 지형 모드",
        L"F3 - 트리거 모드",
        L"F4 - 프리팹 모드",
        L"",
        L"F5 - 타일 테두리 표시",
        L"F6 - 콜라이더 표시",
        L"F7 - 그라운드 타입 표시",
        L"F8 - 그라운드 완성 라인 표시",
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
	case PREFAB_MODE:
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
		m_eToolMode = TOOL_MODE::TEXTURE_MODE;
	if (KEY_TAP(KEY::F2))
		m_eToolMode = TOOL_MODE::GROUND_MODE;
	if (KEY_TAP(KEY::F3))
		m_eToolMode = TOOL_MODE::TRIGGER_MODE;
	if (KEY_TAP(KEY::F4))
		m_eToolMode = TOOL_MODE::PREFAB_MODE;

    vector<wstring> modeText =
        {
	    mode,
	    subMode,
	    write,
	    };

   m_pModeText->AddLines(modeText);
}

// 클릭 시 현재 마우스 위치를 계산하여 해당 타일에 지정된 텍스처를 입히도록 요청하는 함수
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





// 마우스 위치의 타일을 계산하고 해당 타일의 텍스처 변경 함수를 실행하는 함수
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
void CScene_Tool::LoadTileTexUI()
{
	WIN32_FIND_DATAA  data;


	wstring path = CPathMgr::GetInst()->GetContentPath();
	path += L"texture\\tile\\*";



	string path2 = string().assign(path.begin(), path.end());
    
    //m_vecTile_list에 텍스처파일들의 이름을 전부 넣기
	try {
		HANDLE hFind = FindFirstFileA(path2.c_str(), &data); //첫번째 파일 찾아 핸들 리턴
		if (hFind == INVALID_HANDLE_VALUE)
			throw std::runtime_error("FindFirstFile 실패"); //예외처리 

		while (FindNextFileA(hFind, &data))
		{

			if ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) &&  //파일이라면
				!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) //시스템파일은 제외
			{
				// PNG 파일만 필터링 (BMP에서 PNG로 마이그레이션)
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


	path = CPathMgr::GetInst()->GetRelativePath(path.c_str());
	path.pop_back();
	path2 = m_vecTile_list[m_iImgIndex];
	path += wstring().assign(path2.begin(), path2.end());

	//불러올 타일이 없으면 중지
	if (m_vecTile_list.size() == 0)
		assert(nullptr);

	CTexture* pTileTexture = CResMgr::GetInst()->LoadTexture(L"TILE0", path.c_str());
	m_pTexUI->SetTexture(pTileTexture);


}

// 현재 인덱스에 해당하는 텍스처 파일을 UI에 띄우도록 요청하는 함수
void CScene_Tool::ChangeTileTexUI()
{
	wstring path = CPathMgr::GetInst()->GetContentPath();
	path += L"texture\\tile\\*";


	path = CPathMgr::GetInst()->GetRelativePath(path.c_str());
	path.pop_back();
	string path2 = m_vecTile_list[m_iImgIndex];
	path += wstring().assign(path2.begin(), path2.end());

	wstring fileName = L"TILE";
	fileName += to_wstring(m_iImgIndex);

	CTexture* pTileTexture = CResMgr::GetInst()->LoadTexture(fileName.c_str(), path.c_str());
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



void CScene_Tool::SaveBmp()
{
	HDC hdcScreen = CCore::GetInst()->GetMainDC();


	int screenX =TILE_SIZE*GetTileX();
	int screenY = TILE_SIZE * GetTileY();


	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenX, screenY);
	SelectObject(hdcMem, hBitmap);

	//화면 캡처
	BitBlt(hdcMem, 0, 0, screenX, screenY, hdcScreen, 0, 0, SRCCOPY);

	//비트맵 저장
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = screenX;
	bi.biHeight = screenY;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	HANDLE hFile = CreateFile(L"content\\texture\\map\\screenshot.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwWritten = 0;
	DWORD dwSizeofDIB = screenX * screenY * 3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	BITMAPFILEHEADER bmfHeader;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfHeader.bfSize = dwSizeofDIB + sizeof(BITMAPFILEHEADER);
	bmfHeader.bfType = 0x4D42;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	LPSTR lpBits = new char[dwSizeofDIB];
	GetDIBits(hdcScreen, hBitmap, 0, static_cast<UINT>(screenY), lpBits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	WriteFile(hFile, lpBits, dwSizeofDIB, &dwWritten, NULL);


	delete[] lpBits;
	CloseHandle(hFile);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
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
