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
#include "CGround.h"
#include "CBackGround.h"

#include "resource.h"


void ChangeScene(DWORD_PTR, DWORD_PTR);

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
	, groundType(GROUND_TYPE::GROUND)
	, m_bErase(false)
	, m_bSecondTex(false)
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
	pBtnTileTex->SetScale(Vec2(320.f, 320.f));
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



	



	CBackGround* backGround2 = new CBackGround;
	backGround2->SetPos(Vec2(0, 0));
	CTexture* back2 = CResMgr::GetInst()->LoadTexture(L"TutorialBackSky", L"texture\\background\\Forest_Sky.bmp");
	backGround2->SetTexture(back2);
	backGround2->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround2, GROUP_TYPE::BACKGROUND);
	

	CBackGround* backGround = new CBackGround;
	backGround->SetPos(Vec2(0, 0));
	CTexture* back = CResMgr::GetInst()->LoadTexture(L"TutorialBack", L"texture\\background\\Forest_Mountain.bmp");
	backGround->SetTexture(back);
	backGround->SetScale(CCore::GetInst()->GetResolution());
	AddObject((GameObject*)backGround, GROUP_TYPE::BACKGROUND);

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

		if(!m_pPanelUI->IsMouseOn())
			SetTileIdx();


		if (KEY_TAP(KEY::BACK))
		{
			m_bErase = !m_bErase;
		}
		if (KEY_TAP(KEY::KEY_1))
		{
			m_bSecondTex = false;
			m_bErase = false;
		}
		if (KEY_TAP(KEY::KEY_2))
		{
			m_bSecondTex = true;
			m_bErase = false;

		}

	}
	break;
	case GROUND_MODE:
		CreateGround();
		if (KEY_TAP(KEY::BACK))
		{
			if (GetGroundCount() > 0)
			{
				DeleteObject(CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::GROUND).back());
				SetGroundCount(GetGroundCount() - 1);
			}
		}

		if (KEY_TAP(KEY::P))
		{
			cout <<"ground Count: " << GetGroundCount() << endl;
			cout << "ground Type:" << (int)groundType<<endl;

		}
		if (KEY_TAP(KEY::KEY_2))
		{
			int a = (int)groundType;
			groundType= (GROUND_TYPE)++a;
			if (groundType == GROUND_TYPE::END)
				groundType = GROUND_TYPE::GROUND;
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
	m_pTexUI->SetTexture(pTileTexture);


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
	m_pTexUI->SetTexture(pTileTexture);
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


		if (groundType == GROUND_TYPE::GROUND)
		{
			CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::GROUND,
				mousePos1, mousePos2);
			//AddObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
			CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
		}
		else if (groundType == GROUND_TYPE::NONGROUND)
		{
			CGround* pGround2 = CGroundPrefab::CreateGround(GROUND_TYPE::NONGROUND,
				mousePos1, mousePos2);
			//AddObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
			CreateObject((GameObject*)pGround2, GROUP_TYPE::GROUND);
		}



		SetGroundCount(GetGroundCount() + 1);
	}

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