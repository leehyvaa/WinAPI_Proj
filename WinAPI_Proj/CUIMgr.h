#pragma once
class CUI;

class CUIMgr
{

	SINGLE(CUIMgr);


private:

public:
	void Update();

private:
	CUI* GetTargetedUI(CUI* _pParentUI); // �θ� UI ������ ������ Ÿ�ٵ� UI�� ã�Ƽ� ��ȯ
};

