#pragma once
#include "CScene.h"
#include "Object/Trigger/CTrigger.h" // CTrigger 헤더 포함
#include "Object/Ground/CSkylineCar.h"
#include <array>

class CTextUI;
class CBtnUI;
class CPanelUI;

enum TOOL_MODE
{
	TEXTURE_MODE,
	GROUND_MODE,
	TRIGGER_MODE,
	CAR_MODE,
	SPAWN_MODE,
};

class CScene_Tool : public CScene
{
private:
	CBtnUI* m_pTexUI;
	CPanelUI* m_pPanelUI;
	CTextUI* m_pModeText;  // 모드 표시용 텍스트 UI
	CTextUI* m_pHelpText;  // 설명서 텍스트 UI
	CTextUI* m_pHelpSubText; // 세부 설명서 텍스트 UI

	// 트리거 모드 관련 변수
	array<CTrigger*, 5> m_arrTriggers;
	int m_iCurrentTriggerIndex;
	int m_iTriggerAreaClickCount;
	int m_iTriggerAreaP1_TileIdx; 
	int m_iWallAreaClickCount; 
	int m_iWallAreaP1_TileIdx;
	MON_TYPE m_eCurrentMonsterType;
	vector<GameObject*> m_vecSampleMonsters;

	// Car 모드 관련 변수
	array<CSkylineCar*, 10> m_arrCars;
	int m_iCurrentCarIndex;


	wstring m_strCurTexFolder;
	vector<string> m_vecTile_list;

	Vec2 m_vTilePos;
	UINT m_iImgIndex;

	int m_iImgTileX;
	int m_iImgTileY;
	UINT m_iImgTileIdx;
	Vec2 m_vImgTilePos;

    bool m_eWriteTwo;
	bool m_bErase;
	bool m_bSecondTex;
	TOOL_MODE m_eToolMode;
	GROUND_TYPE m_eGroundType;
   
    int m_iLastBotRightTileIdx;
    int m_iLastTopLeftTileIdx;

 bool m_bDraggingClearArea;   // 클리어 영역 드래그 중인지
 
    // 모드별 설명 텍스트 저장용
    vector<wstring> m_textureHelp;
    vector<wstring> m_groundHelp;
    vector<wstring> m_triggerHelp; // 트리거 모드 설명 추가
    vector<wstring> m_carHelp;
    vector<wstring> m_spawnHelp;
    vector<wstring> m_commonHelp;
    
    // 마우스 위치 타일 인덱스 계산 함수
    bool CalculateTileIndex(int& iCol, int& iRow,int& iTileX);
    
public:

 virtual void Update();
 virtual void Enter();
 virtual void Exit();
 virtual void Render(ID2D1RenderTarget* _pRenderTarget);

public:
 void SetTileIdx();
 void SetTileUIIdx();
 void DrawSelectTile();

    
 void SaveTile(const wstring& _strFilePath);
 void SaveTileData();
 void LoadTileData();
 void LoadTile(const wstring& _strFilePath);

 void LoadTileTexUI(const wstring& folderPath);
 void ChangeTileTexUI();
 void NextTileUI();
 void PrevTileUI();
    
 void SettingSampleMonster(Vec2 pos, MON_TYPE eType, CTrigger* pOwnerTrigger);

 void SettingTopLeftGround();
    void SettingBotRightGround();

    // 스폰 및 클리어 위치 설정
    void SetPlayerSpawnPos();
    void SetSceneClearPos();

    // Update 함수에서 분리된 모드별 처리 함수들
    void UpdateTriggerMode();
    void UpdateCarMode();
    wstring UpdateSpawnMode();
    

	CScene_Tool();
	virtual ~CScene_Tool();
};
