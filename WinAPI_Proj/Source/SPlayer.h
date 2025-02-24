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
	float m_fWireRange;
	float m_fWireMaxRange;
	bool m_bOnGround;
	bool m_bClimbing;
	bool m_bRidingWire;
	bool m_bCanBooster;
	PLAYER_STATE m_eCurState;
	PLAYER_STATE m_ePrevState;
	PLAYER_CLIMB_STATE m_eClimbState;
	PLAYER_CLIMB_STATE m_ePrevClimbState;
	CHook* m_pPlayerHook;
	PlayerArm* m_pPlayerArm;
	Raycast* m_pPlayerRay;
	CCollider* m_pRayHitCollider;
	Vec2 m_vRayHitPos;
	float m_fMoveEnergy;
	float m_fPosEnergy;

public:
	SPlayer();

	// 부모와 자식부분을 따로 복사하는 복사생성자 아래 내용을 안적어도 아래와 같이
	// 컴파일러가 자동으로 구현해줌
	// 복사생성자가 문제가 되는 경우는 콜라이더처럼 자신의 owner를 가지고 있거나
	// 고유한 id값을 가지고 있을때 얕은복사를 하면 문제가 됨
	SPlayer(const SPlayer &_origin)
		: GameObject(_origin), m_iDir(_origin.m_iDir), m_fSpeed(_origin.m_fSpeed), m_eCurState(_origin.m_eCurState), m_ePrevState(_origin.m_ePrevState), m_iPrevDir(_origin.m_iPrevDir), m_pPlayerArm(_origin.m_pPlayerArm), m_pPlayerRay(_origin.m_pPlayerRay), m_bOnGround(false), m_bClimbing(false), m_vRayHitPos(_origin.m_vRayHitPos), m_pRayHitCollider(nullptr), m_eClimbState(PLAYER_CLIMB_STATE::NONE)
	{
	}
	virtual ~SPlayer();

	void SetDir(int _dir) { m_iDir = _dir; }
	int GetDir() { return m_iDir; }
	int GetPrevDir() { return m_iPrevDir; }
	Vec2 GetTargetPos() { return m_vRayHitPos; }
	void SetOnGround(bool _onGround) { m_bOnGround = _onGround; }
    void SetRidingWire(bool _ridingWire) { m_bRidingWire = _ridingWire; }
	bool IsOnGround() { return m_bOnGround; }
	bool IsWallClimbing() { return m_bClimbing; }
    bool IsRidingWire() { return m_bRidingWire; }
	void SetArm(PlayerArm *_arm) { m_pPlayerArm = _arm; }
	void SetHookRemove(CHook *_hook) { m_pPlayerHook = _hook; }

	PLAYER_STATE GetState() { return m_eCurState; }
	PLAYER_STATE GetPrevState() { return m_ePrevState; }
	PLAYER_CLIMB_STATE GetClimbState() { return m_eClimbState; }

	void SetPlayerState(PLAYER_STATE _eState) { m_eCurState = _eState; }

	void SetWallClimbing(bool _isClimbing) { m_bClimbing = _isClimbing; }

	virtual void Update() override;
	virtual void Render(HDC _dc) override;

private:
	void Enter_State(PLAYER_STATE _eState);
	void Update_State();
	void Exit_State(PLAYER_STATE _eState);

	void Update_Animation();
	void Update_Gravity();

	virtual void OnCollisionEnter(CCollider *_pOther);
	virtual void OnCollision(CCollider *_pOther);
	virtual void OnCollisionExit(CCollider *_pOther);
    
	void ClimbAnimationUpdate();
	void WallKickJump();
	void HorizontalMove();
	void VirticalMove();
	void SwingMove();

	void CreateHook();
	void RayCasting();

	// 자신의 데이터를 복사해서 하나 새로 생성한 후 이를 반환하는 클론함수
	// virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)
};
