#pragma once
#include "CScene.h"
class CScene_Tool : public CScene
{
private:

public:
	CScene_Tool();
	virtual ~CScene_Tool();

	virtual void Update();
	virtual void Enter();
	virtual void Exit();

public:
	void SetTileIdx();

};

