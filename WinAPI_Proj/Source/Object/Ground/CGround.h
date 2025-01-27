#pragma once
#include "GameObject.h"


class CGround : public GameObject
{
private:

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);


	virtual void Start();
	virtual void Update();
	virtual void Render(HDC _dc);
	

	virtual void Save(FILE* _file);
	virtual void Load(FILE* _file);

public:
	CGround();
	virtual ~CGround();
	
	CLONE(CGround)


	friend class CGroundPrefab;

};

