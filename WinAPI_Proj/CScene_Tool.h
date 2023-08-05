#pragma once
#include "CScene.h"

class CUI;

class CScene_Tool : public CScene
{
private:
	CUI* m_pUI;

public:
	

	virtual void Update();
	virtual void Enter();
	virtual void Exit();

public:
	void SetTileIdx();

	void SaveTile(const wstring& _strFilePath);
	void SaveTileData();

	void LoadTileData();


	CScene_Tool();
	virtual ~CScene_Tool();
};

