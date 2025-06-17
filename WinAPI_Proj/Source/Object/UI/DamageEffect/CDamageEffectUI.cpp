#include "CDamageEffectUI.h"
#include "../../../Manager/TimeMgr/CTimeMgr.h"
#include "../../../Core/CCore.h"
#include <d2d1.h>

CDamageEffectUI::CDamageEffectUI()
    : CUI(false)  // 카메라 영향 받지 않음 (화면 전체 오버레이)
    , m_eFadeState(FADE_STATE::INACTIVE)
    , m_fCurrentOpacity(0.0f)
    , m_fMaxOpacity(0.1f)           // 투명도
    , m_fDisplayDuration(0.35f)     
    , m_fFadeSpeed(4.0f)            
    , m_fDisplayTimer(0.0f)
    , m_fDeltaTime(0.0f)
    , m_DamageColor(D2D1::ColorF(D2D1::ColorF::Red, 1.0f))  // 빨간색
{
    // 화면 전체를 덮도록 크기 설정
    HWND hWnd = CCore::GetInst()->GetMainHwnd();
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    
    SetScale(Vec2((float)(clientRect.right - clientRect.left), 
                  (float)(clientRect.bottom - clientRect.top)));
    SetWorldPos(Vec2(0.0f, 0.0f));
    SetVisibleBox(false);
}


CDamageEffectUI::~CDamageEffectUI()
{
}


CDamageEffectUI::CDamageEffectUI(const CDamageEffectUI& _origin)
    : CUI(_origin)
    , m_eFadeState(FADE_STATE::INACTIVE)
    , m_fCurrentOpacity(0.0f)
    , m_fMaxOpacity(_origin.m_fMaxOpacity)
    , m_fDisplayDuration(_origin.m_fDisplayDuration)
    , m_fFadeSpeed(_origin.m_fFadeSpeed)
    , m_fDisplayTimer(0.0f)
    , m_fDeltaTime(0.0f)
    , m_DamageColor(_origin.m_DamageColor)
{
    SetVisibleBox(false);
}


void CDamageEffectUI::Update()
{
    CUI::Update();
    
    m_fDeltaTime = CTimeMgr::GetInst()->GetfDT();
    
    // 페이드 애니메이션 업데이트
    if (m_eFadeState != FADE_STATE::INACTIVE)
    {
        UpdateFadeAnimation();
    }
}


void CDamageEffectUI::FinalUpdate()
{
    CUI::FinalUpdate();
}


void CDamageEffectUI::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget || m_eFadeState == FADE_STATE::INACTIVE || m_fCurrentOpacity <= 0.0f)
        return;
    
    // 데미지 오버레이 렌더링
    RenderDamageOverlay(_pRenderTarget);
    
    CUI::Render(_pRenderTarget);
}


CUI* CDamageEffectUI::Clone()
{
    return new CDamageEffectUI(*this);
}


void CDamageEffectUI::OnPlayerDamaged()
{
    m_eFadeState = FADE_STATE::FADE_IN;
    m_fCurrentOpacity = 0.0f;
    m_fDisplayTimer = 0.0f;
}


void CDamageEffectUI::ResetEffect()
{
    m_eFadeState = FADE_STATE::INACTIVE;
    m_fCurrentOpacity = 0.0f;
    m_fDisplayTimer = 0.0f;
}


bool CDamageEffectUI::IsEffectActive() const
{
    return m_eFadeState != FADE_STATE::INACTIVE;
}


void CDamageEffectUI::SetMaxOpacity(float _fOpacity)
{
    m_fMaxOpacity = max(0.0f, min(1.0f, _fOpacity));
}


void CDamageEffectUI::SetDisplayDuration(float _fDuration)
{
    m_fDisplayDuration = max(0.0f, _fDuration);
}


void CDamageEffectUI::SetFadeSpeed(float _fSpeed)
{
    m_fFadeSpeed = max(0.1f, _fSpeed);
}


void CDamageEffectUI::SetDamageColor(D2D1_COLOR_F _color)
{
    m_DamageColor = _color;
}


void CDamageEffectUI::UpdateFadeAnimation()
{
    switch (m_eFadeState)
    {
    case FADE_STATE::FADE_IN:
        m_fCurrentOpacity += m_fFadeSpeed * m_fDeltaTime;
        if (m_fCurrentOpacity >= m_fMaxOpacity)
        {
            m_fCurrentOpacity = m_fMaxOpacity;
            m_eFadeState = FADE_STATE::DISPLAY;
            m_fDisplayTimer = 0.0f;
        }
        break;
        
    case FADE_STATE::DISPLAY:
        m_fDisplayTimer += m_fDeltaTime;
        if (m_fDisplayTimer >= m_fDisplayDuration)
        {
            m_eFadeState = FADE_STATE::FADE_OUT;
        }
        break;
        
    case FADE_STATE::FADE_OUT:
        m_fCurrentOpacity -= m_fFadeSpeed * m_fDeltaTime;
        if (m_fCurrentOpacity <= 0.0f)
        {
            m_fCurrentOpacity = 0.0f;
            m_eFadeState = FADE_STATE::INACTIVE;
        }
        break;
        
    case FADE_STATE::INACTIVE:
    default:
        break;
    }
}


void CDamageEffectUI::RenderDamageOverlay(ID2D1RenderTarget* _pRenderTarget)
{
    if (m_fCurrentOpacity <= 0.0f)
        return;
    
    ID2D1SolidColorBrush* pBrush = nullptr;
    
    D2D1_COLOR_F overlayColor = m_DamageColor;
    overlayColor.a = m_fCurrentOpacity;
    
    // 브러시 생성
    HRESULT hr = _pRenderTarget->CreateSolidColorBrush(overlayColor, &pBrush);
    if (SUCCEEDED(hr) && pBrush)
    {
        // 화면 전체 크기의 사각형
        Vec2 vPos = GetFinalPos();
        Vec2 vScale = GetScale();
        
        D2D1_RECT_F overlayRect = D2D1::RectF(
            vPos.x,
            vPos.y,
            vPos.x + vScale.x,
            vPos.y + vScale.y
        );
        
        // 오버레이 렌더링
        _pRenderTarget->FillRectangle(overlayRect, pBrush);
        
        pBrush->Release();
    }
}