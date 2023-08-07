#pragma once
class GameObject;

class CRigidBody
{
private:
	GameObject* m_pOwner;
	
	Vec2 m_vForce; //ũ��, ����
	Vec2 m_vAccel; //���ӵ�
	Vec2 m_vAccelA; //���ӵ� �߰�
	Vec2 m_vVelocity; //�ӵ�(ũ�� :�ӷ�, ����)

	Vec2 m_vMaxVelocity; //�ִ� �ӵ�
	float m_fMass; //����
	float m_fFricCoeff; //���� ���

					  
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


private:
	void Move();

public:
	CRigidBody();
	~CRigidBody();

	friend class GameObject;
};

