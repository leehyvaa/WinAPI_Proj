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
	//�浹ü ���� ���� ������ �浹 ����
	map<LONGLONG, bool> m_mapColInfo;
	UINT m_arrCheck[(UINT)GROUP_TYPE::END]; //�׷찣�� �浹 üũ ��Ʈ����
public:
	void Update();

	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset(){memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);}

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft,GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
};

