#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"

int g_arrVK[static_cast<int>(KEY::LAST)] =
{
	VK_LEFT,//LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	//
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	//
	VK_MENU,//ALT,
	VK_CONTROL,//CTRL,
	VK_LSHIFT,//LSHIFT,
	VK_SPACE,//SPACE,
	VK_RETURN,//ENTER,
	VK_ESCAPE,//ESC,
	VK_LBUTTON,//LBUTTON,
	VK_RBUTTON,//RBUTTON,
	VK_BACK,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
    '9',
    '0'
    
	//
	//LAST,
};

CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::init()
{
	for (int i = 0; i < static_cast<int>(KEY::LAST); i++)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE,false });
	}
}

void CKeyMgr::Update()
{
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	if (nullptr != hWnd)
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); i++)
		{
			//키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) * 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					//이전에도 눌려있었다.
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					//이전에 눌려있지 않았다.
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				m_vecKey[i].bPrevPush = true;
			}

			//키가 안눌려있다.
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					//이전에 눌려있었다.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					//이전에도 안눌려있었다.
					m_vecKey[i].eState = KEY_STATE::NONE;

				}
				m_vecKey[i].bPrevPush = false;
			}
		}

		//마우스 위치 계산
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2(static_cast<float>(ptPos.x), static_cast<float>(ptPos.y));
	}
	else
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); i++)
		{
			m_vecKey[i].bPrevPush = false;

			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}

}
