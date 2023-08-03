#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"

int g_arrVK[(int)KEY::LAST] =
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
	for (int i = 0; i < (int)KEY::LAST; i++)
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
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			//Ű�� �����ִ�.
			if (GetAsyncKeyState(g_arrVK[i]) * 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					//�������� �����־���.
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					//������ �������� �ʾҴ�.
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				m_vecKey[i].bPrevPush = true;
			}

			//Ű�� �ȴ����ִ�.
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					//������ �����־���.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					//�������� �ȴ����־���.
					m_vecKey[i].eState = KEY_STATE::NONE;

				}
				m_vecKey[i].bPrevPush = false;
			}
		}

		//���콺 ��ġ ���
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}
	else
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
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
