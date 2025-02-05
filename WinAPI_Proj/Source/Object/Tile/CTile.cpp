#include "pch.h"
#include "CTile.h"

#include "CCollider.h"
#include "CTexture.h"
#include "SelectGDI.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

CTile::CTile()
	:m_pTileTex(nullptr)
	,m_pTileTex2(nullptr)
	,m_iImgIdx2(0)
	,m_iImgIdx(0)
    ,m_eGroundType(GROUND_TYPE::END)
    ,m_iBotRightTileIdx(-1)
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

	if (nullptr != m_pTileTex && -1 != m_iImgIdx)
	{
		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
		Vec2 vScale = GetScale();


		UINT iWidth = m_pTileTex->Width();
		UINT iHeight = m_pTileTex->Height();

		UINT iMaxCol = iWidth / TILE_SIZE;
		UINT iMaxRow = iHeight / TILE_SIZE;

		UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
		UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

		// 이미지 범위를 벗어난 인덱스 체크
		if (iMaxRow <= iCurRow)
		{
			return;
			assert(nullptr);
		}



		//BitBlt(_dc
		//	, (int)(vRenderPos.x)
		//	, (int)(vRenderPos.y)
		//	, (int)vScale.x
		//	, (int)vScale.y
		//	, m_pTileTex->GetDC()
		//	, iCurCol*TILE_SIZE
		//	, iCurRow * TILE_SIZE
		//	,SRCCOPY);


		TransparentBlt(_dc
			, int(vRenderPos.x)
			, int(vRenderPos.y)
			, (int)vScale.x, (int)vScale.y
			, m_pTileTex->GetDC()
			, iCurCol * TILE_SIZE, iCurRow * TILE_SIZE,
			TILE_SIZE, TILE_SIZE, RGB(255, 0, 255));

	}
	


	if (nullptr != m_pTileTex2 && -1 != m_iImgIdx2)
	{
		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
		Vec2 vScale = GetScale();


		UINT iWidth = m_pTileTex2->Width();
		UINT iHeight = m_pTileTex2->Height();

		UINT iMaxCol = iWidth / TILE_SIZE;
		UINT iMaxRow = iHeight / TILE_SIZE;

		UINT iCurRow = (UINT)m_iImgIdx2 / iMaxCol;
		UINT iCurCol = (UINT)m_iImgIdx2 % iMaxCol;

		// 이미지 범위를 벗어난 인덱스 체크
		if (iMaxRow <= iCurRow)
		{
			return;
		}

		TransparentBlt(_dc
			, int(vRenderPos.x)
			, int(vRenderPos.y)
			, (int)vScale.x, (int)vScale.y
			, m_pTileTex2->GetDC()
			, iCurCol * TILE_SIZE, iCurRow * TILE_SIZE,
			TILE_SIZE, TILE_SIZE, RGB(255, 0, 255));
	}

    if (CSceneMgr::GetInst()->GetCurScene()->GetDrawGroundType() && m_eVertexPosition != VertexPosition::NONE)
    {

        PEN_TYPE ePen = PEN_TYPE::BLUE;


        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            ePen = PEN_TYPE::GREEN;
        }
        else if (m_eGroundType == GROUND_TYPE::UNWALKABLE)
        {
            ePen = PEN_TYPE::PURPLE;
        }
        else if (m_eGroundType == GROUND_TYPE::DAMAGEZONE)
        {
            ePen = PEN_TYPE::ORANGE;
        }
        else if (m_eGroundType == GROUND_TYPE::DEADZONE)
        {
            ePen = PEN_TYPE::RED;
        }

        SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);
        SelectGDI p(_dc, ePen);

        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetPos().x + 2, GetPos().y + 2));
        Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);



        Rectangle(_dc, (int)(vRenderPos.x)
            , (int)(vRenderPos.y)
            , (int)(vRenderPos.x + vScale.x)
            , (int)(vRenderPos.y + vScale.y));
    }

	



    GameObject::Component_Render(_dc);
}

