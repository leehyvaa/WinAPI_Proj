#include "pch.h"
#include "CUIMgr.h"

#include "CScene.h"
#include "CSceneMgr.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	:m_pFocusedUI(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}
void CUIMgr::Update()
{
	//1.FocusedUI를 확인
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI)
		return;

	//2. FocusedUI 내에서 ,부모 UI 포함, 자식 UI들 중 타겟된 UI를 서치
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);

	bool bLbtnAway = KEY_AWAY(KEY::LBUTTON);
	bool bLbtnTap = KEY_TAP(KEY::LBUTTON);


	if (nullptr != pTargetUI)
	{
		pTargetUI->MouseOn();

		if (bLbtnTap)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLbtnUp();

			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLbtnClicked();
			}


			pTargetUI->m_bLbtnDown = false;

		}
	}



		

}

void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	//이미 포커싱 중인 경우 or 포커싱 해제 요청인 경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;



	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<GameObject*>& vecUI = pCurScene->GetUIGroup();

	vector<GameObject*>::iterator iter = vecUI.begin();


	for (; iter != vecUI.end(); iter++)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}

	}

	//벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);

}

CUI* CUIMgr::GetFocusedUI()
{

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<GameObject*>& vecUI = pCurScene->GetUIGroup();

	bool bLbtnTap = KEY_TAP(KEY::LBUTTON);
	
	//기존 포커싱 UI를 받아두고 변경되었을 경우 체인지
	CUI* pFocusedUI = m_pFocusedUI;

	if (!bLbtnTap)
	{
		return pFocusedUI;
	}


	//왼쪽버튼 Tap 이 발생했다는 전제 하
	vector<GameObject*>::iterator targetiter = vecUI.end();
	vector<GameObject*>::iterator iter =vecUI.begin() ;


	for (; iter != vecUI.end(); iter++)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
		
	}

	//이번에 Focus된 UI가 없다
	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetiter;

	//벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{

	bool bLbtnAway = KEY_AWAY(KEY::LBUTTON);



	//1. 부모 UI 포함, 모든 자식들을 검사한다.
	CUI* pTargetUI = nullptr;

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	while (!queue.empty())
	{
		//데이터 하나씩 꺼내기
		CUI* pUI = queue.front();
		queue.pop_front();

		//큐에서 꺼내온 UI가 TargetUI인지 확인
		//타겟 UI 중 더 우선순위가 높은 기준은 더 낮은 계층의 자식 UI다

		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}

			pTargetUI = pUI;
		}
		else
		{

			vecNoneTarget.push_back(pUI);
		}

		


		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); i++)
		{
			queue.push_back(vecChild[i]);
		}
	}

	
	
	//왼쪽버튼을 떼면, 눌렸던 체크를 다시 해제한다.
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); i++)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}


	//리턴이 null이면 아무 ui도 타겟되지 않은 것 
	return pTargetUI;
}
