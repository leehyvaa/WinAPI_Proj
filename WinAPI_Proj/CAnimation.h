#pragma once

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2 vLT; 
	Vec2 vSlice;//텍스쳐마다 크기가 다를 수 있으니 프레임마다 자를 사이즈
	Vec2 vOffset;
	float fDuration;
};

class CAnimation
{
private:
	wstring m_strName;
	CAnimator* m_pAnimator;

	CTexture* m_pTex; //Animation이 사용하는 텍스쳐
	vector<tAnimFrm> m_vecFrm; //모든 프레임 정보
	int	m_iCurFrm; //현재프레임
	float m_fAccTime; //시간 누적
	bool m_bFinish;

public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0;
	}

	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }
	int GetMaxFrame() { return m_vecFrm.size(); }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void Update();
	void Render(HDC _dc);

	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration,UINT _iFrameCount);


public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

