#pragma once
#include "GameObject.h"

struct ID2D1RenderTarget; // Direct2D 전방 선언

class MouseCursor :
    public GameObject
{
private:

public:
    MouseCursor();
    ~MouseCursor();

    virtual void Update() override;
    virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;

    CLONE(MouseCursor)
};

