#pragma once
#include "GameObject.h"

class CTexture;
class PlayerArm;
class CHook;
class Raycast;



enum class PLAYER_ATTACK_STATE
{
	
};



class SPlayer : public GameObject
{

private:
	int m_iDir;
	int m_iPrevDir;
	float m_fSpeed;
	float wireRange;
	float wireMaxRange;
	bool m_bOnGround;
	bool isClimbing;
	bool canBooster;
	PLAYER_STATE m_eCurState;
	PLAYER_STATE m_ePrevState;
	CHook* playerHook;
	PlayerArm* playerArm;
	Raycast* playerRay;
	CCollider* onCollisionRay;
	Vec2 targetPos;
	float moveEnergy;
	float posEnergy;

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
		,playerArm(_origin.playerArm)
		,playerRay(_origin.playerRay)
		,m_bOnGround(false)
		,isClimbing(false)
		,targetPos(_origin.targetPos)
		, onCollisionRay(nullptr)
		
	{}
	virtual ~SPlayer();


	void SetDir(int _dir) { m_iDir = _dir; }
	int GetDir() { return m_iDir; }
	int GetPrevDir() { return m_iPrevDir; }
	Vec2 GetTargetPos() { return targetPos; }
	void SetOnGround(bool _onGround) { m_bOnGround = _onGround; }
	void SetArm(PlayerArm* _arm) { playerArm = _arm; }
	void SetHookRemove(CHook* _hook) { playerHook = _hook; }

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
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);


	void Jump();
	void ClimbJump();
	void HorizontalMove();
	void VirticalMove();
	void SwingMove();

	void CreateHook();
	void RayCasting();

	//�ڽ��� �����͸� �����ؼ� �ϳ� ���� ������ �� �̸� ��ȯ�ϴ� Ŭ���Լ�
	//virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)


};

