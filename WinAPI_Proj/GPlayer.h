#pragma once
#include "GObject.h"
class GPlayer : public GObject
{

private:



public:
	GPlayer();
	GPlayer(Vec2 _vPos, Vec2 _vScale);
	virtual ~GPlayer();

	Vec2 GetPos() { return m_vPos; }


	virtual void Update() override;
	virtual void Draw()  override;
	virtual bool Collision(GObject& vObj) override;

	void Damaged();
	
};

