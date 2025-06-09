#pragma once

// Direct2D 전방 선언
struct ID2D1RenderTarget;

class CMonster;
class CState;

class AI
{
private:
	map<MON_STATE, CState*> m_mapState;
	CState* m_pCurState;
	CMonster* m_pOwner;

public:
	void Update();
    void Render(HDC _dc);
    void RenderD2D(ID2D1RenderTarget* _pRenderTarget);

public:
	void AddState(CState* _pState);
	CState* GetState(MON_STATE _eState);
	MON_STATE GetCurState();
	void SetCurState(MON_STATE _eState);
	void ChangeState(MON_STATE _eNextState);

	CMonster* GetOwner() { return m_pOwner; }

public:
	AI();
	~AI();

	friend class CMonster;
};

