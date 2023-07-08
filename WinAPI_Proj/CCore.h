#pragma once
class CCore
{
private:
	static CCore* g_pInst;
private:
	CCore();
	~CCore();

public:
	static CCore* GetIstance()
	{
		//최초호출
		if (nullptr == g_pInst)
		{
			g_pInst = new CCore;
		}

		return g_pInst;
	}


	static void Release()
	{
		if (nullptr != g_pInst)
		{
			delete g_pInst;
			g_pInst = nullptr;
		}
	
	}
};

