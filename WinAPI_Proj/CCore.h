#pragma once
#include"pch.h"
//��ü�� �����Ϳ����� �־�δ� ���
class CCore
{
	SINGLE(CCore);

private:
	HWND m_hWnd;
	POINT m_ptResolution;
	HDC m_hDC;
public:
	int init(HWND _hWnd, POINT _ptResolution);
	void Progress();
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

