#pragma once
#include "GameObject.h"
#include "CTile.h"

// Direct2D 전방 선언
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class CBackGround :
    public CTile
{
private:
    ID2D1Bitmap* m_pD2DBitmap;  // Direct2D 비트맵

public:
    virtual void Render(HDC _dc);
    virtual void RenderD2D(ID2D1RenderTarget* _pRenderTarget);
    void CacheD2DBitmap(ID2D1RenderTarget* _pRenderTarget);
    void ReleaseD2DBitmap();

    CBackGround();
    virtual ~CBackGround();

};

