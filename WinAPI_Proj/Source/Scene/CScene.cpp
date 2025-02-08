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
#include "CGround.h"
#include "CTextUI.h"
#include "SPlayer.h"

CScene::CScene()
	:m_iTileX(0)
	,m_iTileY(0)
	,m_iGroundCount(0)
	,m_pPlayer(nullptr)
	,bDrawGrid(false)
	,bDrawCollider(false)
	,bDrawGroundType(false)
	,bDrawOutWindow(false)
    ,bDrawCompleteGround(false)
	,backGround(nullptr)
    ,m_pPlayerText(nullptr)
    ,bDrawPlayerState(true)
{

    m_pPlayerText = new CTextUI();
    m_pPlayerText->SetPos(Vec2(900, 0));
    m_pPlayerText->SetAlign(CTextUI::TEXT_ALIGN::CENTER);
    m_pPlayerText->SetLineSpace(5);
    m_pPlayerText->SetVisibleBox(false);
    m_pPlayerText->SetFontSize(20);
    m_pPlayerText->SetFontColor(RGB(0,0,255));
    AddObject(m_pPlayerText, GROUP_TYPE::UI);
}

CScene::~CScene()
{
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
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
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			m_arrObj[i][j]->Start();
		}
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
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
    if (KEY_TAP(KEY::F8))
    {
        bDrawCompleteGround = !bDrawCompleteGround;
    }
	if (KEY_TAP(KEY::F12))
	{
		bDrawOutWindow = !bDrawOutWindow;
	}
    if (KEY_TAP(KEY::C))
    {
        bDrawPlayerState = !bDrawPlayerState;
    }

    if (bDrawPlayerState)
    {
        m_pPlayerText->ClearLines();
        const vector<GameObject*>& vecPlayer = GetGroupObject(GROUP_TYPE::PLAYER);
        SPlayer* player;
        if (vecPlayer.size() > 0)
        {
            player = static_cast<SPlayer*>(vecPlayer[0]);
            static wstring state;
            static wstring ground;
            static wstring climb;
            if (player->IsOnGround())
                ground = L"GROUND";
            else
                ground = L"Not GROUND";
            
            if (player->IsWallClimbing())
                climb = L"Climb";
            else
                climb = L"Not Climb";

            player->GetState();
            switch (player->GetState())
            {
            case PLAYER_STATE::IDLE:
                state = L"IDLE";
                break;
            case PLAYER_STATE::RUN:
                state = L"RUN";
                break;
            case PLAYER_STATE::JUMP:
                state = L"JUMP";
                break;
            case PLAYER_STATE::SWING:
                state = L"SWING";
                break;
            case PLAYER_STATE::FALL:
                state = L"FALL";
                break;
            case PLAYER_STATE::CLIMB:
                state = L"CLIMB";
                break;
            case PLAYER_STATE::CLIMBUP:
                state = L"CLIMBUP";
                break;
            case PLAYER_STATE::CLIMBDOWN:
                state = L"CLIMBDOWN";
                break;
            case PLAYER_STATE::SHOT:
                state = L"SHOT";
                break;
            case PLAYER_STATE::ATTACK:
                state = L"ATTACK";
                break;
            case PLAYER_STATE::DAMAGED:
                state = L"DAMAGED";
                break;
            case PLAYER_STATE::DEAD:
                state = L"DEAD";
                break;
            }
            vector<wstring> Texts =
            {
                state,
                ground,
                climb,
            };

            m_pPlayerText->AddLines(Texts);
        }
    }
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			m_arrObj[i][j]->FinalUpdate();
		}
	}
}

