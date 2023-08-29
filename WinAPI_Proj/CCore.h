#pragma once

class CTexture;

//��ü�� �����Ϳ����� �־�δ� ���
class CCore
{
	SINGLE(CCore)

private:
	HWND	m_hWnd;
	POINT	m_ptResolution;
	HDC		m_hDC;

	CTexture* m_pMemTex; //����� �ؽ���


	//GDI Object
	HBRUSH m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN m_arrPen[(UINT)PEN_TYPE::END];

	//�޴�
	HMENU m_hMenu; //Tool������ ���

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void Progress();

	void CreateBrushPen();
	void Clear(HDC _dc);

	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);


	HDC GetmemDC();
	HDC GetMainDC() { return m_hDC; }
	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
	

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

