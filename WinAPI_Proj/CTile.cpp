#include "pch.h"
#include "CTile.h"
#include "CTexture.h"
#include "SelectGDI.h"
#include "CCore.h"
#include "CResMgr.h"

CTile::CTile()
	:m_pTileTex(nullptr)
	, m_iImgIdx(0)
{
	SetScale(Vec2(TILE_SIZE, TILE_SIZE));
}

CTile::~CTile()
{
}

void CTile::Update()
{
}

void CTile::Render(HDC _dc)
{


	if (nullptr == m_pTileTex || -1 == m_iImgIdx)
		return;

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();


	UINT iWidth = m_pTileTex->Width();
	UINT iHeight = m_pTileTex->Height();

	UINT iMaxCol = iWidth / TILE_SIZE;
	UINT iMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// �̹��� ������ ��� �ε��� üũ
	if (iMaxRow <= iCurRow)
	{
		return;
		assert(nullptr);
	}



	BitBlt(_dc
		, (int)(vRenderPos.x)
		, (int)(vRenderPos.y)
		, (int)vScale.x
		, (int)vScale.y
		, m_pTileTex->GetDC()
		, iCurCol*TILE_SIZE
		, iCurRow * TILE_SIZE
		,SRCCOPY);




}

void CTile::Save(FILE* _pFile)
{
	//fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);

	fprintf(_pFile, "[Tile]\n");
	fprintf(_pFile, "%d\n", m_iImgIdx);

	if (m_pTileTex)
	{
		fprintf(_pFile, "[Texture_Name]\n");
		string strName = string(m_pTileTex->GetKey().begin(), m_pTileTex->GetKey().end());
		fprintf(_pFile, strName.c_str());
		fprintf(_pFile, "\n");

		fprintf(_pFile, "[Texture_Path]\n");
		strName = string(m_pTileTex->GetRelativePath().begin(), m_pTileTex->GetRelativePath().end());
		fprintf(_pFile, strName.c_str());
		fprintf(_pFile, "\n");
	}
	else
	{
		fprintf(_pFile, "[Texture_Name]\n");
		fprintf(_pFile, "-1\n");
		fprintf(_pFile, "[Texture_Path]\n");
		fprintf(_pFile, "-1\n");
	}
	fprintf(_pFile, "\n");
}

void CTile::Load(FILE* _pFile)
{
	char szBuff[256] = {};
	string str;

	FScanf(szBuff, _pFile);//[Tile]
	fscanf_s(_pFile, "%d", &m_iImgIdx);
	FScanf(szBuff, _pFile);


	FScanf(szBuff, _pFile);//[Texture_Name]
	FScanf(szBuff, _pFile);

	if (strcmp(szBuff, "-1"))
	{
		str = szBuff;
		wstring strTexKey = wstring(str.begin(), str.end());

		FScanf(szBuff, _pFile);//[Texture_Path]
		FScanf(szBuff, _pFile);

		str = szBuff;
		wstring strTexPath = wstring(str.begin(), str.end());

		m_pTileTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);
	}
	else
	{
		FScanf(szBuff, _pFile);
		FScanf(szBuff, _pFile);
	}
	FScanf(szBuff, _pFile);
}
