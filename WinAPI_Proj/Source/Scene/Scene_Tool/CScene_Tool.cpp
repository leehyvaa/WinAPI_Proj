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
	, m_vImgTilePos(Vec2(0, 0))
	, toolMode(TOOL_MODE::TEXTURE_MODE)
	, groundType(GROUND_TYPE::NORMAL)
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
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x,0.f));
	AddObject(pPanelUI, GROUP_TYPE::UI);
	m_pPanelUI = (CPanelUI*)pPanelUI;

	CBtnUI* pBtnTileTex = new CBtnUI;
	pBtnTileTex->SetName(L"ChildUI");
	pBtnTileTex->SetScale(Vec2(320.f, 288.f));
	pBtnTileTex->SetPos(Vec2(0.f, 30.f));
	((CBtnUI*)pBtnTileTex)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetTileUIIdx);
	
	pPanelUI->AddChild(pBtnTileTex);

	m_pTexUI = pBtnTileTex;
	LoadTileTexUI();

	CBtnUI* pBtnPrev = new CBtnUI;
	pBtnPrev->SetName(L"ChildUI");
	pBtnPrev->SetScale(Vec2(32.f, 31.f));
	pBtnPrev->SetPos(Vec2(1.f,288.f));
	//pBtnUI->SetClickedCallBack(&ChangeScene,0,0);
	((CBtnUI*)pBtnPrev)->SetClickedCallBack(this,(SCENE_MEMFUNC) & CScene_Tool::PrevTileUI);
	CTexture* pBtnTexPrev = CResMgr::GetInst()->LoadTexture(L"UI_LEFT", L"Texture\\UI\\UI_LEFT.bmp");
	pBtnPrev->SetTexture(pBtnTexPrev);
	pBtnTileTex->AddChild(pBtnPrev);
	


	CBtnUI* pBtnNext = pBtnPrev->Clone();
	pBtnNext->SetPos(Vec2(283.f, 288.f));
	((CBtnUI*)pBtnNext)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::NextTileUI);
	CTexture* pBtnTexNext = CResMgr::GetInst()->LoadTexture(L"UI_RIGHT", L"Texture\\UI\\UI_RIGHT.bmp");
	pBtnNext->SetTexture(pBtnTexNext);
	pBtnTileTex->AddChild(pBtnNext);


	CBtnUI* pBtnSave = pBtnPrev->Clone();
	pBtnSave->SetPos(Vec2(140.f, 288.f));
	((CBtnUI*)pBtnSave)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	//CTexture* pBtnTexNext = CResMgr::GetInst()->LoadTexture(L"UI_RIGHT", L"Tool\\Button\\UI_RIGHT.bmp");
	//pBtnNext->SetTexture(pBtnTexNext);
	pBtnTileTex->AddChild(pBtnSave);


	//UI 클론 하나 추가
	/*CUI* pClonepPanel = pPanelUI->Clone();
	pClonepPanel->SetPos(pClonepPanel->GetPos() + Vec2(-100.f, 0.f));
	((CBtnUI*)pClonepPanel->GetChildUI()[0])->SetClickedCallBack(&ChangeScene, 0, 0);

	AddObject(pClonepPanel, GROUP_TYPE::UI);
	

	m_pUI = pClonepPanel;*/



    // 모드 텍스트 박스 생성
    m_pModeText = new CTextUI();
    m_pModeText->SetPos(Vec2(900, 0));
    m_pModeText->SetAlign(CTextUI::TEXT_ALIGN::CENTER);
    m_pModeText->SetLineSpace(5);
    m_pModeText->SetVisibleBox(false);
    m_pModeText->SetFontSize(20);
    m_pModeText->SetFontColor(RGB(0,0,255));
    AddObject(m_pModeText, GROUP_TYPE::UI);


    // 설명서 텍스트 박스 생성
    m_pHelpText = new CTextUI();
    m_pHelpText->SetPos(Vec2(0, 0));
    m_pHelpText->SetAlign(CTextUI::TEXT_ALIGN::LEFT);
    m_pHelpText->SetLineSpace(5);
    m_pHelpText->SetVisibleBox(false);
    m_pHelpText->SetFontColor(RGB(137,0,255));

    m_pHelpText->SetFontSize(20);
    



    // 기존 초기화 코드...

    // 모드별 설명 텍스트 초기화
    m_textureHelp = {
        L"[텍스처 모드]",
        L"좌클릭 - 타일 배치",
        L"우클릭 - 타일 복사", 
        L"1 - 배경 레이어",
        L"2 - 전경 레이어",
        L"BACK - 지우기",
    };

    m_groundHelp = {
        L"[지형 모드]",
        L"좌클릭 - 지형 콜라이더 생성",
        L"1 - 기본 지형",
        L"2 - 벽 지형",
        L"3 - 데미지 지형",
        L"4 - 즉사 지형",
        L"BACK - 지우기"
    };

    m_commonHelp = {
        L"[조작법]",
        L"F1 - 텍스처 모드",
        L"F2 - 지형 모드",
        L"F3 - 트리거 모드",
        L"F4 - 프리팹 모드",
        L"F5 - 타일 테두리 표시",
        L"F6 - 콜라이더 표시",
        L"F7 - 그라운드 타입 표시",
        L"CTRL - 타일맵 불러오기",
        L"ESC - 시작 화면으로"
    };



    AddObject(m_pHelpText, GROUP_TYPE::UI);

	/*CBackGround* backGround2 = new CBackGround;
	backGround2->SetPos(Vec2(0, 0));
	backGround2->SetTexture(back2);
	backGround2->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround2, GROUP_TYPE::BACKGROUND);*/
	

	CBackGround* backGround = new CBackGround;
	backGround->SetPos(Vec2(0, 0));
	CTexture* back = CResMgr::GetInst()->LoadTexture(L"TutorialBack", L"texture\\background\\Forest_Mountain2.bmp");
	
	backGround->SetTexture(back);
	backGround->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround, GROUP_TYPE::BACKGROUND);

	SetBackGround(backGround);
	

	//카메라 Look 지정
	CCamera::GetInst()->SetLookAt(vResolution/2.f);
}

