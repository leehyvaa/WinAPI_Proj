#include "pch.h"
#include "CEventMgr.h"
#include "GameObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CUIMgr.h"
#include "AI.h"
CEventMgr::CEventMgr()
{

}
CEventMgr::~CEventMgr()
{

}

void CEventMgr::Update()
{
	//이전 프레임에 등록해둔 dead object들을 삭제
	for (size_t i = 0; i < m_vecDead.size(); i++)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();


	//Event 처리
	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		Excute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		//lParam : Objcet Adress
		//wParam : Group Type
		GameObject* pNewObj = (GameObject*)_eve.lParam;
		GROUP_TYPE eType = static_cast<GROUP_TYPE>(_eve.wParam);
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}	
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		//lParam : 삭제될 오브젝트 주소
		//object를 dead 상태로 변경
		//삭제예정 오브젝트들을 모아둔다.
		GameObject* pDeadObj = (GameObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Cene Type
		CSceneMgr::GetInst()->ChangeScene(static_cast<SCENE_TYPE>(_eve.lParam));

		//포커스 UI 해제(이전 Scene의 UI를 가리키고 있기 때문
		CUIMgr::GetInst()->SetFocusedUI(nullptr);
	}break;
	
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		//lParam AI  , wParam NextType
		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eNextState = static_cast<MON_STATE>(_eve.wParam);
		pAI->ChangeState(eNextState);


	}break;
	}
}
