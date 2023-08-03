#include "pch.h"
#include "CCamera.h"

#include "GameObject.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
CCamera::CCamera()
	:m_pTargetObj(nullptr)
	,m_fTime(0.5f)
	,m_fAccTime(0.5f)
	,m_fSpeed(0)
{

}

CCamera::~CCamera()
{

}


void CCamera::Update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			m_vLookAt = m_pTargetObj->GetPos();
		}

	}

	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;


	//ȭ�� �߾���ǥ�� ī�޶� LookAt ��ǥ���� ���̰� ���
	CalDiff();
}

void CCamera::CalDiff()
{
	//���� LookAt �� ���� Look�� ���̰��� �����ؼ� ������ LookAt�� ���Ѵ�.
	m_fAccTime += fDT;

	if (m_fAccTime >= m_fTime)
	{
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
		m_vCurLookAt = m_vPrevLookAt + vLookDir.normalize() * m_fSpeed * fDT;
	}


	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;

}

//ī�޶� ������ �����̴� ��ǥ���� �����Ҷ��� �ӵ��� �ٿ��� �����ϰԲ� �ϴ� ���� ����
//ī�޶� ����ŷ ��ɵ� 