void CScene_Tool::Exit()
{
	CCore::GetInst()->DivideMenu();

	DeleteAll();
}

void CScene_Tool::Update()
{
	CScene::Update();
    m_pModeText->ClearLines();
    m_pHelpText->ClearLines();

    // 공통 설명 표시
    m_pHelpText->AddLines(m_commonHelp);
    m_pHelpText->AddLine(L"");  // 빈 줄 추가

    // 현재 모드의 설명만 표시
    switch(toolMode)
    {
    case TEXTURE_MODE:
        m_pHelpText->AddLines(m_textureHelp);
        break;
            
    case GROUND_MODE:
        m_pHelpText->AddLines(m_groundHelp);
        break;
    }

	if (KEY_TAP(KEY::ESC))
		ChangeScene(SCENE_TYPE::START);

    static wstring mode;
    static wstring subMode = L"None";
    static wstring write = L"writing";

	switch (toolMode)
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
            groundType = GROUND_TYPE::NORMAL;
        }
        if (KEY_TAP(KEY::KEY_2))
        {
            subMode = L"UNWALKABLE";
            groundType = GROUND_TYPE::UNWALKABLE;
        }
        if (KEY_TAP(KEY::KEY_3))
        {
            subMode = L"DAMAGEZONE";
            groundType = GROUND_TYPE::DAMAGEZONE;
        }
        if (KEY_TAP(KEY::KEY_4))
        {
            subMode = L"DEADZONE";
            groundType = GROUND_TYPE::DEADZONE;
        }
	        
        if (KEY_HOLD(KEY::LBUTTON))
        {
            CreateGround();
        }


        // if (KEY_TAP(KEY::BACK))
        // {
        //     if (GetGroundCount() > 0)
        //     {
        //         DeleteObject(CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::GROUND).back());
        //         SetGroundCount(GetGroundCount() - 1);
        //     }
        // }

        // if (KEY_TAP(KEY::P))
        // {
        //     cout << "ground Count: " << GetGroundCount() << endl;
        //     cout << "ground Type:" << (int)groundType << endl;
        //
        // }
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
	{
		toolMode = TOOL_MODE::TEXTURE_MODE;
	}
	if (KEY_TAP(KEY::F2))
	{
		toolMode = TOOL_MODE::GROUND_MODE;
	}
	if (KEY_TAP(KEY::F3))
	{
		toolMode = TOOL_MODE::TRIGGER_MODE;
	}
	if (KEY_TAP(KEY::F4))
	{
		toolMode = TOOL_MODE::PREFAB_MODE;
    }

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
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		if (vMousePos.x < 0.f || iTileX <= iCol
			|| vMousePos.y < 0.f || iTileX <= iRow)
			return;

		if (m_iImgTileIdx < 0)
			return;

		UINT iIdx = iRow * iTileX + iCol;


		Vec2 tilePos = Vec2((float)iCol, (float)iRow);

		if (tilePos == m_vTilePos)
			return;
		else
		{
			Vec2 diff = tilePos - m_vTilePos;
			diff += m_vImgTilePos;


			CTexture* tex = m_pTexUI->GetTexture();
			m_iImgTileX = (int)tex->Width() / TILE_SIZE;
			m_iImgTileY = (int)tex->Height() / TILE_SIZE;


			if (diff.x < 0.f || diff.y < 0.f ||
				diff.x >= m_iImgTileX || diff.y >= m_iImgTileY)
				return;

			int newTileIdx = (int)diff.y * m_iImgTileX + (int)diff.x;


			const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

			if (m_bSecondTex)
			{
				((CTile*)vecTile[iIdx])->SetTextureTwo(m_pTexUI->GetTexture());
				((CTile*)vecTile[iIdx])->SetImgIdxTwo(newTileIdx);
			}
			else
			{
				((CTile*)vecTile[iIdx])->SetTexture(m_pTexUI->GetTexture());
				((CTile*)vecTile[iIdx])->SetImgIdx(newTileIdx);
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

	if (KEY_TAP(KEY::RBUTTON))
	{
		//DrawSelectTile();
	}
}





// 마우스 위치의 타일을 계산하고 해당 타일의 텍스처 변경 함수를 실행하는 함수
void CScene_Tool::DrawSelectTile()
{
	Vec2 vMousePos = MOUSE_POS;
	vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

	int iTileX = (int)GetTileX();
	int iTileY = (int)GetTileY();

	int iCol = (int)vMousePos.x / TILE_SIZE;
	int iRow = (int)vMousePos.y / TILE_SIZE;

	if (vMousePos.x < 0.f || iTileX <= iCol
		|| vMousePos.y < 0.f || iTileX <= iRow)
		return;

	if (m_iImgTileIdx < 0)
		return;
    
    // 선택된 타일의 인덱스 계산
	UINT iIdx = iRow * iTileX + iCol;
	
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);



	
	

    
	if (!m_bErase)
	{
		if (m_bSecondTex)
		{
			((CTile*)vecTile[iIdx])->SetTextureTwo(m_pTexUI->GetTexture());
			((CTile*)vecTile[iIdx])->SetImgIdxTwo(m_iImgTileIdx);
		}
		else
		{
			((CTile*)vecTile[iIdx])->SetTexture(m_pTexUI->GetTexture());
			((CTile*)vecTile[iIdx])->SetImgIdx(m_iImgTileIdx);
		}

	}
	else
	{
		if (m_bSecondTex)
		{
			((CTile*)vecTile[iIdx])->SetTextureTwo(nullptr);
			((CTile*)vecTile[iIdx])->SetImgIdxTwo(-1);
		}
		else
		{
			((CTile*)vecTile[iIdx])->SetTexture(nullptr);
			((CTile*)vecTile[iIdx])->SetImgIdx(-1);
		}
	}

	

	m_vTilePos = Vec2((float)iCol, (float)iRow);
}

