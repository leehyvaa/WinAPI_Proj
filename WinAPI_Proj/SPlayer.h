#pragma once
#include "GameObject.h"

class CTexture;

class SPlayer : public GameObject
{

private:
	CTexture* m_pTex;
	Vec2 m_vDir;
	float m_fSpeed;
public:
	SPlayer();

	// �θ�� �ڽĺκ��� ���� �����ϴ� ��������� �Ʒ� ������ ����� �Ʒ��� ���� 
	//�����Ϸ��� �ڵ����� ��������
	//��������ڰ� ������ �Ǵ� ���� �ݶ��̴�ó�� �ڽ��� owner�� ������ �ְų�
	//������ id���� ������ ������ �������縦 �ϸ� ������ ��
	SPlayer(const SPlayer& _origin) 
		:GameObject(_origin)
		,m_pTex(_origin.m_pTex)
		,m_vDir(_origin.m_vDir)
		,m_fSpeed(_origin.m_fSpeed)
	{}
	virtual ~SPlayer();


	void SetDir(Vec2 _dir) { m_vDir = _dir; }


	virtual void Update() override;
	virtual void Render(HDC _dc)  override;

private:
	void CreateWire();

	//�ڽ��� �����͸� �����ؼ� �ϳ� ���� ������ �� �̸� ��ȯ�ϴ� Ŭ���Լ�
	//virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)
};

