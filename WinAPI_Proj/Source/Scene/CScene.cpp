#include "pch.h"
#include "CScene.h"

#include "CAnimator.h"
#include "GameObject.h"
#include "CTile.h"
#include "CBackGround.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CCore.h"
#include "CGravity.h"
#include "CKeyMgr.h"
#include "CGround.h"
#include "CObjectPool.h"
#include "CRigidBody.h"
#include "CTextUI.h"
#include "CUI.h"
#include "CTimeMgr.h"
#include "SPlayer.h"
#include "Object/Trigger/CTrigger.h" // ADDED

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
    ,m_pPoolDebugText(nullptr)
    ,m_vPlayerSpawnPos(Vec2(0, 0))
    ,m_vSceneClearStartPos(Vec2(0, 0))
    ,m_vSceneClearEndPos(Vec2(0, 0))
    ,m_bPlayerSpawnSet(false)
    ,m_bSceneClearSet(false)
{


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
void CScene::Enter()
{
    if (m_pPlayerText == nullptr) {
        m_pPlayerText = new CTextUI();
        m_pPlayerText->SetWorldPos(Vec2(750, 0));
        m_pPlayerText->SetAlign(CTextUI::TEXT_ALIGN::CENTER);
        m_pPlayerText->SetLineSpace(5);
        m_pPlayerText->SetVisibleBox(false);
        m_pPlayerText->SetFontSize(20);
        AddObject(m_pPlayerText, GROUP_TYPE::UI);
    }

    // 디버그 설정 초기화
    bDrawGrid = false;
    bDrawCollider = false;
    bDrawGroundType = false;
    bDrawCompleteGround = false;
    bDrawOutWindow = false;
    m_pPlayerText->SetActive(true);

    // 풀 디버그 텍스트 초기화
    if (m_pPoolDebugText == nullptr) {
        m_pPoolDebugText = new CTextUI();
        m_pPoolDebugText->SetWorldPos(Vec2(10, 10)); // 왼쪽 상단에 위치
        m_pPoolDebugText->SetAlign(CTextUI::TEXT_ALIGN::LEFT);
        m_pPoolDebugText->SetLineSpace(5);
        m_pPoolDebugText->SetVisibleBox(true);
        m_pPoolDebugText->SetFontSize(16);
        m_pPoolDebugText->SetActive(false); // 기본적으로 숨김
        AddObject(m_pPoolDebugText, GROUP_TYPE::UI);
    }
}

void CScene::Exit()
{
	m_vPlayerSpawnPos = Vec2(0.f, 0.f);
	m_vSceneClearStartPos = Vec2(0.f, 0.f);	m_vSceneClearEndPos = Vec2(0.f, 0.f);
	m_bSceneClearSet = false;
	m_bPlayerSpawnSet = false;
    // 씬 종료 시 오브젝트 풀 제외한 모든 씬 내의 오브젝트를 삭제
    DeleteAll();
}


void CScene::Update()
{
    CTimeMgr::StartTimer(L"Scene_Update");




    


    // 씬 내의 오브젝트들 Update
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			if(!m_arrObj[i][j]->IsDead() && m_arrObj[i][j]->IsActive())
			{
				m_arrObj[i][j]->Update();
			}
		}
	}
 

    // 맵 그리드 확인
    if (KEY_TAP(KEY::F6))
        bDrawGrid= !bDrawGrid;
    // 그라운드 타입 디버깅 
	if (KEY_TAP(KEY::F7))
		bDrawGroundType = !bDrawGroundType;
    // 그라운드 완성 처리 디버깅
    if (KEY_TAP(KEY::F8))
        bDrawCompleteGround = !bDrawCompleteGround;
    // 콜라이더 디버깅
    if (KEY_TAP(KEY::F5))
        bDrawCollider = !bDrawCollider;
    // 오브젝트 풀 내의 오브젝트 활성화 여부 디버깅
    if (KEY_TAP(KEY::O))
        TogglePoolDebugDisplay();
	if (KEY_TAP(KEY::F12))
		bDrawOutWindow = !bDrawOutWindow;
    // 플레이어 정보 UI 디버깅 토글
    if (KEY_TAP(KEY::C))
        m_pPlayerText->SetActive(!m_pPlayerText->IsActive());


    // 풀 디버그 UI가 활성화된 경우 정보 업데이트
    if (m_pPoolDebugText && m_pPoolDebugText->IsActive())
    {
        UpdatePoolDebugInfo();
    }
    
    // 플레이어 정보 UI 업데이트
    UpdateDebugUI();
    
    CTimeMgr::EndTimer(L"Scene_Update");
}

