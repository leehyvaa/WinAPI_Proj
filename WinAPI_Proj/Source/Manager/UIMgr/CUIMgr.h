﻿#pragma once
class CUI;

class CUIMgr
{

	SINGLE(CUIMgr);


private:
	CUI* m_pFocusedUI;

public:
	void Update();

	void SetFocusedUI(CUI* _pUI);

private:
	CUI* GetFocusedUI();
	CUI* GetTargetedUI(CUI* _pParentUI); // 부모 UI 내에서 실제로 타겟된 UI를 찾아서 반환
};

