#pragma once
#include "GameObject.h"
class MouseCursor :
    public GameObject
{
private:

public:
    MouseCursor();
    ~MouseCursor();


    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    CLONE(MouseCursor)
};

