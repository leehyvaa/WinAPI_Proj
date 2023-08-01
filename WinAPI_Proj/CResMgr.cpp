#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"

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
CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
    CTexture* pTex = FIndTexture(_strKey);
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

CTexture* CResMgr::FIndTexture(const wstring& _strKey)
{


    //map 의 find 리턴형은 iterator로 나옴
    map<wstring,CRes*>::iterator iter = m_mapTex.find(_strKey);

    if (iter == m_mapTex.end())
        return nullptr;

    return (CTexture*)iter->second;
}
