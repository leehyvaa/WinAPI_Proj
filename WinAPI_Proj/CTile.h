#pragma once
#include "GameObject.h"
class CTexture;

class CTile :
    public GameObject
{
private:
    CTexture* m_pTileTex;
    int m_iImgIdx;
    CTexture* m_pTileTex2;
    int m_iImgIdx2;


public:
    virtual void Update();
    virtual void Render(HDC _dc);

    virtual void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }
    virtual CTexture* GetTexture() { return m_pTileTex; }
    void AddImgIdx() { ++m_iImgIdx; }
    void SetImgIdx(int _idx) { m_iImgIdx = _idx; }

    virtual void SetTextureTwo(CTexture* _pTex) { m_pTileTex2 = _pTex; }
    virtual CTexture* GetTextureTwo() { return m_pTileTex2; }
    void SetImgIdxTwo(int _idx) { m_iImgIdx2 = _idx; }

    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);


    CLONE(CTile)
public:
    CTile();
    virtual ~CTile();
};

