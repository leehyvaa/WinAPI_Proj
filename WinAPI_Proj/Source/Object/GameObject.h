#pragma once
//오브젝트를 설계할 땐 항상 복사생성자를 커스텀해서 만들어야 하는지를 생각할 것


#include "global.h"
#include "CCamera.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;
struct ID2D1RenderTarget;




class GameObject
{
private:
    bool m_bAlive;
    bool m_bActive; // 활성화 상태 (풀링에서도 사용)
    bool m_bManagedByPool;


    
	wstring m_strName;
	Vec2 m_vDir;
	Vec2 m_vStandardDir;
	float m_fLocalRotation;
    
	Vec2 m_vScale;
    Vec2 m_vPos;
    Vec2 m_vPivotOffset;  // 회전 중심점 오프셋
    GameObject* m_pParent;
	CCollider* m_pCollider;
	CAnimator* m_pAnimator;
	CRigidBody* m_pRigidBody;
	CGravity* m_pGravity;

    // 로테이션 캐싱
    float m_cachedWorldRotation;
    bool m_worldRotationDirty;
    float m_cachedParentWorldRotation;
    bool m_hasCachedParentInfo;

protected:
    GROUP_TYPE m_eGroup;
    Vec2 m_vLocalPos;
    bool m_bIsFacingRight;
    bool m_bIsFacingRightPrev;
public:
	GameObject();
	GameObject(const GameObject& _origin);
	virtual ~GameObject();

    void SetIsFacingRightPrev(bool _dir) { m_bIsFacingRightPrev = _dir; }
    void SetIsFacingRight(bool _dir) { m_bIsFacingRight = _dir; }
    bool GetIsFacingRightPrev() { return m_bIsFacingRightPrev; }
    bool GetIsFacingRight() { return m_bIsFacingRight; }
	virtual void LookAt(Vec2 _target);
	void SetDir(Vec2 _dir) { m_vDir = _dir; }
    void SetWorldPos(Vec2 _vWorldPos);
    void SetLocalPos(Vec2 _vLocalPos) { m_vLocalPos = _vLocalPos; };
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	void SetStdDir(Vec2 _dir) { m_vStandardDir = _dir; }
    void SetGroup(GROUP_TYPE _type) { m_eGroup = _type; }
    void SetPivotOffset(Vec2 _offset) { m_vPivotOffset = _offset; }
    
    Vec2 GetPivotOffset() { return m_vPivotOffset; }
    Vec2 GetLocalPos(){ return m_vLocalPos; }
	Vec2 GetWorldPos();
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetDir() { return m_vDir; }
	Vec2 GetStdDir() { return m_vStandardDir; }
    GROUP_TYPE GetGroup() { return m_eGroup; }

    
    void SetLocalRotation(float _fRot) {  m_fLocalRotation = _fRot; m_worldRotationDirty = true; }
    float GetLocalRotation() {return m_fLocalRotation; }
    float GetWorldRotation();

    
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

    // 오브젝트 초기화 (풀에서 재사용시 호출)
    virtual void Reset();
    bool IsActive() const { return m_bActive; }
    void SetActive(bool _bActive);
	bool IsDead() { return !m_bAlive; }
    void SetManagedByPool(bool _bManaged) { m_bManagedByPool = _bManaged; }
    bool IsManagedByPool() { return m_bManagedByPool; }

	void CreateCollider();
	CCollider* GetCollider() {return m_pCollider; }
	void CreateAnimator();
	CAnimator* GetAnimator() { return m_pAnimator; }
	void CreateRigidBody();
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	void CreateGravity();
	CGravity* GetGravity() { return m_pGravity; }
    void SetParent(GameObject* _pParent) { m_pParent = _pParent; }
    GameObject* GetParent() { return m_pParent; }

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};


	virtual void Start() {}; //씬 시작되기 직전에 호출되는함수
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
	virtual void RenderD2D(ID2D1RenderTarget* _pRenderTarget) {}  // Direct2D 렌더링 기본 구현

	void Component_Render(HDC _dc);

	virtual GameObject* Clone() = 0;

	void SetDead(bool _bDead) { m_bAlive = _bDead; }
protected:

	friend class CEventMgr;
};

#pragma once


