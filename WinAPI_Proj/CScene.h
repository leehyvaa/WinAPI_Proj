#pragma once
class GameObject;

class CScene
{
private:
	//오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	vector<GameObject*> m_arrObj[(UINT)GROUP_TYPE::END];
	wstring			 m_strName;//씬 이름


public:
	void SetName(const wstring& _strName) { m_strName = _strName;}
	const wstring& GetName() { return m_strName; }


	void Update();
	void FinalUpdate();
	void Render(HDC _dc);

	virtual void Enter()=0;
	virtual void Exit() = 0;

public:
	void AddObject(GameObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	//벡터 레퍼런스를 반환하면서 const로 원본수정 불가능하게
	const vector<GameObject*>& GetGroupObject(GROUP_TYPE _eType){return m_arrObj[(UINT)_eType];}
public:
	CScene();
	virtual ~CScene();

};

