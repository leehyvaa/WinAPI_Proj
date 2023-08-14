#include "pch.h"
#include "CScene.h"
#include "GameObject.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CKeyMgr.h"

CScene::CScene()
	:m_iTileX(0)
	,m_iTileY(0)
	,m_pPlayer(nullptr)
	,bDrawGrid(false)
	,bDrawCollider(false)
	,bDrawGroundType(false)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (UINT j = 0; j < m_arrObj[i].size(); j++)
		{
			//m_arrObj[i] 그룹 벡터의 j 물체 삭제
			delete m_arrObj[i][j];
		}
	}
}

void CScene::Start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			m_arrObj[i][j]->Start();
		}
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			if(!m_arrObj[i][j]->IsDead())
				m_arrObj[i][j]->Update();
		}
	}

	if (KEY_TAP(KEY::F5))
	{
		bDrawGrid= !bDrawGrid;
	}
	if (KEY_TAP(KEY::F6))
	{
		bDrawCollider = !bDrawCollider;
	}
	if (KEY_TAP(KEY::F7))
	{
		bDrawGroundType = !bDrawGroundType;
	}

}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			m_arrObj[i][j]->FinalUpdate();
		}
	}
}

void CScene::Render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			Render_Tile(_dc);
			continue;
		}


		vector<GameObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(_dc);
				++iter;
			}
			else
			{
				iter =m_arrObj[i].erase(iter);
			}
		}
	}


}

void CScene::Render_Tile(HDC _dc)
{
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;
	
	int iTileSize = TILE_SIZE;

	int iLTCol = (int)vLeftTop.x / iTileSize;
	int iLTRow = (int)vLeftTop.y / iTileSize;





	int iLTIdx = m_iTileX * iLTRow + iLTCol;

	int iClientWidth = ((int)vResolution.x / iTileSize) +2;
	int iClientHeight = ((int)vResolution.y / iTileSize)+2;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow +iClientHeight); iCurRow++)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); iCurCol++)
		{
			if (iCurCol < 0 || m_iTileX <= (UINT)iCurCol ||
				iCurRow < 0 || m_iTileY <= (UINT)iCurRow)
			{
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			if(bDrawGrid)
			{
				Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vecTile[iIdx]->GetPos());
				Vec2 vScale = vecTile[iIdx]->GetScale();

				SelectGDI brush(CCore::GetInst()->GetMainDC(), BRUSH_TYPE::HOLLOW);

				Rectangle(_dc, (int)(vRenderPos.x)
					, (int)(vRenderPos.y)
					, (int)(vRenderPos.x + vScale.x)
					, (int)(vRenderPos.y + vScale.y));
			}


			vecTile[iIdx]->Render(_dc);

			
		}
	}

}


//지정된 그룹의 벡터를 지우는 함수
void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec<GameObject*>(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);


	m_iTileX = _iXCount;
	m_iTileY = _iYCount;


	//CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\Prologue_Tileset32.bmp");


	for (UINT i = 0; i < _iYCount; i++)
	{
		for (UINT j = 0; j < _iXCount; j++)
		{
			CTile* pTile = new CTile();

			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			//pTile->SetTexture(pTileTex);

			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}


}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	//커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	//타일 가로 세로 개수 불러오기
	UINT xCount =0;
	UINT yCount =0;

	//fread(&xCount, sizeof(UINT), 1, pFile);
	//fread(&yCount, sizeof(UINT), 1, pFile);
	char szBuff[256] = {};

	FScanf(szBuff, pFile);
	fscanf_s(pFile, "%d", &xCount);
	fscanf_s(pFile, "%d", &yCount);
	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);


	//불러온 개수에 맞게 EmptyTile 들 만들어두기
	CreateTile(xCount, yCount);

	//만들어진 타일 개별로필요한 정보를 불러옴
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}


	fclose(pFile);

}


