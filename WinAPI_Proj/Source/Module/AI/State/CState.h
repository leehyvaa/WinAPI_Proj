#pragma once

// Direct2D 전방 선언
struct ID2D1RenderTarget;

class AI;
class CMonster;

class CState
{
private:
	AI* m_pAI;
	MON_STATE m_eState;


public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType(){ return m_eState; }
	CMonster* GetMonster();

	virtual void Update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;
    virtual void Render(HDC _dc);
    virtual void RenderD2D(ID2D1RenderTarget* _pRenderTarget);

public:
	CState(MON_STATE _eState);
	virtual ~CState();

	friend class AI;
};

