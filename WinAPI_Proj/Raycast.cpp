#include "pch.h"
#include "Raycast.h"
#include "CCollider.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CKeyMgr.h"
#include "CCamera.h"
Raycast::Raycast()
	:onCollisionRay(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(5.f, 5.f));
}

Raycast::~Raycast()
{
}

void Raycast::Update()
{


	CollisionCheck();
	

}

void Raycast::Render(HDC _hdc)
{
	if (!targetPos.IsZero())
	{
		Vec2 renderPos = CCamera::GetInst()->GetRenderPos(targetPos);
		Rectangle(_hdc, renderPos.x - 5, renderPos.y - 5, renderPos.x +5, renderPos.y + 5);
	}
}



void Raycast::CollisionCheck()
{
	Vec2 mousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

	Vec2 dir = mousePos - GetPos();
	dir.Normalize();
	Vec2 checkPos = GetPos();


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const vector<GameObject*>& vecGround = pCurScene->GetGroupObject(GROUP_TYPE::GROUND);
	const vector<GameObject*>& vecMonster = pCurScene->GetGroupObject(GROUP_TYPE::MONSTER);

	onCollisionRay = nullptr;
	
	Vec2 offsetPos = Vec2(0.f, 0.f);
	targetPos = Vec2(0.f, 0.f);

	int i = 0;
	while (1)
	{
		offsetPos += dir * i*3;
		GetCollider()->SetOffsetPos(offsetPos);


		for (size_t j = 0; j < vecGround.size(); j++)
		{
			if (nullptr == vecGround[j]->GetCollider())
				continue;

			CCollider* collider = vecGround[j]->GetCollider();

			if (IsCollision(GetCollider(), collider))
			{
				onCollisionRay = collider;
				break;
			}
		}

		for (size_t j = 0; j < vecMonster.size(); j++)
		{
			if (nullptr == vecMonster[j]->GetCollider())
				continue;

			CCollider* collider = vecMonster[j]->GetCollider();

			if (IsCollision(GetCollider(), collider))
			{
				onCollisionRay = collider;
				break;
			}
		}


		if (onCollisionRay != nullptr)
		{
			Vec2 pos = GetPos();
			Vec2 ColPos = GetCollider()->GetOffsetPos();
			//targetpos가 플레이어와 가장 가까운 곳으로 찍히게 수정할 여지 있음
			
			targetPos = GetPos() + GetCollider()->GetOffsetPos();
			break;
		}

		Vec2 curPos = GetCollider()->GetOffsetPos() +GetPos();

		//레이 최대거리
		if ((curPos - GetPos()).Length() >= 600.f)
		{

			break;
		}
		
		i++;
	}





	GetCollider()->SetOffsetPos(Vec2(0.f,0.f));
}

bool Raycast::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetOffsetPos() + _pLeftCol->GetObj()->GetPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	return false;
}




