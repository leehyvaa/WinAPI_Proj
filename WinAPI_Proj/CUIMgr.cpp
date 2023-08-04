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
		

		//�θ� UI ����, �ڽ� UI�� �� Ÿ�ٵ� UI�� ��ġ
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



	//1. �θ� UI ����, ��� �ڽĵ��� �˻��Ѵ�.
	CUI* pTargetUI = nullptr;

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	while (!queue.empty())
	{
		//������ �ϳ��� ������
		CUI* pUI = queue.front();
		queue.pop_front();

		//ť���� ������ UI�� TargetUI���� Ȯ��
		//Ÿ�� UI �� �� �켱������ ���� ������ �� ���� ������ �ڽ� UI��

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

	
	
	//���ʹ�ư�� ����, ���ȴ� üũ�� �ٽ� �����Ѵ�.
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); i++)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}


	//������ null�̸� �ƹ� ui�� Ÿ�ٵ��� ���� �� 
	return pTargetUI;
}
