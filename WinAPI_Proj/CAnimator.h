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
	bool m_bRepeat;

public:
	GameObject* GetObj() { return m_pOwner; }


	void CreateAnimation(const wstring& _strName,CTexture* _pTex, Vec2 _vLT ,
						Vec2 _vSliceSize, Vec2 _vStep, float _fDuration,UINT _iFrameCount);
	void LoadAnimation(const wstring& _strRelativePath);
	
	
	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName,bool _bRepeat);

	void Update();
	void FinalUpdate();
	void Render(HDC _dc);

public:
	CAnimator();
	~CAnimator();

	friend class GameObject;
};

