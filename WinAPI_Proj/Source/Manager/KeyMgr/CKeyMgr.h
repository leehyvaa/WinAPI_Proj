#pragma once

enum class KEY_STATE
{
	NONE,//누르지 않았고 이전에도 눌리지 않은 상태
	TAP, //누른 시점
	HOLD,//누르고 있는
	AWAY,//막 땐 시점
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
	BACK,

	F1,
	F2,
	F3,
	F4,

	F5,
	F6,
	F7,
	F8,

	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,


	

	LAST,
};

struct tKeyInfo
{
	KEY_STATE	eState; //키 상태값
	bool bPrevPush; //이전 프레임에서 눌렸는지 여부
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

