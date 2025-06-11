#pragma once
struct ID2D1RenderTarget;
class GameObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*> m_mapAnim; // 모든 Animation
	CAnimation* m_pCurAnim; // 현재 재생중 Animation
	GameObject* m_pOwner;
	bool m_bRepeat;
	CTexture* tempTex;


public:
	GameObject* GetObj() { return m_pOwner; }
	CTexture* GetTempTex() { return tempTex; }

	void CreateAnimation(const wstring& _strName,CTexture* _pTex, Vec2 _vLT ,
						Vec2 _vSliceSize, Vec2 _vStep, float _fDuration,UINT _iFrameCount, float _fSizeMulti, Vec2 _vOffset);
	void LoadAnimation(const wstring& _strRelativePath);
	
    Vec2 GetCurrentAnimationOffset();
 
	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName,bool _bRepeat);
    CAnimation* GetCurAnimation(){return m_pCurAnim;}
    
	void Update();
	void FinalUpdate();
	void Render(ID2D1RenderTarget* _pRenderTarget); 

    void Reset();
public:
	CAnimator();
	~CAnimator();

	friend class GameObject;
};