void CTile::Save(FILE* _pFile)
{
	//fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);

	fprintf(_pFile, "[Tile]\n");
	fprintf(_pFile, "%d\n", m_iImgIdx);
	fprintf(_pFile, "%d\n", m_iImgIdx2);

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
	if (m_pTileTex2)
	{
		fprintf(_pFile, "[Texture_Name]\n");
		string strName = string(m_pTileTex2->GetKey().begin(), m_pTileTex2->GetKey().end());
		fprintf(_pFile, strName.c_str());
		fprintf(_pFile, "\n");

		fprintf(_pFile, "[Texture_Path]\n");
		strName = string(m_pTileTex2->GetRelativePath().begin(), m_pTileTex2->GetRelativePath().end());
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

    fprintf(_pFile, "[VertexPosition]\n");
    if (m_eVertexPosition == VertexPosition::NONE)
        fprintf(_pFile, "0\n");
    else if (m_eVertexPosition == VertexPosition::TOP_LEFT)
        fprintf(_pFile, "1\n");
    else if (m_eVertexPosition == VertexPosition::BOT_RIGHT)
        fprintf(_pFile, "2\n");
    
    fprintf(_pFile, "[GroundType]\n");
    if (m_eGroundType == GROUND_TYPE::NORMAL)
        fprintf(_pFile, "0\n");
    else if (m_eGroundType == GROUND_TYPE::UNWALKABLE)
        fprintf(_pFile, "1\n");
    else if (m_eGroundType == GROUND_TYPE::DAMAGEZONE)
        fprintf(_pFile, "2\n");
    else if (m_eGroundType == GROUND_TYPE::DEADZONE)
        fprintf(_pFile, "3\n");
    
    
    
	fprintf(_pFile, "\n");
}

void CTile::Load(FILE* _pFile)
{
	char szBuff[256] = {};
	string str;

	FScanf(szBuff, _pFile);//[Tile]
	fscanf_s(_pFile, "%d", &m_iImgIdx);
	FScanf(szBuff, _pFile);
	fscanf_s(_pFile, "%d", &m_iImgIdx2);
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

		m_pTileTex2 = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);
	}
	else
	{
		FScanf(szBuff, _pFile);
		FScanf(szBuff, _pFile);
	}

    FScanf(szBuff, _pFile); // [VertexPosition] 섹션
    int iVertexType;
    fscanf_s(_pFile, "%d", &iVertexType);
    FScanf(szBuff, _pFile);

    // VertexType 설정
    switch (iVertexType)
    {
        case 0: m_eVertexPosition = VertexPosition::NONE; break;
        case 1: m_eVertexPosition = VertexPosition::TOP_LEFT; break;
        case 2: m_eVertexPosition = VertexPosition::BOT_RIGHT; break;
    }
    

    FScanf(szBuff, _pFile); // [GroundType] 섹션
    int iGroundType;
    fscanf_s(_pFile, "%d", &iGroundType);
    FScanf(szBuff, _pFile);

    // GroundType 설정
    switch (iGroundType) {
    case 0: m_eGroundType = GROUND_TYPE::NORMAL; break;
    case 1: m_eGroundType = GROUND_TYPE::UNWALKABLE; break;
    case 2: m_eGroundType = GROUND_TYPE::DAMAGEZONE; break;
    case 3: m_eGroundType = GROUND_TYPE::DEADZONE; break;
    default: m_eGroundType = GROUND_TYPE::NORMAL; break;
    }
    
	FScanf(szBuff, _pFile);
}

void CTile::OnCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetName() == L"Player")
    {
    }
    
    // if(m_eCollideType == COLLIDE_TYPE::SOLID)
    // {
    //     // 기본 충돌 반응
    //     ResolveCollision(_pOther);
    // }
    // else if(m_eCollideType == COLLIDE_TYPE::TOP_PLATFORM)
    // {
    //     // 플랫폼 타입(상단에서만 충돌)
    //     if(CheckPlatformCondition(_pOther))
    //     {
    //         ResolveCollision(_pOther);
    //     }
    // }
    
}


