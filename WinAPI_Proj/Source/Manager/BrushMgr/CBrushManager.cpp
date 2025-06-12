#include "pch.h"
#include "CBrushManager.h"

CBrushManager::CBrushManager()
    : m_pRenderTarget(nullptr)
{
}

CBrushManager::~CBrushManager()
{
    Release();
}

uint32_t CBrushManager::HashBrushProperties(const BrushProperties& props)
{
    // 색상 컴포넌트를 정수로 변환 (0-255 범위)
    uint32_t r = static_cast<uint32_t>(props.color.r * 255.0f) & 0xFF;
    uint32_t g = static_cast<uint32_t>(props.color.g * 255.0f) & 0xFF;
    uint32_t b = static_cast<uint32_t>(props.color.b * 255.0f) & 0xFF;
    uint32_t a = static_cast<uint32_t>(props.color.a * 255.0f) & 0xFF;
    
    // 투명도를 정수로 변환
    uint32_t opacity = static_cast<uint32_t>(props.opacity * 255.0f) & 0xFF;
    
    // 해시값 생성 (RGBA + opacity를 조합)
    uint32_t hash = (r << 24) | (g << 16) | (b << 8) | a;
    hash ^= (opacity << 16); // 투명도 추가 혼합
    
    // 추가 속성이 있다면 해시에 반영
    if (props.properties.opacity != 1.0f)
    {
        uint32_t propOpacity = static_cast<uint32_t>(props.properties.opacity * 255.0f) & 0xFF;
        hash ^= (propOpacity << 8);
    }
    
    return hash;
}

void CBrushManager::SafeReleaseBrush(ID2D1SolidColorBrush*& pBrush)
{
    if (!pBrush) return;
    
    pBrush->Release();
    pBrush = nullptr;
}

void CBrushManager::Initialize(ID2D1RenderTarget* pRenderTarget)
{
    if (!pRenderTarget)
        return;

    m_pRenderTarget = pRenderTarget;
    
    // 기본 브러시들을 미리 생성 (성능 최적화)
    GetBrush(BrushType::BLACK);
    GetBrush(BrushType::RED);
    GetBrush(BrushType::GREEN);
    GetBrush(BrushType::BLUE);
}

void CBrushManager::Release()
{
    m_pRenderTarget = nullptr;
}

void CBrushManager::OnDeviceLost()
{
    if (m_brushCache.empty())
    {
        m_pRenderTarget = nullptr;
        return;
    }

    // 맵 해제
    while (!m_brushCache.empty())
    {
        auto it = m_brushCache.begin();
        if (it != m_brushCache.end())
        {
            ID2D1SolidColorBrush* pBrush = it->second;
            m_brushCache.erase(it);

            if (pBrush)
                pBrush->Release();
        }
        else
            break;
    }

    m_brushCache.clear();
    m_pRenderTarget = nullptr;
}

void CBrushManager::OnDeviceRestored(ID2D1RenderTarget* pRenderTarget)
{
    m_pRenderTarget = pRenderTarget;
    // 브러시는 요청 시 지연 생성됨
}

ID2D1SolidColorBrush* CBrushManager::GetBrush(BrushType type)
{
    BrushProperties props(type);
    return GetBrush(props);
}

ID2D1SolidColorBrush* CBrushManager::GetBrush(const BrushProperties& props)
{
    if (!m_pRenderTarget)
    {
        return nullptr;
    }
    
    // 해시 키 생성
    uint32_t hash = HashBrushProperties(props);
    
    // 캐시에서 브러시 검색
    auto it = m_brushCache.find(hash);
    if (it != m_brushCache.end() && it->second)
    {
        return it->second; // 캐시 히트
    }
    
    // 캐시 미스 - 새 브러시 생성
    ID2D1SolidColorBrush* pBrush = nullptr;
    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(props.color, props.properties, &pBrush);
    
    if (SUCCEEDED(hr) && pBrush)
    {
        // 캐시에 저장
        m_brushCache[hash] = pBrush;
        return pBrush;
    }
    else
    {
        return nullptr;
    }
}

ID2D1SolidColorBrush* CBrushManager::GetCustomBrush(float r, float g, float b, float a)
{
    BrushProperties props(r, g, b, a);
    return GetBrush(props);
}

void CBrushManager::ClearCache()
{
    // 캐시 클리어
    if (!m_brushCache.empty())
    {
        // 맵 해제
        while (!m_brushCache.empty())
        {
            auto it = m_brushCache.begin();
            if (it != m_brushCache.end())
            {
                ID2D1SolidColorBrush* pBrush = it->second;
                m_brushCache.erase(it);

                if (pBrush)
                    pBrush->Release();
            }
            else
                break;
        }
        m_brushCache.clear();
    }

    // 기본 브러시들 재생성
    if (m_pRenderTarget)
    {
        GetBrush(BrushType::BLACK);
        GetBrush(BrushType::RED);
        GetBrush(BrushType::GREEN);
        GetBrush(BrushType::BLUE);
    }
}

size_t CBrushManager::GetCacheSize() const
{
    return m_brushCache.size();
}