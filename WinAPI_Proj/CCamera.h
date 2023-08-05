#pragma once

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
	CAM_EFFECT eEffect;//ī�޶� ȿ��
	float fDuration;//ȿ�� ���� �ð�
	float fCurTime;//ȿ���� ����� �ð�
};


class CCamera
{
	SINGLE(CCamera);

private:
	Vec2 m_vLookAt;
	Vec2 m_vCurLookAt; //������ġ�� ������ġ ���� ��ġ
	Vec2 m_vPrevLookAt; //ī�޶� ���� ���������� ��ġ

	GameObject* m_pTargetObj; //ī�޶� Ÿ�� ������Ʈ
	Vec2 m_vDiff; //�ػ� �߽���ġ��, ī�޶� LookAt ���� ���� ��

	float m_fTime; //Ÿ���� ���󰡴µ� �ɸ��� �ð�
	float m_fSpeed; //Ÿ���� ���󰡴� �ӵ�
	float m_fAccTime; //���� �ð�

	CTexture* m_pVeilTex; //ī�޶� ������ �ؽ���
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
private:
	void CalDiff();

};

