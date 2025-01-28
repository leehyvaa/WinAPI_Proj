#pragma once
#include "CScene.h"
class Scene_Stage_01 :public CScene
{
private:


public:
	virtual void Update();
	virtual void Render(HDC _dc);

	virtual void Enter() override;
	virtual void Exit() override;


public:
	Scene_Stage_01();
	~Scene_Stage_01();

};


