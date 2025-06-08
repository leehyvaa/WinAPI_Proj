#include "pch.h"
#include "CTile.h"

#include "CCollider.h"
#include "CTexture.h"
#include "SelectGDI.h"
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
    ,m_pD2DBitmap(nullptr)
    ,m_pD2DBitmap2(nullptr)
    ,m_bD2DCached(false)
    ,m_bD2DCached2(false)
{
	SetScale(Vec2(TILE_SIZE, TILE_SIZE));
}

CTile::~CTile()
{
    ReleaseD2DBitmaps();
}

void CTile::Update()
{
}

void CTile::Render(HDC _dc)
{
    // Direct2D 렌더링이 활성화된 경우 GDI 렌더링 스킵
    // CAnimation과 동일한 패턴으로 중복 렌더링 방지
    ID2D1RenderTarget* pD2DRenderTarget = CCore::GetInst()->GetD2DRenderTarget();
    if (pD2DRenderTarget != nullptr)
    {
        // Direct2D 렌더링이 활성화된 상태이므로 GDI 렌더링을 건너뜀
        return;
    }
    
    // Direct2D가 비활성화된 경우에만 GDI 렌더링 수행
	if (nullptr != m_pTileTex && -1 != m_iImgIdx)
	{
		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
		Vec2 vScale = GetScale();

		UINT iWidth = m_pTileTex->Width();
		UINT iHeight = m_pTileTex->Height();

		UINT iMaxCol = iWidth / TILE_SIZE;
		UINT iMaxRow = iHeight / TILE_SIZE;

		UINT iCurRow = static_cast<UINT>(m_iImgIdx) / iMaxCol;
		UINT iCurCol = static_cast<UINT>(m_iImgIdx) % iMaxCol;

		// 이미지 범위를 벗어난 인덱스 체크
		if (iMaxRow <= iCurRow)
		{
			return;
		}

		TransparentBlt(_dc
			, static_cast<int>(vRenderPos.x)
			, static_cast<int>(vRenderPos.y)
			, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
			, m_pTileTex->GetDC()
			, iCurCol * TILE_SIZE, iCurRow * TILE_SIZE,
			TILE_SIZE, TILE_SIZE, RGB(255, 0, 255));
	}

	if (nullptr != m_pTileTex2 && -1 != m_iImgIdx2)
	{
		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
		Vec2 vScale = GetScale();

		UINT iWidth = m_pTileTex2->Width();
		UINT iHeight = m_pTileTex2->Height();

		UINT iMaxCol = iWidth / TILE_SIZE;
		UINT iMaxRow = iHeight / TILE_SIZE;

		UINT iCurRow = static_cast<UINT>(m_iImgIdx2) / iMaxCol;
		UINT iCurCol = static_cast<UINT>(m_iImgIdx2) % iMaxCol;

		// 이미지 범위를 벗어난 인덱스 체크
		if (iMaxRow <= iCurRow)
		{
			return;
		}

		TransparentBlt(_dc
			, static_cast<int>(vRenderPos.x)
			, static_cast<int>(vRenderPos.y)
			, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
			, m_pTileTex2->GetDC()
			, iCurCol * TILE_SIZE, iCurRow * TILE_SIZE,
			TILE_SIZE, TILE_SIZE, RGB(255, 0, 255));
	}

    // 디버그용 그라운드 타입 렌더링은 Direct2D 상태와 관계없이 항상 GDI로 처리
    if (CSceneMgr::GetInst()->GetCurScene()->GetDrawGroundType() && m_eVertexPosition != VERTEX_POSITION::NONE)
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

        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetWorldPos().x + 2, GetWorldPos().y + 2));
        Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);

        Rectangle(_dc, static_cast<int>(vRenderPos.x)
            , static_cast<int>(vRenderPos.y)
            , static_cast<int>(vRenderPos.x + vScale.x)
            , static_cast<int>(vRenderPos.y + vScale.y));
    }

    GameObject::Component_Render(_dc);
}

void CTile::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    // 첫 번째 타일 텍스처 렌더링 (캐싱 시스템 적용)
    if (nullptr != m_pTileTex && -1 != m_iImgIdx)
    {
        // 캐시된 비트맵이 없으면 생성
        if (!m_bD2DCached || !m_pD2DBitmap)
        {
            CacheD2DBitmap(_pRenderTarget, m_pTileTex, m_iImgIdx, &m_pD2DBitmap);
            m_bD2DCached = true;
        }

        // 캐시된 D2D 비트맵으로 고속 렌더링
        if (m_pD2DBitmap)
        {
            Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
            Vec2 vScale = GetScale();

            D2D1_RECT_F destRect = D2D1::RectF(
                vRenderPos.x,
                vRenderPos.y,
                vRenderPos.x + vScale.x,
                vRenderPos.y + vScale.y
            );

            _pRenderTarget->DrawBitmap(
                m_pD2DBitmap,
                destRect,
                1.0f,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
            );
        }
    }

    // 두 번째 타일 텍스처 렌더링 (캐싱 시스템 적용)
    if (nullptr != m_pTileTex2 && -1 != m_iImgIdx2)
    {
        // 캐시된 비트맵이 없으면 생성
        if (!m_bD2DCached2 || !m_pD2DBitmap2)
        {
            CacheD2DBitmap(_pRenderTarget, m_pTileTex2, m_iImgIdx2, &m_pD2DBitmap2);
            m_bD2DCached2 = true;
        }

        // 캐시된 D2D 비트맵으로 고속 렌더링
        if (m_pD2DBitmap2)
        {
            Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
            Vec2 vScale = GetScale();

            D2D1_RECT_F destRect = D2D1::RectF(
                vRenderPos.x,
                vRenderPos.y,
                vRenderPos.x + vScale.x,
                vRenderPos.y + vScale.y
            );

            _pRenderTarget->DrawBitmap(
                m_pD2DBitmap2,
                destRect,
                1.0f,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
            );
        }
    }
}

