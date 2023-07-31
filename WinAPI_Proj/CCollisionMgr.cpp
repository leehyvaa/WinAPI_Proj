#include "pch.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "GameObject.h"
#include "CCollider.h"
CCollisionMgr::CCollisionMgr()
{

}
CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::Update()
{
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; iRow++)
	{
		m_arrCheck[iRow];
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; iCol++)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}


void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	//GetGroupObject �Լ��� ���͸� ���۷����� �������ϱ�
	//�������� ���۷����� �޾ƾ��Ѵ�. ���۷����� �Ⱦ��� �׳�
	//���������� �������� �Ǿ������.
	const vector<GameObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<GameObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		//�浹ü�� �������� �ʴ� ���
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); j++)
		{
			//�浹ü�� ���ų�, �ڱ� �ڽŰ��� �浹�� ���
			if (nullptr == vecRight[j]->GetCollider() ||
				vecLeft[i] == vecRight[j])
				continue;

			if (IsCollision(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider()))
			{

			}
			else
			{

			}


		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	return false;
}



void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	//�� ���� ���� �׷� Ÿ���� ������ ū ���� ��(��Ʈ)�� ���
	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}


}
