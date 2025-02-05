#pragma once
#include "CMonPrefab.h"
#include "CGroundPrefab.h"

class GameObject;
class CBackGround;
class CTexture;

class CScene
{
private:
	//오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	vector<GameObject*> m_arrObj[(UINT)GROUP_TYPE::END];
	wstring			 m_strName;//씬 이름

	


	UINT m_iTileX; //타일 가로 개수
	UINT m_iTileY;
	UINT m_iGroundCount;

	bool bDrawGrid;
	bool bDrawCollider;
	bool bDrawGroundType;
	bool bDrawOutWindow;

	GameObject* m_pPlayer;
	CBackGround* backGround;

public:
	void SetName(const wstring& _strName) { m_strName = _strName;}
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }
	UINT GetGroundCount() { return m_iGroundCount; }
	void SetGroundCount(UINT _count) { m_iGroundCount = _count; }

	GameObject* GetPlayer() { return m_pPlayer; }
	bool GetDrawGroundType() { return bDrawGroundType; }
	bool GetDrawCollider() { return bDrawCollider; }

	CBackGround* GetBackGround() { return backGround; }
	void SetBackGround(CBackGround* _tex) { backGround = _tex; }

	virtual void Start();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
	void Render_Tile(HDC _dc);

	virtual void Enter()=0;
	virtual void Exit() = 0;

public:
	void AddObject(GameObject* _pObj, GROUP_TYPE _eType){m_arrObj[(UINT)_eType].push_back(_pObj);}
	void RegisterPlayer(GameObject* _pPlayer) { m_pPlayer = _pPlayer; }


	//벡터 레퍼런스를 반환하면서 const로 원본수정 불가능하게
	const vector<GameObject*>& GetGroupObject(GROUP_TYPE _eType){return m_arrObj[(UINT)_eType];}
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);
	void CreateGround();
	
	vector<GameObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

public:
	CScene();
	virtual ~CScene();

};

