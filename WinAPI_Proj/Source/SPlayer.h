#pragma once
#include "GameObject.h"

class CTexture;
class PlayerArm;
class CHook;
class Raycast;
class CMonster;

enum class PLAYER_ATTACK_STATE
{

};

class SPlayer : public GameObject
{

private:
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

    // 와이어 액션 변수
	Raycast* m_pPlayerRay;
	CCollider* m_pRayHitCollider;
	Vec2 m_vRayHitPos;
	float m_fMoveEnergy;
	float m_fPosEnergy;
	float m_fHookDistance;
	
	// 제압 시스템 관련 변수
	CMonster* m_pSubduedMonster; 
	bool m_bIsSubduing;     
	float m_fSubdueRange;           // 제압 가능 거리
    bool m_bIsExecuteDashing;
	
	// 플레이어 이동 관련 변수
	bool m_bIsMovingToTarget;      
	Vec2 m_vMoveStartPos;         
	Vec2 m_vMoveTargetPos;        
	float m_fMoveProgress;         // 이동 진행도 (0.0 ~ 1.0)
	float m_fMoveSpeed;
public:
	SPlayer();

	// 부모와 자식부분을 따로 복사하는 복사생성자 아래 내용을 안적어도 아래와 같이
	// 컴파일러가 자동으로 구현해줌
	// 복사생성자가 문제가 되는 경우는 콜라이더처럼 자신의 owner를 가지고 있거나
	// 고유한 id값을 가지고 있을때 얕은복사를 하면 문제가 됨
	SPlayer(const SPlayer &_origin)
		: GameObject(_origin), m_fSpeed(_origin.m_fSpeed), m_eCurState(_origin.m_eCurState), m_ePrevState(_origin.m_ePrevState), m_pPlayerArm(_origin.m_pPlayerArm), m_pPlayerRay(_origin.m_pPlayerRay), m_bOnGround(false), m_bClimbing(false), m_vRayHitPos(_origin.m_vRayHitPos), m_pRayHitCollider(nullptr), m_eClimbState(PLAYER_CLIMB_STATE::NONE), m_pSubduedMonster(nullptr), m_bIsSubduing(false), m_fSubdueRange(_origin.m_fSubdueRange), m_bIsMovingToTarget(false), m_vMoveStartPos(Vec2(0.f, 0.f)), m_vMoveTargetPos(Vec2(0.f, 0.f)), m_fMoveProgress(0.f), m_fMoveSpeed(_origin.m_fMoveSpeed)
	{
	}
	virtual ~SPlayer();


	
	Vec2 GetTargetPos() { return m_vRayHitPos; }
	float GetMoveEnergy() { return m_fMoveEnergy; }
    float GetPosEnergy() { return m_fPosEnergy; }
    float GetWireRange() { return m_fWireRange; }
    float GetWireMaxRange() { return m_fWireMaxRange; }
    float GetHookDistance() {return m_fHookDistance;}

	   
    bool IsWireTaut();
    bool IsOnGround() { return m_bOnGround; }
    bool IsWallClimbing() { return m_bClimbing; }
    bool IsRidingWire() { return m_bRidingWire; }
	   
    // 제압 시스템 관련 getter/setter
    bool IsSubduing() const { return m_bIsSubduing; }
    CMonster* GetSubduedMonster() const { return m_pSubduedMonster; }
    float GetSubdueRange() const { return m_fSubdueRange; }
    
    PLAYER_STATE GetState() { return m_eCurState; }
    PLAYER_STATE GetPrevState() { return m_ePrevState; }
    PLAYER_CLIMB_STATE GetClimbState() { return m_eClimbState; }
    CHook* GetPlayerHook() const { return m_pPlayerHook; }

	void SetOnGround(bool _onGround) { m_bOnGround = _onGround; }
    void SetRidingWire(bool _ridingWire) { m_bRidingWire = _ridingWire; }
    void SetWallClimbing(bool _isClimbing) { m_bClimbing = _isClimbing; }
    void SetArm(PlayerArm *_arm) { m_pPlayerArm = _arm; }
    void SetHookRemove(CHook *_hook) { m_pPlayerHook = _hook; }
	void SetPlayerState(PLAYER_STATE _eState) { m_eCurState = _eState; }
    void SetMoveEnergy(float _energy) { m_fMoveEnergy = _energy; }
    void SetPosEnergy(float _energy) { m_fPosEnergy = _energy; }
	   
    // 제압 시스템
    void SetSubduing(bool _bSubduing) { m_bIsSubduing = _bSubduing; }
    void SetSubduedMonster(CMonster* _pMonster) { m_pSubduedMonster = _pMonster; }
    void SetSubdueRange(float _fRange) { m_fSubdueRange = _fRange; }
	void EndSubdue();

    
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;
	   virtual void Reset() override;

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
    void ApplySwingVelocity();
    void UpdateSwingEnergy();

	void CreateHook();
	void RayCasting();
	
	// 제압 시스템
	void StartSubdue(CMonster* _pMonster);
	void UpdateSubdue();
	void CleanupSubdueOnDeath();       // 플레이어 사망 시 제압 정리
	
	// 플레이어 대쉬
	void StartMoveToTarget(const Vec2& _targetPos);  // 목표 위치로 빠른 이동 시작
	void UpdateMoveToTarget();                       // 이동 업데이트
	void CompleteMoveToTarget();                     // 이동 완료 처리

    
	// 자신의 데이터를 복사해서 하나 새로 생성한 후 이를 반환하는 클론함수
	// virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)
};
