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

	// 부모와 자식부분을 따로 복사하는 복사생성자 아래 내용을 안적어도 아래와 같이 
	//컴파일러가 자동으로 구현해줌
	//복사생성자가 문제가 되는 경우는 콜라이더처럼 자신의 owner를 가지고 있거나
	//고유한 id값을 가지고 있을때 얕은복사를 하면 문제가 됨
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


	//자신의 데이터를 복사해서 하나 새로 생성한 후 이를 반환하는 클론함수
	//virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)


};

