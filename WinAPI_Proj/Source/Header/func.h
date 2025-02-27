﻿#pragma once
class GameObject;
class AI;

void CreateObject(GameObject* _pObj, GROUP_TYPE _eGroup);
void DeleteObject(GameObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);

template<typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); i++)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template<typename T1,typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	//템플릿 안에서 iterator 같은 이너클래스 쓰려면 typename을 적어줘야함
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); iter++)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	_map.clear();
}

void FScanf(char* _pOutBuff, FILE* _pFIle);


void SaveWString(const wstring& _str, FILE* _pFile);
void LoadWString(wstring& _str, FILE* _pFile);