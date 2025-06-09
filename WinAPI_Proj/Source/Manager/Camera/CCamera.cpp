#include "pch.h"
#include "CCamera.h"

#include "GameObject.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CTexture.h"

CCamera::CCamera()
	:m_pTargetObj(nullptr)
	,m_fTime(0.5f)
	,m_fAccTime(0.5f)
	,m_fSpeed(0)
	,m_pVeilTex(nullptr)
{
}

CCamera::~CCamera()
{

}


void CCamera::Update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead() || !m_pTargetObj->IsActive()) 
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			m_vLookAt = m_pTargetObj->GetWorldPos() + Vec2(0.f,-200.f);
		}

	}

	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;


	//화면 중앙좌표와 카메라 LookAt 좌표간의 차이값 계산
	CalDiff();
}

void CCamera::Render(HDC _dc)
{
	if (m_listCamEffect.empty())
		return;

	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;





	float fRatio = 0.f; //이펙트 진행 비율
	fRatio = effect.fCurTime / effect.fDuration;

	if (fRatio < 0.f)
		fRatio = 0.f;
	if (fRatio > 1.f)
		fRatio = 1.f;


	int iAlpha = 0;

	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
	{
		iAlpha = static_cast<int>(255.f * fRatio);
	}
	else if (CAM_EFFECT::FADE_IN == effect.eEffect)
	{
		iAlpha = static_cast<int>(255.f * (1.f - fRatio));
	}


	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha; //전역적으로 적용되는 알파

	AlphaBlend(_dc
		, 0
		, 0
		, static_cast<int>(m_pVeilTex->Width()), static_cast<int>(m_pVeilTex->Height())
		, m_pVeilTex->GetDC()
		, 0, 0, static_cast<int>(m_pVeilTex->Width()), static_cast<int>(m_pVeilTex->Height())
		, bf);


	//진행시간이 이펙트 지속시간을 넘어서면 스톱
	if (effect.fDuration < effect.fCurTime)
	{
		m_listCamEffect.pop_front();
	}
}

void CCamera::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
    if (m_listCamEffect.empty() || !_pRenderTarget)
        return;

    tCamEffect& effect = m_listCamEffect.front();
    effect.fCurTime += fDT;

    float fRatio = 0.f; // 이펙트 진행 비율
    fRatio = effect.fCurTime / effect.fDuration;

    if (fRatio < 0.f)
        fRatio = 0.f;
    if (fRatio > 1.f)
        fRatio = 1.f;

    float fAlpha = 0.f;

    if (CAM_EFFECT::FADE_OUT == effect.eEffect)
    {
        fAlpha = fRatio;
    }
    else if (CAM_EFFECT::FADE_IN == effect.eEffect)
    {
        fAlpha = 1.f - fRatio;
    }

    // Direct2D로 페이드 효과 렌더링
    static ID2D1SolidColorBrush* s_pFadeBrush = nullptr;
    if (!s_pFadeBrush)
    {
        _pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &s_pFadeBrush);
    }

    if (s_pFadeBrush)
    {
        s_pFadeBrush->SetOpacity(fAlpha);
        Vec2 vResolution = CCore::GetInst()->GetResolution();
        D2D1_RECT_F rect = D2D1::RectF(0, 0, vResolution.x, vResolution.y);
        _pRenderTarget->FillRectangle(&rect, s_pFadeBrush);
    }

    // 진행시간이 이펙트 지속시간을 넘어서면 스톱
    if (effect.fDuration < effect.fCurTime)
    {
        m_listCamEffect.pop_front();
    }
}

void CCamera::init()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	m_pVeilTex = CResMgr::GetInst()->CreateTexture(L"CameraVeil", static_cast<UINT>(vResolution.x), static_cast<UINT>(vResolution.y));

}



void CCamera::CalDiff()
{
	//이전 LookAt 과 현재 Look의 차이값을 보정해서 현재의 LookAt을 구한다.
	m_fAccTime += fDT;

	if (m_fAccTime >= m_fTime)
	{
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;
		}
	}


	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;

}

void CCamera::FadeIn(float _fDuration)
{
	tCamEffect ef = {};
	ef.eEffect = CAM_EFFECT::FADE_IN;
	ef.fDuration = _fDuration;
	ef.fCurTime = 0.f;

	m_listCamEffect.push_back(ef);

	if (0.f == _fDuration)
		assert(nullptr);
}

void CCamera::FadeOut(float _fDuration)
{
	tCamEffect ef = {};
	ef.eEffect = CAM_EFFECT::FADE_OUT;
	ef.fDuration = _fDuration;
	ef.fCurTime = 0.f;

	m_listCamEffect.push_back(ef);

	if (0.f == _fDuration)
		assert(nullptr);
}
//카메라 빠르게 움직이다 목표지점 도착할때쯤 속도를 줄여서 도착하게끔 하는 개선 가능
//카메라 쉐이킹 기능등 