#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"

CScene_Tool::CScene_Tool()
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::Update()
{
	CScene::Update();

	if (KEY_TAP(KEY::T))
		ChangeScene(SCENE_TYPE::START);
}

void CScene_Tool::Enter()
{
	//타일 생성
}

void CScene_Tool::Exit()
{
}