void CTile::CacheD2DBitmap(ID2D1RenderTarget* _pRenderTarget, CTexture* _pTex, int _iImgIdx, ID2D1Bitmap** _ppD2DBitmap)
{
    if (!_pRenderTarget || !_pTex || _iImgIdx == -1 || !_ppD2DBitmap)
        return;

    // 기존 비트맵이 있으면 해제
    if (*_ppD2DBitmap)
    {
        (*_ppD2DBitmap)->Release();
        *_ppD2DBitmap = nullptr;
    }

    Vec2 vScale = GetScale();

    UINT iWidth = _pTex->Width();
    UINT iHeight = _pTex->Height();

    UINT iMaxCol = iWidth / TILE_SIZE;
    UINT iMaxRow = iHeight / TILE_SIZE;

    UINT iCurRow = static_cast<UINT>(_iImgIdx) / iMaxCol;
    UINT iCurCol = static_cast<UINT>(_iImgIdx) % iMaxCol;

    // 이미지 범위를 벗어난 인덱스 체크
    if (iMaxRow <= iCurRow)
        return;

    // WIC Factory를 정적으로 관리하여 재사용
    static IWICImagingFactory* s_pWICFactory = nullptr;
    if (!s_pWICFactory)
    {
        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory, (LPVOID*)&s_pWICFactory
        );
        if (FAILED(hr))
            return;
    }

    // GDI+ 비트맵으로 텍스처 로드
    HBITMAP hSourceBitmap = _pTex->GetHBITMAP();
    if (!hSourceBitmap)
        return;

    Bitmap sourceGdiplusBitmap(hSourceBitmap, nullptr);

    // 타일 영역 크기 계산
    int srcX = iCurCol * TILE_SIZE;
    int srcY = iCurRow * TILE_SIZE;
    int srcWidth = TILE_SIZE;
    int srcHeight = TILE_SIZE;
    
    int destWidth = static_cast<int>(vScale.x);
    int destHeight = static_cast<int>(vScale.y);

    // 32비트 ARGB GDI+ 비트맵 생성 (투명 처리를 위해)
    Bitmap* tileArgbBitmap = new Bitmap(destWidth, destHeight, PixelFormat32bppARGB);
    Graphics tileGraphics(tileArgbBitmap);
    
    // 픽셀 깨짐 방지
    tileGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    tileGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);

    // 투명색 지정 (마젠타)
    ImageAttributes imgAttr;
    imgAttr.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

    // 타일 영역에서 투명색 적용 후 그리기
    tileGraphics.DrawImage(
        &sourceGdiplusBitmap,
        Rect(0, 0, destWidth, destHeight),
        srcX, srcY, srcWidth, srcHeight,
        UnitPixel,
        &imgAttr
    );

    IWICBitmap* pWICBitmap = nullptr;

    // 투명 처리된 비트맵에서 HBITMAP 추출
    HBITMAP hArgbBitmap = NULL;
    if (tileArgbBitmap->GetHBITMAP(Color(0, 0, 0, 0), &hArgbBitmap) == Ok)
    {
        // HBITMAP -> WIC 비트맵 변환
        HRESULT hr = s_pWICFactory->CreateBitmapFromHBITMAP(
            hArgbBitmap, nullptr, WICBitmapUsePremultipliedAlpha, &pWICBitmap
        );
        
        if (SUCCEEDED(hr))
        {
            // WIC 비트맵 -> D2D 비트맵 변환하여 캐시에 저장
            hr = _pRenderTarget->CreateBitmapFromWicBitmap(pWICBitmap, nullptr, _ppD2DBitmap);
        }
        DeleteObject(hArgbBitmap);
    }

    // 임시 리소스 정리
    if (pWICBitmap)
        pWICBitmap->Release();
    delete tileArgbBitmap;
}

void CTile::ReleaseD2DBitmaps()
{
    if (m_pD2DBitmap)
    {
        m_pD2DBitmap->Release();
        m_pD2DBitmap = nullptr;
    }
    
    if (m_pD2DBitmap2)
    {
        m_pD2DBitmap2->Release();
        m_pD2DBitmap2 = nullptr;
    }
    
    m_bD2DCached = false;
    m_bD2DCached2 = false;
}

void CTile::RenderSingleTileD2D(ID2D1RenderTarget* _pRenderTarget, CTexture* _pTex, int _iImgIdx)
{
    // 이 메서드는 이제 사용되지 않음 (캐싱 시스템으로 대체)
    // 하위 호환성을 위해 빈 함수로 유지
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


