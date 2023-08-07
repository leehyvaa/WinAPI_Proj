#pragma once
#include "GameObject.h"

class CTexture;

enum class PLAYER_STATE
{
	IDLE,
	WALK,
	ATTACK,
	JUMP,
	DAMAGED,
	DEAD,
};

enum class PLAYER_ATTACK_STATE
{
	
};



class SPlayer : public GameObject
{

private:
	int m_iDir;
	int m_iPrevDir;
	float m_fSpeed;
	PLAYER_STATE m_eCurState;
	PLAYER_STATE m_ePrevState;

public:
	SPlayer();

	// �θ�� �ڽĺκ��� ���� �����ϴ� ��������� �Ʒ� ������ ����� �Ʒ��� ���� 
	//�����Ϸ��� �ڵ����� ��������
	//��������ڰ� ������ �Ǵ� ���� �ݶ��̴�ó�� �ڽ��� owner�� ������ �ְų�
	//������ id���� ������ ������ �������縦 �ϸ� ������ ��
	SPlayer(const SPlayer& _origin) 
		:GameObject(_origin)
		,m_iDir(_origin.m_iDir)
		,m_fSpeed(_origin.m_fSpeed)
	{}
	virtual ~SPlayer();


	void SetDir(int _dir) { m_iDir = _dir; }


	virtual void Update() override;
	virtual void Render(HDC _dc)  override;

private:
	void CreateWire();
	void Update_State();
	void Update_Move();
	void Update_Animation();
	void Update_Gravity();

	virtual void OnCollisionEnter(CCollider* _pOther);

	//�ڽ��� �����͸� �����ؼ� �ϳ� ���� ������ �� �̸� ��ȯ�ϴ� Ŭ���Լ�
	//virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)
};

