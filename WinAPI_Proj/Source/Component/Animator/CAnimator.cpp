#include "pch.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCore.h"
#include "CTexture.h"
#include "CResMgr.h"
CAnimator::CAnimator()
	:m_pOwner(nullptr)
	,m_pCurAnim(nullptr)
	,m_bRepeat(false)
	,tempTex(nullptr)
{
	tempTex = CResMgr::GetInst()->LoadTexture(L"AnimatorTemp", L"texture\\player\\Grab_Right.png");


}

CAnimator::~CAnimator()
{
	Safe_Delete_Map(m_mapAnim);
}





void CAnimator::Update()
{
	
}

void CAnimator::FinalUpdate()
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->Update();

		//c는 앞의 조건이 거짓이면 뒤의 조건은 보지도 않고 패스한다
		//이를 이용한 프로그래밍 방식을 고민할것
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::Render(ID2D1RenderTarget* _pRenderTarget)
{
	if (nullptr != m_pCurAnim && nullptr != _pRenderTarget)
	{
		m_pCurAnim->Render(_pRenderTarget);
	}
}

void CAnimator::Reset()
{
    m_pCurAnim = nullptr;
    m_bRepeat = false;
        
    // 각 애니메이션 초기화
    for (auto& pair : m_mapAnim)
    {
        if (pair.second)
        {
            pair.second->SetFrame(0);
            pair.second->SetFinish(false);
        }
    }
}


void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT,
                                Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount, float _fSizeMulti, Vec2 _vOffset)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep, _fDuration,_iFrameCount,_fSizeMulti,_vOffset);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::LoadAnimation(const wstring& _strRelativePath)
{
	CAnimation* pAnim = new CAnimation;
	pAnim->Load(_strRelativePath);

	pAnim->m_pAnimator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

Vec2 CAnimator::GetCurrentAnimationOffset()
{
    if (m_pCurAnim)
        return m_pCurAnim->GetCurrentFrameOffset();
    return Vec2(0.f, 0.f);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring,CAnimation*>::iterator iter = m_mapAnim.find(_strName);
	
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
    CAnimation* pAnim = FindAnimation(_strName);
    
    // 이미 같은 애니메이션이 재생 중이면 무시
    if (m_pCurAnim == pAnim)
        return;
        
    m_pCurAnim = pAnim;
    m_bRepeat = _bRepeat;
    
    // 새로운 애니메이션 시작 시 초기화
    if (m_pCurAnim)
    {
        m_pCurAnim->SetFrame(0);
        m_pCurAnim->SetFinish(false);
    }
}