void CScene_Tool::CreateGround()
{
    Vec2 vMousePos = MOUSE_POS;
    vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

    int iTileX = (int)GetTileX();
    int iTileY = (int)GetTileY();

    int iCol = (int)vMousePos.x / TILE_SIZE;
    int iRow = (int)vMousePos.y / TILE_SIZE;

    if (vMousePos.x < 0.f || iTileX <= iCol
        || vMousePos.y < 0.f || iTileX <= iRow)
        return;

    if (m_iImgTileIdx < 0)
        return;
    
    // 선택된 타일의 인덱스 계산
    UINT iIdx = iRow * iTileX + iCol;
	
    const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

    CTile* selectedTile = (CTile*)vecTile[iIdx];
    
    if (m_bErase)
    {
        selectedTile->SetCollideType(TILE_COLLIDE_TYPE::NONE);
    }
    else
    {
        selectedTile->SetCollideType(TILE_COLLIDE_TYPE::SOLID);

        if (groundType == GROUND_TYPE::NORMAL)
            selectedTile->SetGroundType(GROUND_TYPE::NORMAL);
        if (groundType == GROUND_TYPE::UNWALKABLE)
            selectedTile->SetGroundType(GROUND_TYPE::UNWALKABLE);
        if (groundType == GROUND_TYPE::DAMAGEZONE)
            selectedTile->SetGroundType(GROUND_TYPE::DAMAGEZONE);
        if (groundType == GROUND_TYPE::DEADZONE)
            selectedTile->SetGroundType(GROUND_TYPE::DEADZONE);
    }
    
    

	

    m_vTilePos = Vec2((float)iCol, (float)iRow);
    
    // static Vec2 mousePos1(0, 0);
    // static Vec2 mousePos2(0, 0);
    //
    //
    // if (KEY_TAP(KEY::LBUTTON))
    // {
    // 	mousePos1 = MOUSE_POS;
    // 	mousePos1 = CCamera::GetInst()->GetRealPos(mousePos1);
    // 	cout << mousePos1.x <<endl;
    // }
    // if (KEY_AWAY(KEY::LBUTTON))
    // {
    // 	mousePos2 = MOUSE_POS;
    // 	mousePos2 =CCamera::GetInst()->GetRealPos(mousePos2);
    // 	cout << mousePos2.x <<endl;
    //
    //
    // 	if (groundType == GROUND_TYPE::GROUND)
    // 	{
    // 		CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::GROUND,
    // 			mousePos1, mousePos2);
    // 		//AddObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
    // 		CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
    // 	}
    // 	else if (groundType == GROUND_TYPE::NONGROUND)
    // 	{
    // 		CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::NONGROUND,
    // 			mousePos1, mousePos2);
    // 		CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
    // 	}
    // 	else if (groundType == GROUND_TYPE::DAMAGEZONE)
    // 	{
    // 		CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::DAMAGEZONE,
    // 			mousePos1, mousePos2);
    // 		CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
    // 	}
    // 	else if (groundType == GROUND_TYPE::DEADZONE)
    // 	{
    // 		CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::DEADZONE,
    // 			mousePos1, mousePos2);
    // 		CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
    // 	}
    //
    //
    // 	SetGroundCount(GetGroundCount() + 1);
    // }

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

		m_iImgTileX = (int)tex->Width()/ TILE_SIZE;
		m_iImgTileY = (int)tex->Height()/ TILE_SIZE;

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		if (vMousePos.x < 0.f || m_iImgTileX <= iCol
			|| vMousePos.y < 0.f || m_iImgTileX <= iRow)
			return;

        // 원본텍스처에서 내가 클릭한 부분의 idx
		m_iImgTileIdx = iRow * m_iImgTileX + iCol;

		m_vImgTilePos = Vec2(iCol, iRow);
	}
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{

	FILE* pFile = nullptr;
	_wfopen_s(&pFile,_strFilePath.c_str(),L"wb");
	assert(pFile);

	//타일 가로세로 개수 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fprintf(pFile, "[TileCount]\n");
	fprintf(pFile, "%d\n", (int)xCount);
	fprintf(pFile, "%d\n", (int)yCount);

	fprintf(pFile,"\n");

	//모든 타일들을 개별적으로 저장할 데이터를 저장하게 함
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	const vector<GameObject*>& vecGround = GetGroupObject(GROUP_TYPE::GROUND);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}

	fprintf(pFile, "[GroundCount]\n");
	fprintf(pFile, "%d\n", (int)GetGroundCount());
	for (size_t i = 0; i < vecGround.size(); i++)
	{
		((CGround*)vecGround[i])->Save(pFile);
	}



	fclose(pFile);
}

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


// 파일입력 기능으로 타일 정보가 저장된 메모장 파일을 불러오는 함수, 그 후에 LoadTile함수로 타일 정보를 각각 가져온다.
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
				m_vecTile_list.push_back(std::string(data.cFileName));
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
		m_iImgIndex = (UINT)m_vecTile_list.size() - 1;

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
	GetDIBits(hdcScreen, hBitmap, 0, (UINT)screenY, lpBits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	WriteFile(hFile, lpBits, dwSizeofDIB, &dwWritten, NULL);


	delete[] lpBits;
	CloseHandle(hFile);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
 }

void CScene_Tool::UpdateTextBox()
{
    m_pModeText->ClearLines();

 

    
    


    // 라인 직접 추가
    //pChatBox->AddLine(L"[시스템] 환영합니다!");
    //pChatBox->AddLine(L"> 플레이어가 입장했습니다");

    //// 2. 개행 문자로 분할
    //pChatBox->SetText(L"첫번째 줄\n두번째 줄\n세번째 줄");

    // 3. 벡터로 일괄 추가
    /*vector<wstring> tutorialText = {
        mode,
        L"WASD - 이동",
        L"Space - 점프"
    };
    m_pModeText->AddLines(tutorialText);*/
}







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