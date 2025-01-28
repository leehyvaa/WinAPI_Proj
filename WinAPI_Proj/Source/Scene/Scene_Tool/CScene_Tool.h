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



	vector<std::string> m_vecTile_list;

	Vec2 m_vTilePos;

	UINT m_iImgIndex;

	int m_iImgTileX;
	int m_iImgTileY;
	UINT m_iImgTileIdx;
	Vec2 m_vImgTilePos;
	bool m_bErase;
	bool m_bSecondTex;
	TOOL_MODE toolMode;
	GROUND_TYPE groundType;
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


	void CreateGround();
	void SaveBmp();





	CScene_Tool();
	virtual ~CScene_Tool();
};

