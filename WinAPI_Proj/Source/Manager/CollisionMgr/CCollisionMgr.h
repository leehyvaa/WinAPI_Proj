#pragma once
class CCollider;



union COLLIDER_ID
{
	struct {
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr)
private:
	//충돌체 간의 이전 프레임 충돌 정보
	map<LONGLONG, bool> m_mapColInfo;
	UINT m_arrCheck[static_cast<UINT>(GROUP_TYPE::END)]; //그룹간의 충돌 체크 매트릭스
public:
	void Update();

	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset(){memset(m_arrCheck, 0, sizeof(UINT) * static_cast<UINT>(GROUP_TYPE::END));}

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft,GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
};

