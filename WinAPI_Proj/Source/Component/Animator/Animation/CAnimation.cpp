#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTexture.h"
#include "GameObject.h"
#include "CTimeMgr.h"
#include "CCamera.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CCore.h"
#include "SelectGDI.h"

CAnimation::CAnimation()
	:m_pAnimator(nullptr)
	,m_pTex(nullptr)
	,m_iCurFrm(0)
	,m_fAccTime(0.f)
	,m_bFinish(false)
	,m_fSizeMulti(1)
	,rotPos(nullptr)
{
	rotPos = new POINT[4];
}

CAnimation::~CAnimation()
{
	delete[] rotPos;
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

	
	vPos += m_vecFrm[m_iCurFrm].vOffset; //object 위치에 offset 만큼 추가 이동위치
	

	//렌더링 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);



	if (m_pAnimator->GetRotation() != 0.f)
	{
		HDC tempDC = m_pAnimator->GetTempTex()->GetDC();

		float rot = m_pAnimator->GetRotation();

		
	

		double degree[3];
		degree[0] = rot + 315.f - 90.f;
		degree[1] = rot + 45.f - 90.f;
		degree[2] = rot + 225.f - 90.f;

		

		double radian1 = degree[0] * (3.14159 / 180.f);
		double radian2 = degree[1] * (3.14159 / 180.f);
		double radian3 = degree[2] * (3.14159 / 180.f);



		rotPos[0] = {(int)(150 + 141 * cos(radian1)),(int)(150 + 141 * sin(radian1)) };
		rotPos[1] = { (int)(150 + 141 * cos(radian2)),(int)(150 + 141 * sin(radian2)) };
		rotPos[2] = { (int)(150 + 141 * cos(radian3)),(int)(150 + 141 * sin(radian3)) };


		POINT a = rotPos[0];
		POINT d = rotPos[1];
		POINT e = rotPos[2];




		SelectGDI b(tempDC, BRUSH_TYPE::MAGENTA);
		PatBlt(tempDC, 0, 0, 300, 300, PATCOPY);


		PlgBlt(tempDC, rotPos, m_pTex->GetDC()
			, (int)(m_vecFrm[m_iCurFrm].vLT.x)
			, (int)(m_vecFrm[m_iCurFrm].vLT.y)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
			, NULL, NULL, NULL);



		TransparentBlt(_dc
			, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x * m_fSizeMulti / 2.f)
			, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y * m_fSizeMulti / 2.f)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.x * m_fSizeMulti)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.y * m_fSizeMulti)
			, tempDC
			, 0
			, 0
			, 300
			, 300
			, (int)(RGB(255, 0, 255))
		);
	}
	else
	{
		TransparentBlt(_dc
			, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x * m_fSizeMulti / 2.f)
			, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y * m_fSizeMulti / 2.f)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.x * m_fSizeMulti)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.y * m_fSizeMulti)
			, m_pTex->GetDC()
			, (int)(m_vecFrm[m_iCurFrm].vLT.x)
			, (int)(m_vecFrm[m_iCurFrm].vLT.y)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
			, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
			, (int)(RGB(255, 0, 255))
		);
	}


	

	
	
	
	//PlgBlt()

	
	//PlgBlt(_dc,)

}


void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize,
					Vec2 _vStep, float _fDuration, UINT _iFrameCount, float _fSizeMulti, Vec2 _vOffset)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};

	for (UINT i = 0; i < _iFrameCount; i++)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;
		frm.vOffset = _vOffset;

		m_vecFrm.push_back(frm);
	}



	m_fSizeMulti = _fSizeMulti;
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	//Animation의 이름을 저장한다. (데이터 직렬화)
	//SaveWString(m_strName, pFile);
	fprintf(pFile, "[Animation_Name]\n");
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	//Animation이 사용하는 텍스쳐
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


	//프레임 개수
	//size_t iFrameCount = m_vecFrm.size();
	//fwrite(&iFrameCount, sizeof(size_t), 1, pFile);
	fprintf(pFile, "[Frame_Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());


	fprintf(pFile, "[Animation_Size_Multiply]\n");
	fprintf(pFile, "%f\n", m_fSizeMulti);


	//모든 프레임 정보
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



	////애니메이션 이름 읽기(바이너리)
	//LoadWString(m_strName, pFile);

	////텍스쳐
	//wstring strTexKey, strTexPath;
	//LoadWString(strTexKey, pFile);
	//LoadWString(strTexPath, pFile);
	//m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey,strTexPath);



	////프레임 개수
	//size_t iFrameCount = 0;
	//fread(&iFrameCount, sizeof(size_t), 1, pFile);

	////모든 프레임 정보
	//m_vecFrm.resize(iFrameCount);
	//fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);





	//Animation의 이름을 읽어온다.
	string str;
	char szBuff[256] = {};

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile); //한줄씩 읽어오는 함수
	
	str = szBuff;
	m_strName = wstring(str.begin(), str.end());


	//참조하는 텍스처 이름 및 경로
	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);


	



	//프레임 개수
	FScanf(szBuff, pFile);
	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount); //문자를 정수로 바꿔서 읽음
	FScanf(szBuff, pFile);


	//사이즈 배율
	FScanf(szBuff, pFile);
	fscanf_s(pFile, "%f", &m_fSizeMulti);
	FScanf(szBuff, pFile);



	//모든 프레임 정보
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
				fscanf_s(pFile, "%d", &pt.y); //정수만날때까지 ,나 공백문자를 다 읽으면서 넘김
			
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


