#include "pch.h"
#include "CTile.h"

#include "CCollider.h"
#include "CTexture.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include <d2d1.h>
#include <wincodec.h>



CTile::CTile()
	:m_pTileTex(nullptr)
	,m_pTileTex2(nullptr)
	,m_iImgIdx2(0)
	,m_iImgIdx(0)
    ,m_eGroundType(GROUND_TYPE::NONE)
    ,m_eVertexPosition(VERTEX_POSITION::NONE)
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
void CTile::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    // 전면 텍스쳐 그리기
    if (nullptr != m_pTileTex && -1 != m_iImgIdx)
    {
        if (!m_pTileTex->IsValid())
            return;

        UINT iWidth = m_pTileTex->Width();
        UINT iHeight = m_pTileTex->Height();

        // TILE_SIZE 유효성 확인
        if (TILE_SIZE == 0)
            return;

        UINT iMaxCol = iWidth / TILE_SIZE;
        UINT iMaxRow = iHeight / TILE_SIZE;

        UINT iCurRow = static_cast<UINT>(m_iImgIdx) / iMaxCol;
        UINT iCurCol = static_cast<UINT>(m_iImgIdx) % iMaxCol;

        // 이미지 범위를 벗어난 인덱스 체크
        if (iMaxRow <= iCurRow)
            return;

        // 소스 사각형 계산
        D2D1_RECT_F srcRect = D2D1::RectF(
            static_cast<float>(iCurCol * TILE_SIZE),
            static_cast<float>(iCurRow * TILE_SIZE),
            static_cast<float>((iCurCol + 1) * TILE_SIZE),
            static_cast<float>((iCurRow + 1) * TILE_SIZE)
        );

        // 목적지 크기 계산
        Vec2 vScale = GetScale();
        D2D1_SIZE_F dstSize = D2D1::SizeF(vScale.x, vScale.y);

        // Direct2D 비트맵 직접 사용 (PNG 알파 채널 지원)
        ID2D1Bitmap* pD2DBitmap = m_pTileTex->GetBitmap();
        
        if (pD2DBitmap)
        {
            Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());

            D2D1_RECT_F destRect = D2D1::RectF(
                vRenderPos.x,
                vRenderPos.y,
                vRenderPos.x + vScale.x,
                vRenderPos.y + vScale.y
            );

            _pRenderTarget->DrawBitmap(
                pD2DBitmap,
                destRect,
                1.0f,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                srcRect
            );
        }
    }

    // 후면 텍스쳐 그리기
    if (nullptr != m_pTileTex2 && -1 != m_iImgIdx2)
    {
        if (!m_pTileTex2->IsValid())
            return;


        UINT iWidth = m_pTileTex2->Width();
        UINT iHeight = m_pTileTex2->Height();

        // TILE_SIZE 유효성 확인
        if (TILE_SIZE == 0)
            return;

        UINT iMaxCol = iWidth / TILE_SIZE;
        UINT iMaxRow = iHeight / TILE_SIZE;

        UINT iCurRow = static_cast<UINT>(m_iImgIdx2) / iMaxCol;
        UINT iCurCol = static_cast<UINT>(m_iImgIdx2) % iMaxCol;

        // 이미지 범위를 벗어난 인덱스 체크
        if (iMaxRow <= iCurRow)
            return;

        // 소스 사각형 계산
        D2D1_RECT_F srcRect = D2D1::RectF(
            static_cast<float>(iCurCol * TILE_SIZE),
            static_cast<float>(iCurRow * TILE_SIZE),
            static_cast<float>((iCurCol + 1) * TILE_SIZE),
            static_cast<float>((iCurRow + 1) * TILE_SIZE)
        );

        // 목적지 크기 계산
        Vec2 vScale = GetScale();
        D2D1_SIZE_F dstSize = D2D1::SizeF(vScale.x, vScale.y);
        
        ID2D1Bitmap* pD2DBitmap = m_pTileTex2->GetBitmap();

        if (pD2DBitmap)
        {
            Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());

            D2D1_RECT_F destRect = D2D1::RectF(
                vRenderPos.x,
                vRenderPos.y,
                vRenderPos.x + vScale.x,
                vRenderPos.y + vScale.y
            );

            _pRenderTarget->DrawBitmap(
                pD2DBitmap,
                destRect,
                1.0f,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                srcRect
            );
        }
    }
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

		// BMP에서 PNG로 마이그레이션: 저장 시 확장자를 PNG로 강제 변환
		if (strName.find(".bmp") != string::npos) {
			strName = strName.substr(0, strName.find(".bmp")) + ".png";
		}

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

		// BMP에서 PNG로 마이그레이션: 저장 시 확장자를 PNG로 강제 변환
		if (strName.find(".bmp") != string::npos) {
			strName = strName.substr(0, strName.find(".bmp")) + ".png";
		}

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
    if (m_eVertexPosition == VERTEX_POSITION::NONE)
        fprintf(_pFile, "0\n");
    else if (m_eVertexPosition == VERTEX_POSITION::TOP_LEFT)
        fprintf(_pFile, "1\n");
    else if (m_eVertexPosition == VERTEX_POSITION::BOT_RIGHT)
        fprintf(_pFile, "2\n");
    
    fprintf(_pFile, "[GroundType]\n");
    if (m_eGroundType == GROUND_TYPE::NONE)
        fprintf(_pFile, "0\n");
    else if (m_eGroundType == GROUND_TYPE::NORMAL)
        fprintf(_pFile, "1\n");
    else if (m_eGroundType == GROUND_TYPE::UNWALKABLE)
        fprintf(_pFile, "2\n");
    else if (m_eGroundType == GROUND_TYPE::DAMAGEZONE)
        fprintf(_pFile, "3\n");
    else if (m_eGroundType == GROUND_TYPE::DEADZONE)
        fprintf(_pFile, "4\n");

    fprintf(_pFile, "[BotRightTileIdx]\n");
    fprintf(_pFile, "%d\n", m_iBotRightTileIdx);
    
	fprintf(_pFile, "\n");
}

