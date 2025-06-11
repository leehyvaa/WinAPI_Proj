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
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Gdiplus.lib")

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
//
//void CTile::Render(HDC _dc)
// {
//     // Dx2D 렌더링이 활성화된 경우 GDI 렌더링 스킵
//     ID2D1RenderTarget* pD2DRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
//     if (pD2DRenderTarget != nullptr)
//         return;
//     
//     
//     // 전면 텍스쳐 그리기
// 	if (nullptr != m_pTileTex && -1 != m_iImgIdx)
// 	{
// 		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
// 		Vec2 vScale = GetScale();
//
// 		UINT iWidth = m_pTileTex->Width();
// 		UINT iHeight = m_pTileTex->Height();
//
// 		UINT iMaxCol = iWidth / TILE_SIZE;
// 		UINT iMaxRow = iHeight / TILE_SIZE;
//
// 		UINT iCurRow = static_cast<UINT>(m_iImgIdx) / iMaxCol;
// 		UINT iCurCol = static_cast<UINT>(m_iImgIdx) % iMaxCol;
//
// 		// 이미지 범위를 벗어난 인덱스 체크
// 		if (iMaxRow <= iCurRow)
// 		{
// 			return;
// 		}
//
// 		TransparentBlt(_dc
// 			, static_cast<int>(vRenderPos.x)
// 			, static_cast<int>(vRenderPos.y)
// 			, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
// 			, m_pTileTex->GetDC()
// 			, iCurCol * TILE_SIZE, iCurRow * TILE_SIZE,
// 			TILE_SIZE, TILE_SIZE, RGB(255, 0, 255));
// 	}
//     
//     // 후면 텍스쳐 그리기
// 	if (nullptr != m_pTileTex2 && -1 != m_iImgIdx2)
// 	{
// 		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
// 		Vec2 vScale = GetScale();
//
// 		UINT iWidth = m_pTileTex2->Width();
// 		UINT iHeight = m_pTileTex2->Height();
//
// 		UINT iMaxCol = iWidth / TILE_SIZE;
// 		UINT iMaxRow = iHeight / TILE_SIZE;
//
// 		UINT iCurRow = static_cast<UINT>(m_iImgIdx2) / iMaxCol;
// 		UINT iCurCol = static_cast<UINT>(m_iImgIdx2) % iMaxCol;
//
// 		// 이미지 범위를 벗어난 인덱스 체크
// 		if (iMaxRow <= iCurRow)
// 		{
// 			return;
// 		}
//
// 		TransparentBlt(_dc
// 			, static_cast<int>(vRenderPos.x)
// 			, static_cast<int>(vRenderPos.y)
// 			, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
// 			, m_pTileTex2->GetDC()
// 			, iCurCol * TILE_SIZE, iCurRow * TILE_SIZE,
// 			TILE_SIZE, TILE_SIZE, RGB(255, 0, 255));
// 	}
//
//     // 디버그용 그라운드 타입 렌더링, 이거 Dx쪽으로 옮겨야함
//     if (CSceneMgr::GetInst()->GetCurScene()->GetDrawGroundType() && m_eVertexPosition != VERTEX_POSITION::NONE)
//     {
//         PEN_TYPE ePen = PEN_TYPE::BLUE;
//
//         if (m_eGroundType == GROUND_TYPE::NORMAL)
//         {
//             ePen = PEN_TYPE::GREEN;
//         }
//         else if (m_eGroundType == GROUND_TYPE::UNWALKABLE)
//         {
//             ePen = PEN_TYPE::PURPLE;
//         }
//         else if (m_eGroundType == GROUND_TYPE::DAMAGEZONE)
//         {
//             ePen = PEN_TYPE::ORANGE;
//         }
//         else if (m_eGroundType == GROUND_TYPE::DEADZONE)
//         {
//             ePen = PEN_TYPE::RED;
//         }
//
//         // GDI 모드에서는 기본 펜과 브러시 사용
//         HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 기본 빨간색
//         HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
//         
//         HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
//         HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);
//
//         Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetWorldPos().x + 2, GetWorldPos().y + 2));
//         Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);
//
//         Rectangle(_dc, static_cast<int>(vRenderPos.x)
//             , static_cast<int>(vRenderPos.y)
//             , static_cast<int>(vRenderPos.x + vScale.x)
//             , static_cast<int>(vRenderPos.y + vScale.y));
//             
//         // 리소스 정리
//         SelectObject(_dc, hOldPen);
//         SelectObject(_dc, hOldBrush);
//         DeleteObject(hPen);
//     }
//
//     GameObject::Component_Render(_dc);
// }

void CTile::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    // 전면 텍스쳐 그리기
    if (nullptr != m_pTileTex && -1 != m_iImgIdx)
    {
        // 디버깅: 텍스처 정보 출력
        static int debugCount = 0;
        if (debugCount < 5) // 처음 5개 타일만 로그 출력
        {
            OutputDebugStringA("CTile::RenderD2D - Tile texture found\n");
            debugCount++;
        }

        // 텍스처 유효성 체크 (디버깅용)
        if (!m_pTileTex->IsValid())
        {
            // 텍스처가 유효하지 않으면 스킵
            if (debugCount < 5)
            {
                OutputDebugStringA("CTile::RenderD2D - Texture is not valid\n");
            }
            return;
        }

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
        ID2D1Bitmap* pD2DBitmap = m_pTileTex->GetD2DBitmap();

        if (debugCount < 5)
        {
            if (pD2DBitmap)
                OutputDebugStringA("CTile::RenderD2D - D2D bitmap found, rendering tile\n");
            else
                OutputDebugStringA("CTile::RenderD2D - D2D bitmap is NULL\n");
        }

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
        // 텍스처 유효성 체크 (디버깅용)
        if (!m_pTileTex2->IsValid())
        {
            // 텍스처가 유효하지 않으면 스킵
            return;
        }

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

        // Direct2D 비트맵 직접 사용 (PNG 알파 채널 지원)
        ID2D1Bitmap* pD2DBitmap = m_pTileTex2->GetD2DBitmap();

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
    int iVertexType;
    fscanf_s(_pFile, "%d", &iVertexType);
    FScanf(szBuff, _pFile);

    // VertexType 설정
    switch (iVertexType)
    {
        case 0: m_eVertexPosition = VERTEX_POSITION::NONE; break;
        case 1: m_eVertexPosition = VERTEX_POSITION::TOP_LEFT; break;
        case 2: m_eVertexPosition = VERTEX_POSITION::BOT_RIGHT; break;
    }
    

    FScanf(szBuff, _pFile); // [GroundType] 섹션
    int iGroundType;
    fscanf_s(_pFile, "%d", &iGroundType);
    FScanf(szBuff, _pFile);

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
    fscanf_s(_pFile, "%d", &m_iBotRightTileIdx);
    FScanf(szBuff, _pFile);

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


