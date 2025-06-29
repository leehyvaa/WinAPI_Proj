#pragma once
#include "GameObject.h"

class CEffect : public GameObject
{
public:
    CEffect();
    ~CEffect();

    virtual void Update() override;
    virtual void Reset() override;

    // 이펙트 재생을 시작하는 함수
    void PlayEffect(const wstring& _animName, bool _bRepeat = false);

    CLONE(CEffect);
};