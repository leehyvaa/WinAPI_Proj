#pragma once

class GameObject;

class CCamera
{
	SINGLE(CCamera);

private:
	Vec2 m_vLookAt;
	GameObject* m_pTargetObj; //ī�޶� Ÿ�� ������Ʈ

	Vec2 m_vDiff; //�ػ� �߽���ġ��, ī�޶� LookAt ���� ���� ��

public:
	void SetLookAt(Vec2 _vLook) { m_vLookAt = _vLook; }
	void SetTarget(GameObject* _pTarget) { m_pTargetObj = _pTarget; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }

public:
	void Update();

private:
	void CalDiff();

};

