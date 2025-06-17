#pragma once
#include "CScene.h"

struct ID2D1RenderTarget;

class Scene_Stage_01 :public CScene
{
private:
	bool m_bPlayerDeathMode;  // 플레이어 죽음 모드 플래그
	float m_fDeathSceneTimer;  // 죽음 애니메이션 완료 후 씬 전환 타이머

public:
	virtual void Update();
	virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;

	virtual void Enter() override;
	virtual void Exit() override;


public:
	Scene_Stage_01();
	~Scene_Stage_01();

};


