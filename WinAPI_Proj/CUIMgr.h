#pragma once
class CUI;

class CUIMgr
{

	SINGLE(CUIMgr);


private:

public:
	void Update();

private:
	CUI* GetTargetedUI(CUI* _pParentUI); // 부모 UI 내에서 실제로 타겟된 UI를 찾아서 반환
};

