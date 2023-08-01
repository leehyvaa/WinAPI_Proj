#pragma once
class GameObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*> m_mapAnim; //모든 Animation
	CAnimation* m_pCurAnim; //현재 재생중 Animation
	GameObject* m_pOwner;

public:
	void CreateAnimation(const wstring& _strName,CTexture* _pTex, Vec2 _vLT ,
						Vec2 _vSliceSize, Vec2 _vStep, float _fDuration,UINT _iFrameCount);
	CAnimation* FindAnimation(const wstring& _strName);
	void Play();

	void Update();
	void Render(HDC _dc);

public:
	CAnimator();
	~CAnimator();

	friend class GameObject;
};

