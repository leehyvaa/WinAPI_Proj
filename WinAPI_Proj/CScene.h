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
	void FinalUpdate();
	void Render(HDC _dc);

	virtual void Enter()=0;
	virtual void Exit() = 0;

public:
	void AddObject(GameObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	//���� ���۷����� ��ȯ�ϸ鼭 const�� �������� �Ұ����ϰ�
	const vector<GameObject*>& GetGroupObject(GROUP_TYPE _eType){return m_arrObj[(UINT)_eType];}
public:
	CScene();
	virtual ~CScene();

};

