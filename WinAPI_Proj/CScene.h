#pragma once
class GameObject;

class CScene
{
private:
	//������Ʈ�� ���� �� ������ ���͸� �׷� ������ŭ ����
	vector<GameObject*> m_arrObj[(UINT)GROUP_TYPE::END];
	wstring			 m_strName;//�� �̸�


public:
	void SetName(const wstring& _strName) { m_strName = _strName;}
	const wstring& GetName() { return m_strName; }


	void Update();
	void Render(HDC _dc);

	virtual void Enter()=0;
	virtual void Exit() = 0;

protected:
	void AddObject(GameObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}
public:
	CScene();
	virtual ~CScene();

};
