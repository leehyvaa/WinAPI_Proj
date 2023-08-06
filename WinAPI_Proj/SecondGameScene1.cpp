#include "pch.h"
#include "SecondGameScene1.h"

#include "GameObject.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CMonster.h"
#include "SPlayer.h"

#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"

SecondGameScene1::SecondGameScene1()
	:m_bUseForce(false)
	,m_fForceRadius(500)
	,m_fCurRadius(0)
	,m_fForce(500.f)
{
}

SecondGameScene1::~SecondGameScene1()
{
}


void SecondGameScene1::Update()
{
	if (KEY_HOLD(KEY::LBUTTON))
	{
		m_bUseForce = true;
		CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		const vector<GameObject*>& vecObj = GetGroupObject((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); j++)
		{
			if (!vecObj[j]->IsDead())
			{
				if (m_bUseForce && vecObj[j]->GetRigidBody())
				{
					Vec2 vDiff =  vecObj[j]->GetPos() - m_vForcePos;
					float fLen = vDiff.Length();
					
					if (vDiff.Length() < m_fForceRadius)
					{
						float fRadio = 1.f - (fLen / m_fForceRadius);
						float fForce = m_fForce * fRadio;
						vecObj[j]->GetRigidBody()->AddForce(vDiff.Normalize() * fForce);
						
					}
				
				}
				vecObj[j]->Update();
			}
		}
	}


	//�θ�Ŭ������ update�� virtual�� �ƴϱ� ������
	//CScene::Update();



	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	/*if (KEY_TAP(KEY::LBUTTON))
	{
		Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		CCamera::GetInst()->SetLookAt(vLookAt);
	}*/

	
}

void SecondGameScene1::Render(HDC _dc)
{
	CScene::Render(_dc);


	if (!m_bUseForce)
		return;

	SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);
	SelectGDI gdi2(_dc, PEN_TYPE::BLUE);

	m_fCurRadius += m_fForceRadius * 2.f * fDT;
	if (m_fCurRadius > m_fForceRadius)
	{
		m_fCurRadius = 0.f;
	}

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);

	Ellipse(_dc, vRenderPos.x - m_fCurRadius
		, vRenderPos.y - m_fCurRadius
		, vRenderPos.x + m_fCurRadius
		, vRenderPos.y + m_fCurRadius);


}

void SecondGameScene1::Enter()
{
	//�� ���� ��Ȳ������ AddObject �ص� ������ 
	//Update ��Ȳ������ CreateObject������ �̺�Ʈ�� ������Ʈ ����
	Vec2 vResolution = CCore::GetInst()->GetResolution();



	//������Ʈ �߰�
	/*GObject* pObj = new GObject;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::DEFAULT);*/
	
	GameObject* player = new SPlayer();
	player->SetPos(Vec2(400.f,800.f));
	player->SetScale(Vec2(100.f, 100.f));
	AddObject(player, GROUP_TYPE::PLAYER);
	RegisterPlayer(player);

	//Ŭ���Լ� ���� ���� ������Ʈ ���� ���� �÷��̾ �����Ѵٸ� �Ʒ��� ���� ���
	/*GameObject* pOtherPlayer = new SPlayer(*(SPlayer*)player);
	pOtherPlayer->SetPos(Vec2(200.f, 200.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	//CCamera::GetInst()->SetTarget(player);



	//���� ��ġ �⺻��� 1
	/*int iMonCount = 1;
	CMonster* pMonsterObj = nullptr;

	AI* pAI = new AI;
	pAI->AddState(new CIdleState);
	pAI->AddState(new CTraceState);

	for (size_t i = 0; i < iMonCount; i++)
	{
		pMonsterObj = new CMonster();
		pMonsterObj->SetName(L"Monster");
		pMonsterObj->SetScale(Vec2(100.f, 100.f));
		pMonsterObj->SetPos(vResolution /2.f - Vec2(0.f, 300.f));
		pMonsterObj->SetAI(pAI);

		AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	}*/

	


	//Ŭ�� �Լ��� ����� �� ������ ���� ����
	/*GameObject* pOtherMonster = pMonsterObj->Clone();
	pOtherMonster->SetPos(Vec2(500.f, 400.f));
	AddObject(pOtherMonster, GROUP_TYPE::MONSTER);*/


	//������ ���� ��ġ
	CMonster* pMon = CMonPrefab::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	CreateObject((GameObject*)pMon, GROUP_TYPE::MONSTER);
	//AddObject(pMon, GROUP_TYPE::MONSTER);


	//Ÿ�� �ε�
	//LoadTile(L"Tile\\start.tile");


	//�׷찣 �浹 üũ
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);


	//ī�޶� ��ġ ����
	CCamera::GetInst()->SetLookAt(vResolution/2.f);

	CCamera::GetInst()->FadeOut(1.f);
	CCamera::GetInst()->FadeIn(1.f);

}

void SecondGameScene1::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}

void SecondGameScene1::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

}

