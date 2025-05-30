#pragma once
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

