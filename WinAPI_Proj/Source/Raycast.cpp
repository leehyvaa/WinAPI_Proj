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
	GetCollider()->SetScale(Vec2(10.f, 10.f));
    m_bRender=false; // 현재 충돌지점 미출력으로 설정
}

Raycast::~Raycast()
{
}

void Raycast::Update()
{
	CollisionCheck();
	
}

void Raycast::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;
		
    // RayCast 충돌 위치 출력
	if (!targetPos.IsZero() && m_bRender)
	{
		Vec2 renderPos = CCamera::GetInst()->GetRenderPos(targetPos);
		
		// Direct2D로 사각형 렌더링
		static ID2D1SolidColorBrush* s_pBrush = nullptr;
		if (!s_pBrush)
		{
			_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &s_pBrush);
		}
		
		if (s_pBrush)
		{
			D2D1_RECT_F rect = D2D1::RectF(
				renderPos.x - 5.0f,
				renderPos.y - 5.0f,
				renderPos.x + 5.0f,
				renderPos.y + 5.0f
			);
			_pRenderTarget->DrawRectangle(rect, s_pBrush);
		}
	}
	
	GameObject::RenderD2D(_pRenderTarget);
}



void Raycast::CollisionCheck()
{
	Vec2 mousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

	Vec2 dir = mousePos - GetWorldPos();
	dir.Normalize();


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const vector<GameObject*>& vecGround = pCurScene->GetGroupObject(GROUP_TYPE::GROUND);
	const vector<GameObject*>& vecMonster = pCurScene->GetGroupObject(GROUP_TYPE::MONSTER);

	onCollisionRay = nullptr;
	
	Vec2 offsetPos = Vec2(0.f, 0.f);
	targetPos = Vec2(0.f, 0.f);

	int i = 0;
	while (true)
	{
		offsetPos += dir * i*3;
		GetCollider()->SetOffsetPos(offsetPos);

	    // Ground중 현재 Ray에 충돌한 지형이 있는지 체크
		for (size_t j = 0; j < vecGround.size(); j++)
		{
			if (nullptr == vecGround[j]->GetCollider())
				continue;

			CCollider* collider = vecGround[j]->GetCollider();

			if (IsCollision(GetCollider(), collider))
			{
			    // Ray에 부딪힌 물체의 충돌체를 저장
				onCollisionRay = collider;
				break;
			}
		}

	    // Monster중 현재 Ray에 충돌한 몬스터가 있는지 체크
		for (size_t j = 0; j < vecMonster.size(); j++)
		{
			if (nullptr == vecMonster[j]->GetCollider())
				continue;

			CCollider* collider = vecMonster[j]->GetCollider();

			if (IsCollision(GetCollider(), collider))
			{
			    // Ray에 부딪힌 물체의 충돌체를 저장
				onCollisionRay = collider;
				break;
			}
		}

        // Ray에 부딪힌 물체가 있으면 while문 탈출
		if (onCollisionRay != nullptr)
		{
			Vec2 pos = GetWorldPos();
			Vec2 ColPos = GetCollider()->GetOffsetPos();
			//targetpos가 플레이어와 가장 가까운 곳으로 찍히게 수정할 여지 있음

		    // 타겟과 Ray가 충돌한 지점의 위치를 저장
			targetPos = GetWorldPos() + GetCollider()->GetOffsetPos();
			break;
		}

		Vec2 curPos = GetCollider()->GetOffsetPos() +GetWorldPos();

		//레이 최대거리에 도달시 while문 탈출
		if ((curPos - GetWorldPos()).Length() >= m_fMaxWireRange + 50.f)
			break;
		
		
		i++;
	}

    // 충돌 검사가 끝났으면 Ray의 충돌체 Offset을 시작 위치로 초기화
	GetCollider()->SetOffsetPos(Vec2(0.f,0.f));
}

bool Raycast::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetOffsetPos() + _pLeftCol->GetObj()->GetWorldPos();
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