void CScene::Render(HDC _dc)
{
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		if (static_cast<UINT>(GROUP_TYPE::TILE) == i && !bDrawOutWindow)
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

// 해당 씬에서 그룹타입이 TILE인 모든 오브젝트를 그리는 함수
void CScene::Render_Tile(HDC _dc)
{
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;
	
	int iTileSize = TILE_SIZE;

	int iLTCol = static_cast<int>(vLeftTop.x) / iTileSize;
	int iLTRow = static_cast<int>(vLeftTop.y) / iTileSize;





	int iLTIdx = m_iTileX * iLTRow + iLTCol;

	int iClientWidth = (static_cast<int>(vResolution.x) / iTileSize) +2;
	int iClientHeight = (static_cast<int>(vResolution.y) / iTileSize)+2;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow +iClientHeight); iCurRow++)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); iCurCol++)
		{
			if (iCurCol < 0 || m_iTileX <= static_cast<UINT>(iCurCol) ||
				iCurRow < 0 || m_iTileY <= static_cast<UINT>(iCurRow))
			{
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			if(bDrawGrid)
			{
				Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vecTile[iIdx]->GetPos());
				Vec2 vScale = vecTile[iIdx]->GetScale();

				SelectGDI brush(_dc, BRUSH_TYPE::HOLLOW);


				Rectangle(_dc, static_cast<int>(vRenderPos.x)
					, static_cast<int>(vRenderPos.y)
					, static_cast<int>(vRenderPos.x + vScale.x)
					, static_cast<int>(vRenderPos.y + vScale.y));
			    
			    
			}
            

			vecTile[iIdx]->Render(_dc);

		    // 현재 타일의 위치부터 BotRightTile까지 선 그리기
		    if (bDrawCompleteGround &&
                static_cast<CTile*>(vecTile[iIdx])->GetGroundType() != GROUND_TYPE::NONE)
		    {
		        int botIdx = static_cast<CTile*>(vecTile[iIdx])->GetBotRightTileIdx();
		        if (botIdx != -1 && botIdx < static_cast<int>(vecTile.size()))
		        {
		            Vec2 vStartPos = CCamera::GetInst()->GetRenderPos(vecTile[iIdx]->GetPos());
		            Vec2 vEndPos = CCamera::GetInst()->GetRenderPos(vecTile[botIdx]->GetPos());
		            SelectGDI brush(_dc, PEN_TYPE::BIGGREEN);

		            // 화면에 선 그리기
		            MoveToEx(_dc, (int)vStartPos.x, (int)vStartPos.y, nullptr);
		            LineTo(_dc, (int)vEndPos.x, (int)vEndPos.y);
		        }
		    }
		}
	}

}


//지정된 그룹의 벡터를 지우는 함수
void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec<GameObject*>(m_arrObj[static_cast<UINT>(_eTarget)]);
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		DeleteGroup(static_cast<GROUP_TYPE>(i));
	}
}





/*
    파일에서 타일 정보들을 읽어오는 함수
    파일 첫부분에 x,y 타일 카운트를 읽은 후
    그 만큼 CreateTile을 해서 타일을 만들어 둔다.
    만든 모든 타일에 개별로 Load함수를 사용한다
 */
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


	// 불러온 개수에 맞게 EmptyTile 들 만들어두기
	CreateTile(xCount, yCount);


	// 만들어진 타일 개별로 필요한 정보를 불러옴
	const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		static_cast<CTile*>(vecTile[i])->Load(pFile);
	}



    // Ground 불러오기
	// const vector<GameObject*>& vecGround = GetGroupObject(GROUP_TYPE::GROUND);
	// FScanf(szBuff, pFile); //[GroundCount]
	// fscanf_s(pFile, "%d", &m_iGroundCount);
	// FScanf(szBuff, pFile); //[GroundCount]
	//
	// //불러온 개수에 맞게 Ground생성
	// CreateEmptyGround(m_iGroundCount);
	//
	// for (size_t i = 0; i < vecGround.size(); i++)
	// {
	// 	((CGround*)vecGround[i])->Load(pFile);
	// }


	fclose(pFile);

}

