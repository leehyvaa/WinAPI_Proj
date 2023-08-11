#pragma once

enum class KEY_STATE
{
	NONE,//������ �ʾҰ� �������� ������ ���� ����
	TAP, //���� ����
	HOLD,//������ �ִ�
	AWAY,//�� �� ����
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,
	LBUTTON,
	RBUTTON,

	F1,
	F2,
	F3,
	F4,

	LAST,
};

struct tKeyInfo
{
	KEY_STATE	eState; //Ű ���°�
	bool bPrevPush; //���� �����ӿ��� ���ȴ��� ����
};

class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
	Vec2 m_vCurMousePos;
public:
	void init();
	void Update();
	KEY_STATE GetKeyState(KEY _eKey){return m_vecKey[(int)_eKey].eState;}
	Vec2 GetMousePos() { return m_vCurMousePos; }
};

