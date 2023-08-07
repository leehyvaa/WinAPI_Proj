#include "pch.h"
#include "func.h"

#include "CEventMgr.h"

void CreateObject(GameObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;


	CEventMgr::GetInst()->AddEvent(evn);
}

void DeleteObject(GameObject* _pObj)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;


	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;


	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)_pAI;
	evn.wParam = (DWORD_PTR)_eNextState;


	CEventMgr::GetInst()->AddEvent(evn);
}

void SaveWString(const wstring& _str, FILE* _pFile)
{

	//세이브할때 스트링채로 가져다 넣으면 안되고 wchar_t로 바꿔서 넣는다.
	//이유는 string이 애초에 문자열을 멤버로 저장하는게 아닌 힙메모리에 문자열을 넣어넣고
	//포인터로 가져다 쓰는 녀석이기 때문
	//이런걸 데이터 직렬화라 한다.
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	//문자 길이 저장
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	//문자열 저장
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);


}

void LoadWString(wstring& _str, FILE* _pFile)
{

	//문자열 읽기
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);
	wchar_t szBuff[256] = {};
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	_str = szBuff;
}
