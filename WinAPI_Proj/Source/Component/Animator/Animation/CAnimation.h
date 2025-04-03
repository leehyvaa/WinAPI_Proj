#pragma once

namespace Gdiplus
{
    class Bitmap;
}

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
	bool m_bFinish; //재생 끝에 도달 여부
	float m_fSizeMulti;
	POINT* rotPos;
    Vec2 m_vOffset;
    
    vector<Gdiplus::Bitmap*> m_vecFrameBitmaps; // 미리 처리된 프레임 비트맵들
    bool m_bCached; // 캐싱 여부

    
public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
    void SetFinish(bool _bFinish) { m_bFinish = _bFinish; }
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0;
	}

	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }
	int GetMaxFrame() { return static_cast<UINT>(m_vecFrm.size()); }
    Vec2 GetOffset() { return m_vOffset; }

    // 현재 프레임의 오프셋 반환
    Vec2 GetCurrentFrameOffset() const {
	    if (m_vecFrm.empty() || m_iCurFrm < 0 || m_iCurFrm >= m_vecFrm.size())
	        return Vec2(0.f, 0.f);
	    return m_vecFrm[m_iCurFrm].vOffset;}
    
    // 현재 프레임 인덱스 반환
    int GetCurrentFrame() const {return m_iCurFrm;}
private:
	void SetName(const wstring& _strName) { m_strName = _strName; }
    void CacheFrames();
public:
	void Update();
	void Render(HDC _dc);

	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep,
		float _fDuration,UINT _iFrameCount, float _fSizeMulti, Vec2 _vOffset);

	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