void CTile::Load(FILE* _pFile)
{
	char szBuff[256] = {};
	string str;

	FScanf(szBuff, _pFile);//[Tile]
	FScanf(szBuff, _pFile); // m_iImgIdx 라인 읽기
	m_iImgIdx = atoi(szBuff);
	FScanf(szBuff, _pFile); // m_iImgIdx2 라인 읽기
	m_iImgIdx2 = atoi(szBuff);

	FScanf(szBuff, _pFile); //[Texture_Name]
	FScanf(szBuff, _pFile);

	if (strcmp(szBuff, "-1"))
	{
		str = szBuff;
		wstring strTexKey = wstring(str.begin(), str.end());

		FScanf(szBuff, _pFile);//[Texture_Path]
		FScanf(szBuff, _pFile);

		str = szBuff;
		wstring strTexPath = wstring(str.begin(), str.end());

		// BMP에서 PNG로 마이그레이션: 확장자 자동 변경
		if (strTexPath.find(L".bmp") != wstring::npos) {
			strTexPath = strTexPath.substr(0, strTexPath.find(L".bmp")) + L".png";
		}

		// 디버깅: 로딩하려는 텍스처 경로 출력
		string debugPath = string(strTexPath.begin(), strTexPath.end());
		OutputDebugStringA(("CTile::Load - Loading texture (converted): " + debugPath + "\n").c_str());

		m_pTileTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);

		// 디버깅: 텍스처 로딩 결과 확인
		if (m_pTileTex)
			OutputDebugStringA("CTile::Load - Texture loaded successfully\n");
		else
			OutputDebugStringA("CTile::Load - Texture loading FAILED\n");
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

		// BMP에서 PNG로 마이그레이션: 확장자 자동 변경
		if (strTexPath.find(L".bmp") != wstring::npos) {
			strTexPath = strTexPath.substr(0, strTexPath.find(L".bmp")) + L".png";
		}

		m_pTileTex2 = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);
	}
	else
	{
		FScanf(szBuff, _pFile);
		FScanf(szBuff, _pFile);
	}

    FScanf(szBuff, _pFile); // [VertexPosition] 섹션
    FScanf(szBuff, _pFile); // VertexType 값 라인 읽기
    int iVertexType = atoi(szBuff);

    // VertexType 설정
    switch (iVertexType)
    {
        case 0: m_eVertexPosition = VERTEX_POSITION::NONE; break;
        case 1: m_eVertexPosition = VERTEX_POSITION::TOP_LEFT; break;
        case 2: m_eVertexPosition = VERTEX_POSITION::BOT_RIGHT; break;
    }
    

    FScanf(szBuff, _pFile); // [GroundType] 섹션
    FScanf(szBuff, _pFile); // GroundType 값 라인 읽기
    int iGroundType = atoi(szBuff);

    // GroundType 설정
    switch (iGroundType) {
    case 0: m_eGroundType = GROUND_TYPE::NONE; break;
    case 1: m_eGroundType = GROUND_TYPE::NORMAL; break;
    case 2: m_eGroundType = GROUND_TYPE::UNWALKABLE; break;
    case 3: m_eGroundType = GROUND_TYPE::DAMAGEZONE; break;
    case 4: m_eGroundType = GROUND_TYPE::DEADZONE; break;
    default: m_eGroundType = GROUND_TYPE::NONE; break;
    }

    FScanf(szBuff, _pFile); // [BotRightTileIdx] 섹션
    FScanf(szBuff, _pFile); // BotRightTileIdx 값 라인 읽기
    m_iBotRightTileIdx = atoi(szBuff);

	FScanf(szBuff, _pFile);
}

void CTile::OnCollisionEnter(CCollider* _pOther)
{
    return;
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


