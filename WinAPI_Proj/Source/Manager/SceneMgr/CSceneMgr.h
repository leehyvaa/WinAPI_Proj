#pragma once

#include <d2d1.h>

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)

private:
	CScene* m_arrScene[static_cast<UINT>(SCENE_TYPE::END)]; //모든 씬 목록
	CScene* m_pCurScene; // 현재 씬

public:
	void init();
	void Update();
	// void Render(HDC _dc);
	void RenderD2D(ID2D1RenderTarget* _pRenderTarget);
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetArrScene() { return *m_arrScene; }

private:
	void ChangeScene(SCENE_TYPE _eNext);
	friend class CEventMgr;
};

