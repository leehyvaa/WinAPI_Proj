#pragma once
#include "CMonPrefab.h"
#include "CGroundPrefab.h"
#include <d2d1.h>

class GameObject;
class CBackGround;
class CTexture;
class CTextUI;
class CScene
{
private:
	// 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	vector<GameObject*> m_arrObj[static_cast<UINT>(GROUP_TYPE::END)];
	wstring			 m_strName; //씬 이름
    CTextUI* m_pPlayerText;
    CTextUI* m_pPoolDebugText; // 오브젝트 풀 디버깅용 텍스트 UI


	UINT m_iTileX; // 타일 가로 개수
	UINT m_iTileY;
	UINT m_iGroundCount;

	bool bDrawGrid;
	bool bDrawCollider;
	bool bDrawGroundType;
    bool bDrawCompleteGround;
	bool bDrawOutWindow;
    
	GameObject* m_pPlayer;
	CBackGround* backGround;

	// 스폰, 클리어 데이터
	Vec2 m_vPlayerSpawnPos;
	Vec2 m_vSceneClearStartPos;
	Vec2 m_vSceneClearEndPos;
	bool m_bPlayerSpawnSet;
	bool m_bSceneClearSet;

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

	// 스폰 데이터 접근자
	Vec2 GetPlayerSpawnPos() const { return m_vPlayerSpawnPos; }
	Vec2 GetSceneClearStartPos() const { return m_vSceneClearStartPos; }
	Vec2 GetSceneClearEndPos() const { return m_vSceneClearEndPos; }


	virtual void Start();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render(ID2D1RenderTarget* _pRenderTarget);
	void RenderTile(ID2D1RenderTarget* _pRenderTarget);

	virtual void Enter();
	virtual void Exit();

public:
	void AddObject(GameObject* _pObj, GROUP_TYPE _eType);
	void RegisterPlayer(GameObject* _pPlayer) { m_pPlayer = _pPlayer; }


	// 벡터 레퍼런스를 반환하면서 const로 원본수정 불가능하게
	const vector<GameObject*>& GetGroupObject(GROUP_TYPE _eType){return m_arrObj[static_cast<UINT>(_eType)];}
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);
	void CreateGround();
	
	vector<GameObject*>& GetUIGroup() { return m_arrObj[static_cast<UINT>(GROUP_TYPE::UI)]; }
    void UpdatePoolDebugInfo(); // 오브젝트 풀 정보 디버그
    void TogglePoolDebugDisplay(); 
    void UpdateDebugUI();
public:
	CScene();
	virtual ~CScene();

};

