#pragma once
//��ü�� �����Ϳ����� �־�δ� ���
class CCore
{
	SINGLE(CCore)

private:
	HWND	m_hWnd;
	POINT	m_ptResolution;
	HDC		m_hDC;
	HBITMAP m_hBit;
	HDC		m_memDC;

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void Progress();
	void Update();
	void Render();

	HDC GetmemDC() { return m_memDC; }
	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	void GameOver();
private:

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

