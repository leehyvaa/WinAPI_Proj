#pragma once
#include <d2d1.h>
#include <unordered_map>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// 브러시 타입 정의
enum class BrushType : uint32_t
{
    // 기본 색상 (기존 호환성)
    BLACK = 0x000000FF,
    RED   = 0xFF0000FF,
    GREEN = 0x00FF00FF,
    BLUE  = 0x0000FFFF,
    
    // 확장 색상
    WHITE = 0xFFFFFFFF,
    YELLOW = 0xFFFF00FF,
    CYAN = 0x00FFFFFF,
    MAGENTA = 0xFF00FFFF,
    
    // 시스템 색상
    GRID = 0x808080FF,
    DEBUG = 0xFF8000FF,
    UI_BACKGROUND = 0x2D2D30FF,
    UI_BORDER = 0x3F3F46FF,
    
    // 투명도가 있는 색상
    FADE_BLACK = 0x00000080,
    HIGHLIGHT = 0xFFFF0080,
    
    // 커스텀 범위 시작
    CUSTOM_START = 0x80000000
};

// 브러시 속성 구조체
struct BrushProperties
{
    D2D1_COLOR_F color;
    float opacity = 1.0f;
    D2D1_BRUSH_PROPERTIES properties = D2D1::BrushProperties();
    
    BrushProperties() = default;
    
    BrushProperties(BrushType type)
    {
        uint32_t colorValue = static_cast<uint32_t>(type);
        color.r = ((colorValue >> 24) & 0xFF) / 255.0f;
        color.g = ((colorValue >> 16) & 0xFF) / 255.0f;
        color.b = ((colorValue >> 8) & 0xFF) / 255.0f;
        color.a = (colorValue & 0xFF) / 255.0f;
        opacity = color.a;
    }
    
    BrushProperties(float r, float g, float b, float a = 1.0f)
        : color{r, g, b, a}, opacity(a) {}
    
    BrushProperties(D2D1::ColorF::Enum predefinedColor, float alpha = 1.0f)
        : color(D2D1::ColorF(predefinedColor, alpha)), opacity(alpha) {}
};

// 브러시 매니저 클래스
class CBrushManager
{
    SINGLE(CBrushManager)
    
private:
    unordered_map<uint32_t, ID2D1SolidColorBrush*> m_brushCache;
    ID2D1RenderTarget* m_pRenderTarget;
    
    // 해시 함수 (BrushProperties -> uint32_t)
    uint32_t HashBrushProperties(const BrushProperties& props);
    
    // 안전한 브러시 해제 도우미 함수
    void SafeReleaseBrush(ID2D1SolidColorBrush*& pBrush);
    
public:
    void Initialize(ID2D1RenderTarget* pRenderTarget);
    void Release();
    void OnDeviceLost();
    void OnDeviceRestored(ID2D1RenderTarget* pRenderTarget);
    
    ID2D1SolidColorBrush* GetBrush(BrushType type);
    ID2D1SolidColorBrush* GetBrush(const BrushProperties& props);
    ID2D1SolidColorBrush* GetCustomBrush(float r, float g, float b, float a = 1.0f);
    
    void ClearCache();
    size_t GetCacheSize() const;
    
    ID2D1SolidColorBrush* GetBlackBrush() { return GetBrush(BrushType::BLACK); }
    ID2D1SolidColorBrush* GetRedBrush() { return GetBrush(BrushType::RED); }
    ID2D1SolidColorBrush* GetGreenBrush() { return GetBrush(BrushType::GREEN); }
    ID2D1SolidColorBrush* GetBlueBrush() { return GetBrush(BrushType::BLUE); }
};