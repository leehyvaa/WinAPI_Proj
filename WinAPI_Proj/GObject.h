#pragma once
#include "pch.h"

class GObject
{
public:
	Vec2 m_vPos;
	Vec2 m_vScale;

public:
	GObject();
	virtual ~GObject();
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	

	//virtual void Update();
	//virtual void Draw();
	//virtual bool Collision(GObject& vObj);

};

