#include "CBackGround.h"
#include "CCore.h"
#include "CTexture.h"

CBackGround::CBackGround()
	:CTile::CTile()
{
	SetScale(CCore::GetInst()->GetResolution());
}

CBackGround::~CBackGround()
{
}

void CBackGround::Render(HDC _dc)
{
	return;
	CTexture* pTileTex = GetTexture();

	if (nullptr == pTileTex)
		return;


	Vec2 vRenderPos =GetPos();

	Vec2 vScale = GetScale();


	UINT iWidth = pTileTex->Width();
	UINT iHeight = pTileTex->Height();



	//BitBlt(_dc
	//	, (int)(vRenderPos.x)
	//	, (int)(vRenderPos.y)
	//	, (int)vScale.x
	//	, (int)vScale.y
	//	, m_pTileTex->GetDC()
	//	, iCurCol*TILE_SIZE
	//	, iCurRow * TILE_SIZE
	//	,SRCCOPY);


	


	

	TransparentBlt(_dc
		, int(vRenderPos.x)
		, int(vRenderPos.y)
		, (int)vScale.x, (int)vScale.y
		, pTileTex->GetDC()
		, 0, 0,
		iWidth, iHeight, RGB(255, 0, 255));




}
