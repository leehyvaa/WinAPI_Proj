#pragma once
#include "GameObject.h"
class CTexture;

class CTile :
    public GameObject
{
private:
    CTexture* m_pTileTex;
    int m_iImgIdx;

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }
    void AddImgIdx() { ++m_iImgIdx; }
    void SetImgIdx(int _idx) { m_iImgIdx = _idx; }

    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);


    CLONE(CTile)
public:
    CTile();
    ~CTile();
};

