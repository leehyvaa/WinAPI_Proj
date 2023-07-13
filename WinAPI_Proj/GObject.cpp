#include "pch.h"
#include "GObject.h"
#include "CKeyMgr.h"

GObject::GObject()
	: m_vPos{}
	, m_vScale{}
{
}


GObject::~GObject()
{
}

void GObject::Update()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::HOLD);
}

void GObject::Render(HDC _dc)
{
}



bool GObject::Collision(GObject& _vObj)
{
	return false;
}
