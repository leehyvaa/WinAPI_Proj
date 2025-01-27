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
	//���� �����ӿ� ����ص� dead object���� ����
	for (size_t i = 0; i < m_vecDead.size(); i++)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();


	//Event ó��
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
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}	
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		//lParam : ������ ������Ʈ �ּ�
		//object�� dead ���·� ����
		//�������� ������Ʈ���� ��Ƶд�.
		GameObject* pDeadObj = (GameObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Cene Type
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

		//��Ŀ�� UI ����(���� Scene�� UI�� ����Ű�� �ֱ� ����
		CUIMgr::GetInst()->SetFocusedUI(nullptr);
	}break;
	
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		//lParam AI  , wParam NextType
		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eNextState = (MON_STATE)_eve.wParam;
		pAI->ChangeState(eNextState);


	}break;
	}
}
