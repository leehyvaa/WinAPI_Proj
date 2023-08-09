#include "pch.h"
#include "CScene.h"
#include "GameObject.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CCamera.h"
#include "CCore.h"

CScene::CScene()
	:m_iTileX(0)
	,m_iTileY(0)
	,m_pPlayer(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (UINT j = 0; j < m_arrObj[i].size(); j++)
		{
			//m_arrObj[i] �׷� ������ j ��ü ����
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
			if (iCurCol < 0 || m_iTileX <= iCurCol ||
				iCurRow < 0 || m_iTileY <= iCurRow)
			{
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;
			vecTile[iIdx]->Render(_dc);
		}
	}

}


//������ �׷��� ���͸� ����� �Լ�
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


	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\Prologue_Tileset32.bmp");


	for (UINT i = 0; i < _iYCount; i++)
	{
		for (UINT j = 0; j < _iXCount; j++)
		{
			CTile* pTile = new CTile();

			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			pTile->SetTexture(pTileTex);

			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}


}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	//Ŀ�� ������Ʈ
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	//Ÿ�� ���� ���� ���� �ҷ�����
	UINT xCount =0;
	UINT yCount =0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	//�ҷ��� ������ �°� EmptyTile �� �����α�
	CreateTile(xCount, yCount);

	//������� Ÿ�� �������ʿ��� ������ �ҷ���
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}


	fclose(pFile);

}
