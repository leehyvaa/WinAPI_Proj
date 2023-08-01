#pragma once


class CRes;
class CTexture;

class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes*> m_mapTex;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* FIndTexture(const wstring& _strKey);
};

