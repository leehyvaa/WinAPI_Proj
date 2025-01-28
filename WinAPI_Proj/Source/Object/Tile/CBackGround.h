#pragma once
#include "GameObject.h"
#include "CTile.h"
class CBackGround :
    public CTile
{
private:

public:

    virtual void Render(HDC _dc);


    CBackGround();
    virtual ~CBackGround();

};

