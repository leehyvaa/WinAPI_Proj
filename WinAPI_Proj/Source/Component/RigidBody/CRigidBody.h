#pragma once
class GameObject;

class CRigidBody
{
private:
	GameObject* m_pOwner;
	
	Vec2 m_vForce; //크기, 방향
	Vec2 m_vAccel; //가속도
	Vec2 m_vAccelA; //가속도 추가
	Vec2 m_vVelocity; //속도(크기 :속력, 방향)

	Vec2 m_vMaxVelocity; //최대 속도
	float m_fMass; //질량
	float m_fFricCoeff; //마찰 계수

					  
	//F = M * A
	//V += A * DT

public:
	void FinalUpdate();

	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass() { return m_fMass; }
	float GetSpeed() { return m_vVelocity.Length(); }
	Vec2 GetVelocity() { return m_vVelocity; }


	void AddVelocity(Vec2 _v) { m_vVelocity += _v; }
	void SetVelocity(Vec2 _v) { m_vVelocity = _v; }
	void SetMaxSpeed(Vec2 _v) { m_vMaxVelocity = _v; }
	void SetAccelAlpha(Vec2 _v) { m_vAccelA = _v; }

    void SetVelocityY(float _f) { m_vVelocity.y = _f; }
    void SetVelocityX(float _f) { m_vVelocity.x = _f; }

private:
	void Move();

public:
	CRigidBody();
	~CRigidBody();

	friend class GameObject;
};

