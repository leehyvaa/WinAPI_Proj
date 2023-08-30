#pragma once
#include "CScene.h"
class CScene_Title :
    public CScene
{

public:
	virtual void Update();


	virtual void Enter() override;
	virtual void Exit() override;



public:
	CScene_Title();
	~CScene_Title();
};

