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

	// 부모와 자식부분을 따로 복사하는 복사생성자 아래 내용을 안적어도 아래와 같이 
	//컴파일러가 자동으로 구현해줌
	//복사생성자가 문제가 되는 경우는 콜라이더처럼 자신의 owner를 가지고 있거나
	//고유한 id값을 가지고 있을때 얕은복사를 하면 문제가 됨
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

	//자신의 데이터를 복사해서 하나 새로 생성한 후 이를 반환하는 클론함수
	//virtual GameObject* Clone() { return new SPlayer(*this); }
	CLONE(SPlayer)
};

