#pragma once
#include "GameObject.h"

class CTexture;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class CTile :
    public GameObject
{
private:
    CTexture* m_pTileTex;
    int m_iImgIdx;
    CTexture* m_pTileTex2;
    int m_iImgIdx2;
    int m_iBotRightTileIdx;

    VERTEX_POSITION m_eVertexPosition;
    GROUND_TYPE m_eGroundType; // 지형 타입 필드

    
public:
    
    virtual void Update();
    virtual void Render(ID2D1RenderTarget* _pRenderTarget);

private:

public:

    virtual void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }
    virtual CTexture* GetTexture() { return m_pTileTex; }
    void AddImgIdx() { ++m_iImgIdx; }
    void SetImgIdx(int _idx) { m_iImgIdx = _idx; }

    virtual void SetTextureTwo(CTexture* _pTex) { m_pTileTex2 = _pTex; }
    virtual CTexture* GetTextureTwo() { return m_pTileTex2; }
    void SetImgIdxTwo(int _idx) { m_iImgIdx2 = _idx; }

    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);

    
    GROUND_TYPE GetGroundType() { return m_eGroundType; }
    void SetGroundType(GROUND_TYPE _type){ m_eGroundType = _type; }
    VERTEX_POSITION GetVertexPosition() { return m_eVertexPosition; }
    void SetVertexPosition(VERTEX_POSITION _pos) { m_eVertexPosition = _pos; }

    
    int GetBotRightTileIdx() { return m_iBotRightTileIdx; }
    void SetBotRightTileIdx(int _idx) { m_iBotRightTileIdx = _idx; }
    
    virtual void OnCollisionEnter(CCollider* _pOther);

    
    CLONE(CTile)
public:
    CTile();
    virtual ~CTile();
};

