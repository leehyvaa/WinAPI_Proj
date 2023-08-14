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

#include "resource.h"


void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
	, m_iImgIndex(0)
	, m_vTilePos(Vec2(0,0))
	, m_iImgTileX(-1)
	, m_iImgTileY(-1)
	, m_iImgTileIdx(-1)
	, m_vImgTilePos(Vec2(0,0))
	, toolMode(ToolMode::TEXTURE_MODE)
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


	CBtnUI* pBtnTileTex = new CBtnUI;
	pBtnTileTex->SetName(L"ChildUI");
	pBtnTileTex->SetScale(Vec2(320.f, 320.f));
	pBtnTileTex->SetPos(Vec2(0.f, 30.f));
	((CBtnUI*)pBtnTileTex)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetTileUIIdx);
	
	pPanelUI->AddChild(pBtnTileTex);

	m_pUI = pBtnTileTex;
	LoadTileTexUI();

	CBtnUI* pBtnPrev = new CBtnUI;
	pBtnPrev->SetName(L"ChildUI");
	pBtnPrev->SetScale(Vec2(32.f, 31.f));
	pBtnPrev->SetPos(Vec2(1.f,288.f));
	//pBtnUI->SetClickedCallBack(&ChangeScene,0,0);
	((CBtnUI*)pBtnPrev)->SetClickedCallBack(this,(SCENE_MEMFUNC) & CScene_Tool::PrevTileUI);
	CTexture* pBtnTexPrev = CResMgr::GetInst()->LoadTexture(L"UI_LEFT", L"Tool\\Button\\UI_LEFT.bmp");
	pBtnPrev->SetTexture(pBtnTexPrev);
	pBtnTileTex->AddChild(pBtnPrev);
	


	CBtnUI* pBtnNext = pBtnPrev->Clone();
	pBtnNext->SetPos(Vec2(283.f, 288.f));
	((CBtnUI*)pBtnNext)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::NextTileUI);
	CTexture* pBtnTexNext = CResMgr::GetInst()->LoadTexture(L"UI_RIGHT", L"Tool\\Button\\UI_RIGHT.bmp");
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

	if (KEY_TAP(KEY::T))
		ChangeScene(SCENE_TYPE::START);


	switch (toolMode)
	{
	case TEXTURE_MODE:
	{
		SetTileUIIdx();
		SetTileIdx();
	}
	break;
	case GROUND_MODE:
		CreateGround();


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


	if (KEY_TAP(KEY::F1))
	{
		toolMode = ToolMode::TEXTURE_MODE;
	}
	if (KEY_TAP(KEY::F2))
	{
		toolMode = ToolMode::GROUND_MODE;
	}
	if (KEY_TAP(KEY::F3))
	{
		toolMode = ToolMode::TRIGGER_MODE;
	}
	if (KEY_TAP(KEY::F4))
	{
		toolMode = ToolMode::PREFAB_MODE;
	}

	

	
}

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


			CTexture* tex = m_pUI->GetTexture();
			m_iImgTileX = (int)tex->Width() / TILE_SIZE;
			m_iImgTileY = (int)tex->Height() / TILE_SIZE;


			if (diff.x < 0.f || diff.y < 0.f ||
				diff.x >= m_iImgTileX || diff.y >= m_iImgTileY)
				return;

			int newTileIdx = (int)diff.y * m_iImgTileX + (int)diff.x;


			const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
			((CTile*)vecTile[iIdx])->SetTexture(m_pUI->GetTexture());
			((CTile*)vecTile[iIdx])->SetImgIdx(newTileIdx);
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

	UINT iIdx = iRow * iTileX + iCol;

	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	((CTile*)vecTile[iIdx])->SetTexture(m_pUI->GetTexture());
	((CTile*)vecTile[iIdx])->SetImgIdx(m_iImgTileIdx);

	m_vTilePos = Vec2((float)iCol, (float)iRow);
}


void CScene_Tool::SetTileUIIdx()
{
	if (m_pUI->GetTexture() && m_pUI->IsLbtnDown())
	{
		CTexture* tex = m_pUI->GetTexture();
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
		vMousePos =vMousePos- m_pUI->GetFinalPos();
		vMousePos = CCamera::GetInst()->GetRenderPos(vMousePos);

		m_iImgTileX = (int)tex->Width()/ TILE_SIZE;
		m_iImgTileY = (int)tex->Height()/ TILE_SIZE;

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		if (vMousePos.x < 0.f || m_iImgTileX <= iCol
			|| vMousePos.y < 0.f || m_iImgTileX <= iRow)
			return;


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

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Save(pFile);
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

void CScene_Tool::LoadTileTexUI()
{
	WIN32_FIND_DATAA  data;


	wstring path = CPathMgr::GetInst()->GetContentPath();
	path += L"texture\\tile\\*";



	string path2 = string().assign(path.begin(), path.end());

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
		std::cerr << e.what() << std::endl;
		cout << "툴 에러";
	}

	//출력으로 확인하기 
	std::cout << "파일리스트" << std::endl;
	for (std::string str : m_vecTile_list)
	{
		std::cout << str << std::endl;
	}


	path = CPathMgr::GetInst()->GetRelativePath(path.c_str());
	path.pop_back();
	path2 = m_vecTile_list[m_iImgIndex];
	path += wstring().assign(path2.begin(), path2.end());

	//불러올 타일이 없으면 중지
	if (m_vecTile_list.size() == 0)
		assert(nullptr);

	CTexture* pTileTexture = CResMgr::GetInst()->LoadTexture(L"TILE0", path.c_str());
	m_pUI->SetTexture(pTileTexture);


}

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
	m_pUI->SetTexture(pTileTexture);
}

void CScene_Tool::PrevTileUI()
{
	m_iImgIndex--;
	if (0 > m_iImgIndex)
		m_iImgIndex = (UINT)m_vecTile_list.size() - 1;
	ChangeTileTexUI();
}

void CScene_Tool::CreateGround()
{
	static Vec2 mousePos1(0, 0);
	static Vec2 mousePos2(0, 0);


	if (KEY_TAP(KEY::LBUTTON))
	{
		mousePos1 = MOUSE_POS;
		mousePos1 = CCamera::GetInst()->GetRealPos(mousePos1);
		cout << mousePos1.x <<endl;
	}
	if (KEY_AWAY(KEY::LBUTTON))
	{
		mousePos2 = MOUSE_POS;
		mousePos2 =CCamera::GetInst()->GetRealPos(mousePos2);
		cout << mousePos2.x <<endl;



		CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::GROUND,
			mousePos1, mousePos2);
		//AddObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
		CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);

	}

}




void CScene_Tool::NextTileUI()
{
	m_iImgIndex++;
	if (m_vecTile_list.size() <= m_iImgIndex)
		m_iImgIndex = 0;
	ChangeTileTexUI();

}


void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
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
		if (LOWORD(wParam) == IDOK )
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
