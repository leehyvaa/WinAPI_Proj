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
	virtual ~SPlayer();

	void CreateWire();
	void SetDir(Vec2 _dir) { m_vDir = _dir; }

	virtual void Update() override;
	virtual void Render(HDC _dc)  override;
};

