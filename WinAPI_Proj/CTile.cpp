#include "pch.h"
#include "CTile.h"


CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::Render(HDC _dc)
{
	Vec2 vRenderPos =  CCamera::GetInst()->GetRenderPos(GetPos());
	
	//Rectangle(_dc,vRenderPos.x,vRenderPos.y,vRenderPos.x)
}
