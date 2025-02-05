#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CSound.h"
CResMgr::CResMgr()
{

}
CResMgr::~CResMgr()
{
    /*map<wstring, CTexture*>::iterator iter = m_mapTex.begin();
    for (; iter != m_mapTex.end() ; iter++)
    {
        delete iter->second;
    } 아래 템플릿함수로 대체 */

    Safe_Delete_Map(m_mapTex);
}



CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
    CTexture* pTex = FindTexture(_strKey);
    if (nullptr != pTex) //텍스처가 이미 존재할 경우 만들지 않고 찾아서 줌
        return pTex;

  

    pTex = new CTexture;
    pTex->Create(_iWidth, _iHeight);
    pTex->SetKey(_strKey);

    m_mapTex.insert(make_pair(_strKey, pTex));


    return pTex;
}




CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
    CTexture* pTex = FindTexture(_strKey);
    if (nullptr != pTex) //텍스처가 이미 존재할 경우 만들지 않고 찾아서 줌
        return pTex;

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    pTex = new CTexture;
    pTex->Load(strFilePath);
    pTex->SetKey(_strKey);
    pTex->SetRelativePath(_strRelativePath);

    m_mapTex.insert(make_pair(_strKey, pTex));

    return pTex;
}

CTexture* CResMgr::FindTexture(const wstring& _strKey)
{


    //map 의 find 리턴형은 iterator로 나옴
    map<wstring,CRes*>::iterator iter = m_mapTex.find(_strKey);

    if (iter == m_mapTex.end())
        return nullptr;

    return static_cast<CTexture*>(iter->second);
}

CSound* CResMgr::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
    CSound* pSound = FindSound(_strKey);
    if (nullptr != pSound)
        return pSound;

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    pSound = new CSound;
    pSound->Load(strFilePath);
    pSound->SetKey(_strKey);
    pSound->SetRelativePath(_strRelativePath);

    m_mapSound.insert(make_pair(_strKey, pSound));

    return pSound;
}

CSound* CResMgr::FindSound(const wstring& _strKey)
{
    map<wstring, CRes*>::iterator iter = m_mapSound.find(_strKey);

    if (iter == m_mapSound.end())
        return nullptr;

    return static_cast<CSound*>(iter->second);
}
