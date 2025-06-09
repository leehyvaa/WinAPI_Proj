#pragma once

struct ID2D1RenderTarget;
class GameObject;
class CTexture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT eEffect;//카메라 효과
	float fDuration;//효과 진행 시간
	float fCurTime;//효과가 진행된 시간
};


class CCamera
{
	SINGLE(CCamera);

private:
	Vec2 m_vLookAt;
	Vec2 m_vCurLookAt; //이전위치와 현재위치 보정 위치
	Vec2 m_vPrevLookAt; //카메라가 보는 이전프레임 위치

	GameObject* m_pTargetObj; //카메라 타겟 오브젝트
	Vec2 m_vDiff; //해상도 중심위치와, 카메라 LookAt 간의 차이 값

	float m_fTime; //타겟을 따라가는데 걸리는 시간
	float m_fSpeed; //타겟을 따라가는 속도
	float m_fAccTime; //누적 시간

	CTexture* m_pVeilTex; //카메라 가림막 텍스쳐
	list<tCamEffect> m_listCamEffect;

public:
	void SetLookAt(Vec2 _vLook) 
	{ 
		m_vLookAt = _vLook;
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;
	}
	void SetTarget(GameObject* _pTarget) { m_pTargetObj = _pTarget; }
	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; }

	void FadeIn(float _fDuration);
	void FadeOut(float _fDuration);


public:
	void init();
	void Update();
	void Render(HDC _dc);
	void RenderD2D(ID2D1RenderTarget* _pRenderTarget);
private:
	void CalDiff();

};

