#pragma once
#include "GameObject.h"

class CTexture;

enum class PLAYER_STATE
{
	IDLE,
	RUN,
	ATTACK,
	JUMP,
	CLIMB,
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
	bool m_bOnGround;
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
		,m_eCurState(_origin.m_eCurState)
		,m_ePrevState(_origin.m_ePrevState)
		,m_iPrevDir(_origin.m_iPrevDir)
	{}
	virtual ~SPlayer();


	void SetDir(int _dir) { m_iDir = _dir; }
	int GetDir() { return m_iDir; }
	int GetPrevDir() { return m_iPrevDir; }
	void SetOnGround(bool _onGround) { m_bOnGround = _onGround; }

	PLAYER_STATE GetState() { return m_eCurState; }
	PLAYER_STATE GetPrevState() { return m_ePrevState; }



	virtual void Update() override;
	virtual void Render(HDC _dc)  override;

private:
	void Update_State();
	void Update_Move();
	void Update_Animation();
	void Update_Gravity();

	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);


	void Jump();
	void Climb();
	void HorizontalMove();
	void VirticalMove();


	//�ڽ��� �����͸� �����ؼ� �ϳ� ���� ������ �� �̸� ��ȯ�ϴ� Ŭ���Լ�
	//virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)


};

