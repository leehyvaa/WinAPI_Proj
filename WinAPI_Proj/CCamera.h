#pragma once

class GameObject;

class CCamera
{
	SINGLE(CCamera);

private:
	Vec2 m_vLookAt;
	GameObject* m_pTargetObj; //카메라 타겟 오브젝트

	Vec2 m_vDiff; //해상도 중심위치와, 카메라 LookAt 간의 차이 값

public:
	void SetLookAt(Vec2 _vLook) { m_vLookAt = _vLook; }
	void SetTarget(GameObject* _pTarget) { m_pTargetObj = _pTarget; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }

public:
	void Update();

private:
	void CalDiff();

};