/*
    Tile 그룹을 전부 지우고
    매개변수로 해당 씬의 x 타일 개수,y타일 개수를 받아
    타일 개수에 맞게 타일을 생성한다.
 */
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

            pTile->SetPos(Vec2(static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE)));
            //pTile->SetTexture(pTileTex);
            AddObject(pTile, GROUP_TYPE::TILE);
        }
    }
}

/*
    지형의 맨 왼쪽 위 꼭짓점과 오른쪽 아래 꼭짓점의 위치를 받아와서
    해당 사각형 지형 하나당 상하좌우 4개의 콜라이더 지형을 생성한다.
 */
void CScene::CreateGround()
{

	DeleteGroup(GROUP_TYPE::GROUND);
    const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

    vector<pair<Vec2, Vec2>> vNormalGround;
    
    for (size_t i = 0; i < vecTile.size(); i++)
    {
        CTile* pTile = static_cast<CTile*>(vecTile[i]);
        
        if (pTile->GetVertexPosition() == VERTEX_POSITION::TOP_LEFT)
         {
             Vec2 vPos1 = pTile->GetPos();
             int iBotRightIdx = pTile->GetBotRightTileIdx();
             if (iBotRightIdx < 0 || iBotRightIdx >= (int)vecTile.size()) continue;

             Vec2 vPos2 = vecTile[iBotRightIdx]->GetPos() + vecTile[iBotRightIdx]->GetScale();
             GROUND_TYPE groundType = pTile->GetGroundType();
             Vec2 vTileScale = pTile->GetScale();
            
             if (groundType == GROUND_TYPE::NORMAL)
             {
                 // 1. 상단 지형 (평지)
                 CGround* pTop = new CGround();
                 pTop->SetPos(Vec2(vPos1.x + vTileScale.x,vPos1.y));
                 pTop->SetGroundType(groundType);
                 pTop->SetScale(Vec2(vPos2.x - vPos1.x - vTileScale.x, vTileScale.y));
                 pTop->SetCollideType(TILE_COLLIDE_TYPE::TOP_PLATFORM);
                 AddObject(pTop, GROUP_TYPE::GROUND);

                 // 2. 좌측 빗면
                 CGround* pLeft = new CGround();
                 pLeft->SetPos(vPos1);
                 pLeft->SetGroundType(groundType);
                 pLeft->SetScale(Vec2(vTileScale.x, vPos2.y - vPos1.y));
                 pLeft->SetCollideType(TILE_COLLIDE_TYPE::SLOPE_LEFT);
                 AddObject(pLeft, GROUP_TYPE::GROUND);

                 // 3. 우측 빗면
                 CGround* pRight = new CGround();
                 pRight->SetPos(Vec2(vPos2.x - vTileScale.x, vPos1.y));
                 pRight->SetGroundType(groundType);
                 pRight->SetScale(Vec2(vTileScale.x, vPos2.y - vPos1.y));
                 pRight->SetCollideType(TILE_COLLIDE_TYPE::SLOPE_RIGHT);
                 AddObject(pRight, GROUP_TYPE::GROUND);

                 // 4. 하단 지형
                 CGround* pBottom = new CGround();
                 pBottom->SetPos(Vec2(vPos1.x+vTileScale.x, vPos2.y - vTileScale.y));
                 pBottom->SetGroundType(groundType);
                 pBottom->SetScale(Vec2(vPos2.x - vPos1.x-(vTileScale.x*2), vTileScale.y));
                 pBottom->SetCollideType(TILE_COLLIDE_TYPE::BOT_PLATFORM);
                 AddObject(pBottom, GROUP_TYPE::GROUND);
             }
             else // 이동불가/데미지/즉사 지형
             {
                 CGround* pGround = new CGround();
                 pGround->SetPos(vPos1);
                 pGround->SetGroundType(groundType);
                 pGround->SetScale(vPos2 - vPos1);
                 pGround->SetCollideType(TILE_COLLIDE_TYPE::SOLID);
                 AddObject(pGround, GROUP_TYPE::GROUND);
             }
         }
    }
}




