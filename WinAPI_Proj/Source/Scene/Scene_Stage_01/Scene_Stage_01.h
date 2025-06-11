#pragma once
#include "CScene.h"

struct ID2D1RenderTarget;

class Scene_Stage_01 :public CScene
{
private:


public:
	virtual void Update();
	virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;

	virtual void Enter() override;
	virtual void Exit() override;


public:
	Scene_Stage_01();
	~Scene_Stage_01();

};


