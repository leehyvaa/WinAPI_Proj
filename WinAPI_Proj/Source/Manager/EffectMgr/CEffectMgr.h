#pragma once

struct EffectData
{
    wstring animName;
    wstring animPath;
    // 나중에 추가될 수 있는 속성들
    // wstring soundPath;
    // float scale;
};

class CEffectManager
{
    SINGLE(CEffectManager)

private:
    map<wstring, EffectData> m_mapEffectData;

public:
    void Init();
    void Play(const wstring& _effectName, Vec2 _vPos);

private:
    void RegisterEffect(const wstring& _effectName, const wstring& _animName, const wstring& _animPath);
};