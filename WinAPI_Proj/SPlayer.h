#pragma once
#include "GameObject.h"

class CTexture;

class SPlayer : public GameObject
{

private:
	CTexture* m_pTex;

public:
	SPlayer();
	SPlayer(Vec2 _vPos, Vec2 _vScale);
	virtual ~SPlayer();

	 

	virtual void Update() override;
	virtual void Render(HDC _dc)  override;
};

