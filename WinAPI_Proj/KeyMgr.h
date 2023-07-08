#pragma once

enum class KEY_STATE
{
	HOME,//누르지 않았고 이전에도 눌리지 않은 상태
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

	W,
	A,
	S,
	D,
	LAST,
};

struct tKeyInfo
{
	KEY_STATE	eState; //키 상태값
	bool bPrev; //이전 프레임에서 눌렸는지 여부
};

class KeyMgr
{
	//SINGLE(KeyMgr);

private:
	//vector<tKeyInfo> m_vecKey;
public:
	void init();
	void Update();
};

