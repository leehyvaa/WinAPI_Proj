#include "pch.h"
#include "CAnimator.h"
#include "CAnimation.h"
CAnimator::CAnimator()
	:m_pOwner(nullptr)
	,m_pCurAnim(nullptr)
{
}

CAnimator::~CAnimator()
{
	Safe_Delete_Map(m_mapAnim);
}





void CAnimator::Update()
{
	if (nullptr != m_pCurAnim)
		m_pCurAnim->Update();
}

void CAnimator::Render(HDC _dc)
{
	if (nullptr != m_pCurAnim)
		m_pCurAnim->Render(_dc);
}


void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT,
					Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep, _fDuration,_iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring,CAnimation*>::iterator iter = m_mapAnim.find(_strName);
	
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}


void CAnimator::Play()
{
}