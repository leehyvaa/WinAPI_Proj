#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTexture.h"
#include "GameObject.h"
#include "CTimeMgr.h"
#include "CCamera.h"
#include "CPathMgr.h"
#include "CResMgr.h"

CAnimation::CAnimation()
	:m_pAnimator(nullptr)
	,m_pTex(nullptr)
	,m_iCurFrm(0)
	,m_fAccTime(0.f)
	,m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}
void CAnimation::Update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
		
	}
}

void CAnimation::Render(HDC _dc)
{
	if (m_bFinish)
		return;

	GameObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	
	vPos += m_vecFrm[m_iCurFrm].vOffset; //object ��ġ�� offset ��ŭ �߰� �̵���ġ
	

	//������ ��ǥ�� ��ȯ
	vPos = CCamera::GetInst()->GetRenderPos(vPos);


	TransparentBlt(_dc
		, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, m_pTex->GetDC()
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, (int)(RGB(255, 0, 255))
	);
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize,
					Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};

	for (UINT i = 0; i < _iFrameCount; i++)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	//Animation�� �̸��� �����Ѵ�. (������ ����ȭ)
	//SaveWString(m_strName, pFile);
	fprintf(pFile, "[Animation_Name]\n");
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	//Animation�� ����ϴ� �ؽ���
	//SaveWString(m_pTex->GetKey(),pFile);
	//SaveWString(m_pTex->GetRelativePath(), pFile);
	fprintf(pFile, "[Texture_Name]\n");
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture_Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	//������ ����
	//size_t iFrameCount = m_vecFrm.size();
	//fwrite(&iFrameCount, sizeof(size_t), 1, pFile);
	fprintf(pFile, "[Frame_Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());




	//��� ������ ����
	//fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
	for (size_t i = 0; i < m_vecFrm.size(); i++)
	{
		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n", (int)i);

		fprintf(pFile, "[Left Top]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", m_vecFrm[i].fDuration);

		fprintf(pFile, "\n\n");
	}


	



	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);



	////�ִϸ��̼� �̸� �б�(���̳ʸ�)
	//LoadWString(m_strName, pFile);

	////�ؽ���
	//wstring strTexKey, strTexPath;
	//LoadWString(strTexKey, pFile);
	//LoadWString(strTexPath, pFile);
	//m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey,strTexPath);



	////������ ����
	//size_t iFrameCount = 0;
	//fread(&iFrameCount, sizeof(size_t), 1, pFile);

	////��� ������ ����
	//m_vecFrm.resize(iFrameCount);
	//fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);





	//Animation�� �̸��� �о�´�.
	string str;
	char szBuff[256] = {};

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile); //���پ� �о���� �Լ�
	
	str = szBuff;
	m_strName = wstring(str.begin(), str.end());


	//�����ϴ� �ؽ�ó �̸� �� ���
	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);


	



	//������ ����
	FScanf(szBuff, pFile);
	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount); //���ڸ� ������ �ٲ㼭 ����



	//��� ������ ����
	tAnimFrm frm = {};

	for (int i = 0; i < iFrameCount; i++)
	{
		POINT pt = {};

		while(true)
		{
			FScanf(szBuff, pFile);



			if (!strcmp("[Frame Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left Top]",szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y); //�������������� ,�� ���鹮�ڸ� �� �����鼭 �ѱ�
			
				frm.vLT = pt;
			}
			else if (!strcmp("[Slice Size]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vSlice = pt;
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vOffset = pt;
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &frm.fDuration);
				break;
			}
		}

		m_vecFrm.push_back(frm);
	}



	fclose(pFile);
}


