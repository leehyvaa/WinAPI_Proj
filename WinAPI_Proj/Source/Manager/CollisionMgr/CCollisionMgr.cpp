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
	for (UINT iRow = 0; iRow < static_cast<UINT>(GROUP_TYPE::END); iRow++)
	{
		m_arrCheck[iRow];
		for (UINT iCol = iRow; iCol < static_cast<UINT>(GROUP_TYPE::END); iCol++)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroupUpdate(static_cast<GROUP_TYPE>(iRow), static_cast<GROUP_TYPE>(iCol));
			}
		}
	}
}


bool CCollisionMgr::CalculateCollisionInfo(CCollider* _pLeftCol, CCollider* _pRightCol, Vec2& _outMtvDirection,
    float& _outMtvDepth)
{
    // 유효성 검사
    if (!_pLeftCol || !_pRightCol || !_pLeftCol->IsActive() || !_pRightCol->IsActive())
    {
        return false; // 충돌 불가 조건
    }
    if (_pLeftCol->GetObj() == _pRightCol->GetObj())
    {
        return false; // 자기 자신과는 충돌하지 않음
    }

    // 두 OBB의 정보 가져오기
    Vec2 leftPos = _pLeftCol->GetFinalPos();
    Vec2 leftHalfExtents = _pLeftCol->GetHalfExtents();
    Vec2* leftAxes = _pLeftCol->GetAxes();

    Vec2 rightPos = _pRightCol->GetFinalPos();
    Vec2 rightHalfExtents = _pRightCol->GetHalfExtents();
    Vec2* rightAxes = _pRightCol->GetAxes();

    // 두 OBB 중심 사이의 벡터
    Vec2 distVec = rightPos - leftPos;

    // 분리축 후보 (총 4개)
    Vec2 axesToCheck[4] = {
        leftAxes[0],  // Left OBB의 X축
        leftAxes[1],  // Left OBB의 Y축
        rightAxes[0], // Right OBB의 X축
        rightAxes[1]  // Right OBB의 Y축
    };

    float minOverlap = FLT_MAX; // 최소 겹침(관통 깊이)을 저장할 변수
    Vec2 mtvAxis;               // 최소 겹침이 발생한 축

    // 각 축에 대해 투영하여 겹치는지 검사
    for (int i = 0; i < 4; ++i)
    {
        Vec2 currentAxis = axesToCheck[i];
        currentAxis.Normalize();

        // 각 OBB를 현재 축에 투영했을 때의 반지름 계산
        // (중심에서 축에 투영된 가장 먼 꼭지점까지의 거리)
        float leftRadius = leftHalfExtents.x * abs(currentAxis.Dot(leftAxes[0])) +
                           leftHalfExtents.y * abs(currentAxis.Dot(leftAxes[1]));
        float rightRadius = rightHalfExtents.x * abs(currentAxis.Dot(rightAxes[0])) +
                            rightHalfExtents.y * abs(currentAxis.Dot(rightAxes[1]));

        // 두 OBB 중심 사이의 거리를 현재 축에 투영
        float distProjection = abs(distVec.Dot(currentAxis));

        // 투영된 거리가 두 반지름의 합보다 크면, 분리축이 존재 -> 충돌하지 않음
        if (distProjection > leftRadius + rightRadius)
        {
            return false; // 분리축 발견, 충돌 아님
        }
        else
        {
            // 겹침 발생: 현재 축에서의 겹침(관통) 깊이 계산
            float overlap = (leftRadius + rightRadius) - distProjection;

            // 현재 겹침이 지금까지 발견된 최소 겹침보다 작은지 확인
            if (overlap < minOverlap)
            {
                minOverlap = overlap; // 최소 겹침 값 갱신
                mtvAxis = currentAxis; // 최소 겹침 축 갱신
            }
        }
    }

    // 모든 축에서 겹침이 확인됨 -> 충돌 발생
    // MTV 계산 완료

    // MTV 방향 결정: distVec과 mtvAxis의 내적 부호를 확인하여
    // 두 충돌체를 밀어내는 올바른 방향을 설정
    // MTV는 보통 첫 번째 물체(_pLeftCol)를 두 번째 물체(_pRightCol)로부터 밀어내는 방향,
    // 또는 그 반대 방향으로 정의, 여기서는 _pRightCol을 _pLeftCol로부터 밀어내는 방향으로 설정
    if (distVec.Dot(mtvAxis) < 0.f)
    {
        // distVec과 mtvAxis가 반대 방향을 가리키는 경우 (mtvAxis가 L -> R 방향)
        _outMtvDirection = mtvAxis;
    }
    else
    {
        // distVec과 mtvAxis가 같은 방향을 가리키는 경우 (mtvAxis가 R -> L 방향)
        _outMtvDirection = -mtvAxis; // 방향 반전
    }
    
    _outMtvDirection.Normalize();

    _outMtvDepth = minOverlap; // 계산된 최소 겹침(관통 깊이)

    return true; // 충돌 감지
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
    if (!pCurScene) return; // Scene 로딩 중 예외 처리

	// GetGroupObject 함수가 벡터를 레퍼런스로 보냈으니까
	// 받을때도 레퍼런스로 받아야한다. 레퍼런스를 안쓰면 그냥
	// 지역변수로 받은것이 되어버린다.
	const vector<GameObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<GameObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	map<LONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
	    CCollider* pLeftCol = vecLeft[i]->GetCollider();
	    // 충돌체 없거나, 비활성 상태면 건너뜀
	    if (nullptr == pLeftCol || !pLeftCol->IsActive())
	        continue;

	    for (size_t j = 0; j < vecRight.size(); j++)
	    {
	        CCollider* pRightCol = vecRight[j]->GetCollider();
	        // 충돌체 없거나, 비활성 상태거나, 자기 자신이면 건너뜀
	        if (nullptr == pRightCol || !pRightCol->IsActive() || vecLeft[i] == vecRight[j])
	            continue;

	        // 두 충돌체 조합 아이디 생성
	        COLLIDER_ID ID;
	        // ID 순서 정렬
	        if (pLeftCol->GetID() < pRightCol->GetID())
	        {
	            ID.iLeft_id = pLeftCol->GetID();
	            ID.iRight_id = pRightCol->GetID();
	        }
	        else
	        {
	            ID.iLeft_id = pRightCol->GetID();
	            ID.iRight_id = pLeftCol->GetID();
	        }
			
			iter =  m_mapColInfo.find(ID.ID);

			// 충돌 정보가 미 등록 상태인 경우 등록(충돌하지 않았다로)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}
	        
	        Vec2 mtvDirection;
	        float mtvDepth;
	        bool bCurrentlyColliding = CalculateCollisionInfo(pLeftCol, pRightCol, mtvDirection, mtvDepth);
	        bool bPreviouslyColliding = iter->second;

	        // 현재 충돌중인 경우
	        if (bCurrentlyColliding)
	        {
	            // 이전에도 충돌중
	            if (bPreviouslyColliding) // 충돌 지속 (OnCollision)
	            {
	                // 둘 중 하나라도 삭제 예정이면 충돌 해제
	                if (vecLeft[i]->IsDead() || vecRight[j]->IsDead()) 
	                {
	                        pLeftCol->OnCollisionExit(pRightCol);
	                        pRightCol->OnCollisionExit(pLeftCol);
	                        iter->second = false;
	                }
	                else // 둘 다 살아있으면 OnCollision 호출
	                {
	                    pLeftCol->OnCollision(pRightCol);
	                    pRightCol->OnCollision(pLeftCol);
	                }
	            }
	            else // 충돌 시작 (OnCollisionEnter)
	            {
	                if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead()) // 둘 다 살아있을 때만 Enter
	                {
	                    pLeftCol->OnCollisionEnter(pRightCol);
	                    pRightCol->OnCollisionEnter(pLeftCol);
	                    iter->second = true;
	                }
	            }
	        }
	        else // 현재 충돌 안 함
	        {
	            if (bPreviouslyColliding) // 충돌 종료 (OnCollisionExit)
	            {
	                pLeftCol->OnCollisionExit(pRightCol);
	                pRightCol->OnCollisionExit(pLeftCol);
	                iter->second = false;
	            }
	        }
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
    // 두 OBB의 정보 가져오기
    Vec2 leftPos = _pLeftCol->GetFinalPos();
    Vec2 leftHalfExtents = _pLeftCol->GetHalfExtents();
    Vec2* leftAxes = _pLeftCol->GetAxes();

    Vec2 rightPos = _pRightCol->GetFinalPos();
    Vec2 rightHalfExtents = _pRightCol->GetHalfExtents();
    Vec2* rightAxes = _pRightCol->GetAxes();

    // 두 OBB 중심 사이의 벡터
    Vec2 distVec = rightPos - leftPos;

    // 분리축 후보 (총 4개)
    Vec2 axesToCheck[4] = {
        leftAxes[0],  // Left OBB의 X축
        leftAxes[1],  // Left OBB의 Y축
        rightAxes[0], // Right OBB의 X축
        rightAxes[1]  // Right OBB의 Y축
    };

    // 각 축에 대해 투영하여 겹치는지 검사
    for (int i = 0; i < 4; ++i)
    {
        Vec2 currentAxis = axesToCheck[i];
        // currentAxis.Normalize(); // CCollider에서 이미 정규화됨

        // 각 OBB를 현재 축에 투영했을 때의 반지름 계산
        float leftRadius = leftHalfExtents.x * abs(currentAxis.Dot(leftAxes[0])) +
                           leftHalfExtents.y * abs(currentAxis.Dot(leftAxes[1]));
        float rightRadius = rightHalfExtents.x * abs(currentAxis.Dot(rightAxes[0])) +
                            rightHalfExtents.y * abs(currentAxis.Dot(rightAxes[1]));

        // 두 OBB 중심 사이의 거리를 현재 축에 투영
        float distProjection = abs(distVec.Dot(currentAxis));

        // 투영된 거리가 두 반지름의 합보다 크면, 분리축이 존재 -> 충돌하지 않음
        if (distProjection > leftRadius + rightRadius)
        {
            return false;
        }
    }

    // 모든 축에서 겹친 경우
    return true;
}



void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로 큰 값을 열(비트)로 사용
	UINT iRow = static_cast<UINT>(_eLeft);
	UINT iCol = static_cast<UINT>(_eRight);

	if (iCol < iRow)
	{
		iRow = static_cast<UINT>(_eRight);
		iCol = static_cast<UINT>(_eLeft);
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
