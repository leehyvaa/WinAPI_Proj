#pragma once
#include"pch.h"
//��ü�� �����Ϳ����� �־�δ� ���
class CCore
{

	SINGLE(CCore);


public:
	int init();
	void Update();
	void Render();
private:
	CCore();
	~CCore();
	
};







//�����͸� �����Ϳ����� �ִ� ���
//class CCore
//{
//private:
//	static CCore* g_pInst;
//private:
//	CCore();
//	~CCore();
//
//public:
//	static CCore* GetIstance()
//	{
//		//����ȣ��
//		if (nullptr == g_pInst)
//		{
//			g_pInst = new CCore;
//		}
//
//		return g_pInst;
//	}
//
//
//	static void Release()
//	{
//		if (nullptr != g_pInst)
//		{
//			delete g_pInst;
//			g_pInst = nullptr;
//		}
//	
//	}
//};

