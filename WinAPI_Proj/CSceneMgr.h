#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END]; //��� �� ���
	CScene* m_pCurScene; // ���� ��

public:
	void init();
	void Update();
	void Render(HDC _dc);
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetArrScene() { return *m_arrScene; }

private:
	void ChangeScene(SCENE_TYPE _eNext);
	friend class CEventMgr;
};

