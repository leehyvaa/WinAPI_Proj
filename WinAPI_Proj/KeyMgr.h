#pragma once

enum class KEY_STATE
{
	HOME,//������ �ʾҰ� �������� ������ ���� ����
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

	W,
	A,
	S,
	D,
	LAST,
};

struct tKeyInfo
{
	KEY_STATE	eState; //Ű ���°�
	bool bPrev; //���� �����ӿ��� ���ȴ��� ����
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

