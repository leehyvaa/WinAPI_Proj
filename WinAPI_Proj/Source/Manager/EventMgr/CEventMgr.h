#pragma once

struct tEvent
{
	EVENT_TYPE eEvent;
	DWORD_PTR lParam;
	DWORD_PTR wParam;

};

class CEventMgr
{
	SINGLE(CEventMgr)
private:
	vector<tEvent> m_vecEvent;
	vector<GameObject*> m_vecDead;

public:
	void Update();
	void Excute(const tEvent& _eve);

	void AddEvent(const tEvent& _eve){m_vecEvent.push_back(_eve);}
};