void CScene::FinalUpdate()
{
    CTimeMgr::StartTimer(L"Scene_FinalUpdate");
    
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
		    if(!m_arrObj[i][j]->IsDead() && m_arrObj[i][j]->IsActive())
			    m_arrObj[i][j]->FinalUpdate();
		}
	}

	   // Dead 상태인 오브젝트를 제거
	   for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
	   {
	       vector<GameObject*>& vecObjects = m_arrObj[i];
	       for (int j = (int)vecObjects.size() - 1; j >= 0; --j)
	       {
	           if (vecObjects[j]->IsDead())
	           {
	               vecObjects.erase(vecObjects.begin() + j);
	           }
	       }
	   }
	CTimeMgr::EndTimer(L"Scene_FinalUpdate");
}


void CScene::Render(ID2D1RenderTarget* _pRenderTarget)
{
    CTimeMgr::StartTimer(L"Scene_D2D_Render");
    
	if (!_pRenderTarget)
		return;

    
	// 배경 렌더링 (가장 먼저)
	if (backGround && backGround->IsActive())
	{
		backGround->Render(_pRenderTarget);
	}

	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); i++)
	{
		// 타일 렌더링
		if (static_cast<UINT>(GROUP_TYPE::TILE) == i && !bDrawOutWindow)
		{
			RenderTile(_pRenderTarget);
			continue;
		}

		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			GameObject* pObj = m_arrObj[i][j];
			if (pObj && !pObj->IsDead() && pObj->IsActive())
			{
				if (static_cast<UINT>(GROUP_TYPE::UI) == i)
				{
					CUI* pUI = dynamic_cast<CUI*>(pObj);
					if (pUI)
						pUI->Render(_pRenderTarget);
				}
				else if (static_cast<UINT>(GROUP_TYPE::HOOK) == i)
					pObj->Render(_pRenderTarget);
			    else if (static_cast<UINT>(GROUP_TYPE::GROUND) == i)
			    {
					pObj->Render(_pRenderTarget);
					pObj->Component_Render(_pRenderTarget); // 애니메이터 렌더링 추가
			    }
			    else if (static_cast<UINT>(GROUP_TYPE::TRIGGER) == i)
					pObj->Render(_pRenderTarget);
				// 다른 그룹은 Animator 렌더링
				else if (pObj->GetAnimator())
					pObj->GetAnimator()->Render(_pRenderTarget);
			    
				// 콜라이더 디버그 렌더링 (F5 키로 토글)
				if (bDrawCollider && pObj->GetCollider())
				{
					pObj->GetCollider()->Render(_pRenderTarget);
				}
			}
		}
    }
    
    CTimeMgr::EndTimer(L"Scene_D2D_Render");

    // P 키 - Direct2D 프로파일링 출력
    if (KEY_HOLD(KEY::P))
    {
		CTimeMgr::RenderProfileData(_pRenderTarget, 10);
		
		// 프로파일링 출력 후에만 리셋
		CTimeMgr::ResetProfileData();
    }
}


