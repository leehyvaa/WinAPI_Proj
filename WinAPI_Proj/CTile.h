#pragma once
#include "GameObject.h"
class CTile :
    public GameObject
{
private:
    virtual void Render(HDC _dc);

public:
    CTile();
    ~CTile();
};

