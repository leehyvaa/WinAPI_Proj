#pragma once
#include "CScene.h"

class CTextUI;
class CBtnUI;
class CPanelUI;

enum TOOL_MODE
{
	TEXTURE_MODE,
	GROUND_MODE,
	PREFAB_MODE,
	TRIGGER_MODE,
};



class CScene_Tool : public CScene
{
private:
	CBtnUI* m_pTexUI;
	CPanelUI* m_pPanelUI;
	CTextUI* m_pModeText;  // 모드 표시용 텍스트 UI
	CTextUI* m_pHelpText;  // 설명서 텍스트 UI
    CTextUI* m_pHelpSubText; // 세부 설명서 텍스트 UI


	vector<std::string> m_vecTile_list;

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
    
    // 모드별 설명 텍스트 저장용
    vector<wstring> m_textureHelp;
    vector<wstring> m_groundHelp;
    vector<wstring> m_commonHelp;
    
    // 마우스 위치 타일 인덱스 계산 함수
    bool CalculateTileIndex(int& iCol, int& iRow,int& iTileX);
    
public:
	
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

public:
	void SetTileIdx();
	void SetTileUIIdx();
	void DrawSelectTile();

    
	void SaveTile(const wstring& _strFilePath);
	void SaveTileData();
	void LoadTileData();

	void LoadTileTexUI();
	void ChangeTileTexUI();
	void NextTileUI();
	void PrevTileUI();


	void SettingTopLeftGround();
    void SettingBotRightGround();

    
	void SaveBmp();
    void UpdateTextBox();




	CScene_Tool();
	virtual ~CScene_Tool();
};