void CScene::RenderTile(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

    if (vecTile.empty())
        return;

    // 기존 모드 세팅 저장
    D2D1_ANTIALIAS_MODE oldAliasMode = _pRenderTarget->GetAntialiasMode();

    // 안티앨리어싱 비활성화
    _pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

    // 그리드 그리기용 브러시 생성
    static ID2D1SolidColorBrush* s_pGridBrush = nullptr;
    if (bDrawGrid && !s_pGridBrush)
    {
        _pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White, 0.3f),
            &s_pGridBrush
        );
    }

    Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
    Vec2 vResolution = CCore::GetInst()->GetResolution();

    Vec2 vLeftTop = vCamLook - vResolution / 2.f;

    int iTileSize = TILE_SIZE;

    int iLTCol = static_cast<int>(vLeftTop.x) / iTileSize;
    int iLTRow = static_cast<int>(vLeftTop.y) / iTileSize;

    int iClientWidth = (static_cast<int>(vResolution.x) / iTileSize) + 2;
    int iClientHeight = (static_cast<int>(vResolution.y) / iTileSize) + 2;

    // 지형 완성선용 브러시 생성
    static ID2D1SolidColorBrush* s_pCompleteGroundBrush = nullptr;
    if (bDrawCompleteGround && !s_pCompleteGroundBrush)
    {
        _pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White, 1.0f), 
            &s_pCompleteGroundBrush
        );
    }

    for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); iCurRow++)
    {
        for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); iCurCol++)
        {
            if (iCurCol < 0 || m_iTileX <= static_cast<UINT>(iCurCol) ||
                iCurRow < 0 || m_iTileY <= static_cast<UINT>(iCurRow))
            {
                continue;
            }

            int iIdx = (m_iTileX * iCurRow) + iCurCol;

            if (iIdx >= 0 && iIdx < static_cast<int>(vecTile.size()))
            {
                CTile* pTile = static_cast<CTile*>(vecTile[iIdx]);
                if (pTile && !pTile->IsDead() && pTile->IsActive())
                {
                    // 그리드 그리기
                    if (bDrawGrid && s_pGridBrush)
                    {
                        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pTile->GetWorldPos());
                        Vec2 vScale = pTile->GetScale();

                        D2D1_RECT_F rect = D2D1::RectF(
                            vRenderPos.x,
                            vRenderPos.y,
                            vRenderPos.x + vScale.x,
                            vRenderPos.y + vScale.y
                        );

                        _pRenderTarget->DrawRectangle(rect, s_pGridBrush, 1.0f);
                    }

                    // 타일 렌더링
                    pTile->Render(_pRenderTarget);

                   if (bDrawCompleteGround && s_pCompleteGroundBrush && pTile->GetGroundType() != GROUND_TYPE::NONE && pTile->GetVertexPosition() == VERTEX_POSITION::TOP_LEFT)
                    {
                        int botIdx = pTile->GetBotRightTileIdx();
                        if (botIdx != -1 && botIdx < static_cast<int>(vecTile.size()))
                        {
                            // 지형 타입에 따라 색상 결정
                            D2D1_COLOR_F color;
                            GROUND_TYPE groundType = pTile->GetGroundType();

                            if (groundType == GROUND_TYPE::NORMAL)
                                color = D2D1::ColorF(D2D1::ColorF::Blue);
                            else if (groundType == GROUND_TYPE::UNWALKABLE)
                                color = D2D1::ColorF(D2D1::ColorF::Purple);
                            else if (groundType == GROUND_TYPE::DAMAGEZONE)
                                color = D2D1::ColorF(D2D1::ColorF::Orange);
                            else if (groundType == GROUND_TYPE::DEADZONE)
                                color = D2D1::ColorF(D2D1::ColorF::Red);
                            else 
                                color = D2D1::ColorF(D2D1::ColorF::White);
                            
                            s_pCompleteGroundBrush->SetColor(&color);
                            
                            // 사각형 좌표 계산 및 그리기
                            Vec2 vStartPos = CCamera::GetInst()->GetRenderPos(pTile->GetWorldPos());
                            CTile* pBotRightTile = static_cast<CTile*>(vecTile[botIdx]);
                            Vec2 vEndPos = CCamera::GetInst()->GetRenderPos(pBotRightTile->GetWorldPos());
                            Vec2 vTileScale = pBotRightTile->GetScale();

                            float padding = 2.0f;
                            float lineThickness = 2.0f;

                            D2D1_RECT_F outlineRect = D2D1::RectF(
                                vStartPos.x - padding,
                                vStartPos.y - padding,
                                vEndPos.x + vTileScale.x + padding,
                                vEndPos.y + vTileScale.y + padding
                            );

                            _pRenderTarget->DrawRectangle(
                                outlineRect,
                                s_pCompleteGroundBrush,
                                lineThickness
                            );
                        }
                    }
                }
            }
        }
    }

    // 애니메이션을 위해 안티앨리어싱 원상복구
    _pRenderTarget->SetAntialiasMode(oldAliasMode);
}


void CScene::AddObject(GameObject* _pObj, GROUP_TYPE _eType)
{
    auto& vec = m_arrObj[static_cast<UINT>(_eType)];
    if (std::find(vec.begin(), vec.end(), _pObj) != vec.end())
        return;
    
    vec.push_back(_pObj);
}


// 지정된 그룹의 오브젝트들을 삭제
void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
    vector<GameObject*>& vecObjects = m_arrObj[static_cast<UINT>(_eTarget)];
    
    for (size_t i = 0; i < vecObjects.size(); ++i)
    {
        if (vecObjects[i] != nullptr)
        {
            // 풀에서 관리되지 않는 객체만 delete
            if (!vecObjects[i]->IsManagedByPool())
            {
                delete vecObjects[i];
            }
        }
    }

    // 벡터 자체를 비움 (포인터들을 제거)
    vecObjects.clear();
}


