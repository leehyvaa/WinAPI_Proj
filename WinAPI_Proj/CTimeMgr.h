#pragma once
#include "pch.h"
class CTimeMgr
{
	SINGLE(CTimeMgr)
private:
	//FPS
	//�ش� ��ǻ�Ϳ��� 1�����Ӵ� �ð� Delta Time
	LARGE_INTEGER m_CurCount;
	LARGE_INTEGER m_PrevCount;
	LARGE_INTEGER m_FreQuency;

	double m_dDT; //������ ���� �ð���
	double m_dAcc; //1�� üũ�� ���� ���� �ð�
	UINT m_iCallCount;//�Լ� ȣ�� Ƚ�� üũ
	UINT m_iFPS;//�ʴ� ȣ�� üũ

public:
	void init();
	void Update();

	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }
};

