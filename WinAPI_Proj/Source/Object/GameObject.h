#pragma once
//오브젝트를 설계할 땐 항상 복사생성자를 커스텀해서 만들어야 하는지를 생각할 것


#include "global.h"
#include "CCamera.h"
class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;




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

	Vec2 m_vPos;
	Vec2 m_vScale;
    
    GameObject* m_pParent;
	CCollider* m_pCollider;
	CAnimator* m_pAnimator;
	CRigidBody* m_pRigidBody;
	CGravity* m_pGravity;


protected:
    GROUP_TYPE m_eGroup;

public:
	GameObject();
	GameObject(const GameObject& _origin);
	virtual ~GameObject();

	void LookAt(Vec2 _target);
	void SetDir(Vec2 _dir) { m_vDir = _dir; }
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	void SetStdDir(Vec2 _dir) { m_vStandardDir = _dir; }
    void SetGroup(GROUP_TYPE _type) { m_eGroup = _type; }
    
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetDir() { return m_vDir; }
	Vec2 GetStdDir() { return m_vStandardDir; }
    GROUP_TYPE GetGroup() { return m_eGroup; }

    
    void SetLocalRotation(float _fRot) { m_fLocalRotation = _fRot; }
    float GetLocalRotation() {return m_fLocalRotation; }
    float GetWorldRotation() const
	{
	    float finalRotation = m_fLocalRotation;
	    GameObject* currentParent = m_pParent;

	    while (currentParent != nullptr)
	    {
	        finalRotation += currentParent->GetWorldRotation(); // 부모의 최종 Rotation 값을 더함
	        currentParent = currentParent->GetParent();
	    }
	    return finalRotation;
	}
    
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

	void Component_Render(HDC _dc);

	virtual GameObject* Clone() = 0;

protected:
	void SetDead(bool _bDead) { m_bAlive = _bDead; }

	friend class CEventMgr;
};

#pragma once


