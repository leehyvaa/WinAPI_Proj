#include "pch.h"
#include "CUIMgr.h"

#include "CScene.h"
#include "CSceneMgr.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
}
void CUIMgr::Update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const vector<GameObject*>& vecUI = pCurScene->GetGroupObject(GROUP_TYPE::UI);

	bool bLbtnTap = KEY_TAP(KEY::LBUTTON);
	bool bLbtnAway = KEY_AWAY(KEY::LBUTTON);
		
	for (size_t i = 0; i < vecUI.size(); i++)
	{
		CUI* pUI = (CUI*)vecUI[i];
		

		//부모 UI 포함, 자식 UI들 중 타겟된 UI를 서치
		pUI = GetTargetedUI(pUI);

		if (nullptr != pUI)
		{
			pUI->MouseOn();

			if (bLbtnTap)
			{
				pUI->MouseLbtnDown();
				pUI->m_bLbtnDown = true;
			}
			else if (bLbtnAway)
			{
				pUI->MouseLbtnUp();

				if (pUI->m_bLbtnDown)
				{
					pUI->MouseLbtnClicked();
				}


				pUI->m_bLbtnDown = false;

			}
		}
	}
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
