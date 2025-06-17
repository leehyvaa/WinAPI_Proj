#pragma once
#include "../CUI.h"

class CDamageEffectUI : public CUI
{
private:
    enum class FADE_STATE
    {
        INACTIVE,      
        FADE_IN,       
        DISPLAY,       
        FADE_OUT       
    };

    FADE_STATE m_eFadeState;       
    float m_fCurrentOpacity;       // 현재 투명도
    float m_fMaxOpacity;           // 최대 투명도
    float m_fDisplayDuration;      // 지속 시간
    float m_fFadeSpeed;            // 페이드 속도
    
    float m_fDisplayTimer;          
    float m_fDeltaTime;             
    
    D2D1_COLOR_F m_DamageColor;     

public:
    CDamageEffectUI();
    virtual ~CDamageEffectUI();
    
    CDamageEffectUI(const CDamageEffectUI& _origin);
    
    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;
    virtual CUI* Clone() override;


    void OnPlayerDamaged();         
    void ResetEffect();             
    bool IsEffectActive() const;    
    
    void SetMaxOpacity(float _fOpacity);        
    void SetDisplayDuration(float _fDuration);  
    void SetFadeSpeed(float _fSpeed);           
    void SetDamageColor(D2D1_COLOR_F _color);   
    
    float GetMaxOpacity() { return m_fMaxOpacity; }
    float GetDisplayDuration() { return m_fDisplayDuration; }
    float GetFadeSpeed() { return m_fFadeSpeed; }
    float GetCurrentOpacity() { return m_fCurrentOpacity; }

private:
    void UpdateFadeAnimation();    
    void RenderDamageOverlay(ID2D1RenderTarget* _pRenderTarget);
};