GameObject* CScene::FindObjectByName(const wstring& _strName)
{
    for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
    {
        for (GameObject* pObj : m_arrObj[i])
        {
            if (pObj && pObj->GetName() == _strName)
            {
                return pObj;
            }
        }
    }
    return nullptr;
}
void CScene::DeleteAll()
{
    m_pPlayerText = nullptr;
    m_pPoolDebugText = nullptr; 
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
void CScene::LoadTile(const wstring& _strFilePath)
{
	//커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strFilePath.c_str(), L"r");
    if(!pFile) return;

    // 맵 파일 로드 시 UI와 배경을 제외한 모든 게임 월드 오브젝트를 삭제합니다.
    // DeleteAll()은 툴씬의 UI까지 삭제하여 크래시를 유발하므로 사용하지 않습니다.
    for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::UI); ++i)
    {
        GROUP_TYPE eType = static_cast<GROUP_TYPE>(i);
        if (eType != GROUP_TYPE::BACKGROUND)
        {
            DeleteGroup(eType);
        }
    }
    m_pPlayer = nullptr;

    char szBuffer[256] = {};

    // 파일 파싱을 라인 단위로 변경하여 안정성 향상
    while (true)
    {
        FScanf(szBuffer, pFile);
        if (feof(pFile)) break;
        if (strlen(szBuffer) == 0) continue;

        if (strcmp(szBuffer, "[TileCount]") == 0)
        {
            UINT xCount = 0, yCount = 0;
            FScanf(szBuffer, pFile); sscanf_s(szBuffer, "%u", &xCount);
            FScanf(szBuffer, pFile); sscanf_s(szBuffer, "%u", &yCount);
            CreateTile(xCount, yCount);

            const vector<GameObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
            for (size_t i = 0; i < vecTile.size(); ++i) {
                static_cast<CTile*>(vecTile[i])->Load(pFile);
            }
        }
        else if (strcmp(szBuffer, "[SpawnData]") == 0)
        {
            FScanf(szBuffer, pFile); // [PlayerSpawn]
            float x, y;
            int isSet;
            FScanf(szBuffer, pFile); sscanf_s(szBuffer, "%f %f %d", &x, &y, &isSet);
            m_vPlayerSpawnPos = Vec2(x, y);
            m_bPlayerSpawnSet = (isSet != 0);

            FScanf(szBuffer, pFile); // [SceneClear]
            float startX, startY, endX, endY;
            FScanf(szBuffer, pFile); sscanf_s(szBuffer, "%f %f %f %f %d", &startX, &startY, &endX, &endY, &isSet);
            m_vSceneClearStartPos = Vec2(startX, startY);
            m_vSceneClearEndPos = Vec2(endX, endY);
            m_bSceneClearSet = (isSet != 0);
        }
        else if (strcmp(szBuffer, "[TriggerCount]") == 0)
        {
            int triggerCount = 0;
            FScanf(szBuffer, pFile);
            sscanf_s(szBuffer, "%d", &triggerCount);

            for (int i = 0; i < triggerCount; ++i)
            {
                CTrigger* pTrigger = new CTrigger();
                try {
                    pTrigger->Load(pFile);
                    AddObject(pTrigger, GROUP_TYPE::TRIGGER);
                }
                catch (...) {
                    delete pTrigger;
                }
            }
        }
    }


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

    //CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\Prologue_Tileset32.png");

    for (UINT i = 0; i < _iYCount; i++)
    {
        for (UINT j = 0; j < _iXCount; j++)
        {
            CTile* pTile = new CTile();

            pTile->SetWorldPos(Vec2(static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE)));
            //pTile->SetTexture(pTileTex);
            AddObject(pTile, GROUP_TYPE::TILE);
        }
    }
}


// 지형의 맨 왼쪽 위 꼭짓점과 오른쪽 아래 꼭짓점의 위치를 받아와서 사각형 지형을 생성
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
             Vec2 vPos1 = pTile->GetWorldPos();
             int iBotRightIdx = pTile->GetBotRightTileIdx();
             if (iBotRightIdx < 0 || iBotRightIdx >= (int)vecTile.size()) continue;

             Vec2 vPos2 = vecTile[iBotRightIdx]->GetWorldPos() + vecTile[iBotRightIdx]->GetScale();
             GROUND_TYPE groundType = pTile->GetGroundType();
             Vec2 vTileScale = pTile->GetScale();
            
             if (groundType == GROUND_TYPE::NORMAL)
             {
                 CGround* pGround = new CGround();
                 pGround->SetWorldPos(vPos1);
                 pGround->SetGroundType(groundType);
                 pGround->SetScale(vPos2 - vPos1);
                 pGround->SetCollideType(TILE_COLLIDE_TYPE::SOLID);
                 AddObject(pGround, GROUP_TYPE::GROUND);
                 
             }
             else // 이동불가/데미지/즉사 지형
             {
                 CGround* pGround = new CGround();
                 pGround->SetWorldPos(vPos1);
                 pGround->SetGroundType(groundType);
                 pGround->SetScale(vPos2 - vPos1);
                 pGround->SetCollideType(TILE_COLLIDE_TYPE::SOLID);
                 AddObject(pGround, GROUP_TYPE::GROUND);
             }
         }
    }
}

