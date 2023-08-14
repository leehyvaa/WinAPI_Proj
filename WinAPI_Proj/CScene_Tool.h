#pragma once
#include "CScene.h"


class CBtnUI;

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
	CBtnUI* m_pUI;
	vector<std::string> m_vecTile_list;

	Vec2 m_vTilePos;

	UINT m_iImgIndex;

	int m_iImgTileX;
	int m_iImgTileY;
	UINT m_iImgTileIdx;
	Vec2 m_vImgTilePos;

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

