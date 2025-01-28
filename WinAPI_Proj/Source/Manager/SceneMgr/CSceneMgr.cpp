#include "pch.h"
#include "CSceneMgr.h"
#include "Scene_Stage_01.h"
#include "CScene_Tool.h"
#include "CEventMgr.h"
#include "CScene_Title.h"
CSceneMgr::CSceneMgr()
	:m_pCurScene(nullptr)
	, m_arrScene{}
{

}
CSceneMgr::~CSceneMgr()
{
	//씬 전부 삭제
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void CSceneMgr::init()
{
	//Scene생성
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Title;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Title Scene");

	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");

	m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new Scene_Stage_01;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01]->SetName(L"Stage_01");


	

	//m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_STAGE_01;
	//m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_STAGE_02;


	//현재 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
	m_pCurScene->Enter();
}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
	m_pCurScene->FinalUpdate();
}

void CSceneMgr::Render(HDC _dc)
{
	m_pCurScene->Render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();
	
	m_pCurScene = m_arrScene[(UINT)_eNext];
	m_pCurScene->Enter();
}
