#pragma once
#include "GameObject.h"
#include "CTile.h"

// Direct2D 전방 선언
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class CBackGround :
    public CTile
{
public:
    virtual void Render(HDC _dc);
    virtual void RenderD2D(ID2D1RenderTarget* _pRenderTarget);

    CBackGround();
    virtual ~CBackGround();

};