// 오브젝트 풀 디버그 UI
void CScene::UpdatePoolDebugInfo()
{
    if (!m_pPoolDebugText)
        return;
        
    m_pPoolDebugText->ClearLines();
    
    // 오브젝트 풀 정보 수집
    map<wstring, vector<GameObject*>>& pools = CObjectPool::GetInst()->GetPools();
    

    m_pPoolDebugText->AddLine(L"=== OBJECT POOL ===");
    
    if (pools.empty())
    {
        m_pPoolDebugText->AddLine(L"No active");
        return;
    }
    
    // 활성/비활성 객체 수 계산해서 표시
    for (const auto& pool : pools)
    {
        
        int totalCount = (int)pool.second.size();
        int activeCount = 0;
        
        for (GameObject* obj : pool.second)
            if (obj->IsActive())
                activeCount++;
        
        
        // 풀 정보 문자열 생성
        wstring poolInfo = pool.first + L": " + 
                         L"Active: " + to_wstring(activeCount) + L", " +
                         L"Total: " + to_wstring(totalCount);
        
        m_pPoolDebugText->AddLine(poolInfo);
    }

    
    m_pPoolDebugText->AddLine(L"");
    m_pPoolDebugText->AddLine(L"Press O");
}

void CScene::TogglePoolDebugDisplay()
{
    if (m_pPoolDebugText)
    {
        m_pPoolDebugText->SetActive(!m_pPoolDebugText->IsActive());
    }
}

void CScene::UpdateDebugUI()
{
    // 디버깅용 UI 업데이트
    if (m_pPlayerText != nullptr)
    {
        if(m_pPlayerText) m_pPlayerText->ClearLines();
        const vector<GameObject*>& vecPlayer = GetGroupObject(GROUP_TYPE::PLAYER);
        SPlayer* player;
        if (vecPlayer.size() > 0)
        {
            player = static_cast<SPlayer*>(vecPlayer[0]);
            static wstring state;
            static wstring animation;
            static wstring ongravity;
            static wstring hp;
            static wstring climb;
            static wstring velocityX;
            static wstring velocityY;
            static wstring MoveEnergy;
            static wstring PosEnergy;
            static wstring wireDistance;
            static wstring maxDistance;
            
            if (player->GetGravity()->IsApplyGravity())
                ongravity = L"On Gravity";
            else
                ongravity = L"Off Gravity";
            
            if (player->IsWallClimbing())
                climb = L"Climb";
            else
                climb = L"Not Climb";

    

            
            hp = to_wstring(player->GetCurHP());


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
            case PLAYER_STATE::SHOT:
                state = L"SHOT";
                break;
            case PLAYER_STATE::EXECUTE:
                state = L"EXECUTE";
                break;
            case PLAYER_STATE::DAMAGED:
                state = L"DAMAGED";
                break;
            case PLAYER_STATE::DEAD:
                state = L"DEAD";
                break;
            }
            velocityX = to_wstring(player->GetRigidBody()->GetVelocity().x);
            velocityY = to_wstring(player->GetRigidBody()->GetVelocity().y);
            MoveEnergy=L"MoveEnergy : ";
            MoveEnergy += to_wstring(player->GetMoveEnergy());
            PosEnergy=L"PosEnergy : ";
            PosEnergy += to_wstring(player->GetPosEnergy());
            wireDistance = L"Wire Range : ";
            wireDistance += to_wstring(player->GetWireRange());
            maxDistance = L"Wire Distance : ";
            maxDistance += to_wstring(player->GetHookDistance());
            
            vector<wstring> Texts =
            {
                state,
                ongravity,
                climb,
                hp,
                velocityX,
                velocityY,
                MoveEnergy,
                PosEnergy,
                wireDistance,
                maxDistance
            };

            if (m_pPlayerText) m_pPlayerText->AddLines(Texts);
        }
    }
}

void CScene::SetPlayerSpawnPos(const Vec2& pos)
{
	m_vPlayerSpawnPos = pos;
	m_bPlayerSpawnSet = true;
}

void CScene::SetSceneClearPos(const Vec2& startPos, const Vec2& endPos)
{
	m_vSceneClearStartPos = startPos;
	m_vSceneClearEndPos = endPos;
	m_bSceneClearSet = true;
}


