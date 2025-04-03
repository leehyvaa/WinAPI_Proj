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
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

CAnimation::CAnimation()
	:m_pAnimator(nullptr)
	,m_pTex(nullptr)
	,m_iCurFrm(0)
	,m_fAccTime(0.f)
	,m_bFinish(false)
	,m_fSizeMulti(1)
	,rotPos(nullptr)
    ,m_bCached(false)
{
	rotPos = new POINT[4];
}

CAnimation::~CAnimation()
{
	delete[] rotPos;
    // 캐싱된 비트맵들 해제
    for (auto& bitmap : m_vecFrameBitmaps)
    {
        delete bitmap;
    }
    m_vecFrameBitmaps.clear();
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
    if (m_bFinish || m_iCurFrm < 0 || m_iCurFrm >= m_vecFrameBitmaps.size()) // m_vecFrm.size() ->m_vecFrameBitmaps.size()
        return;

    // 캐싱되지 않은 경우 캐싱 수행 (이전 코드 유지)
    if (!m_bCached)
    {
        CacheFrames();
    }

    GameObject* pObj = m_pAnimator->GetObj();
    Vec2 vPos = pObj->GetWorldPos(); // FinalUpdate에서 계산된 최종 논리적 위치

    // --- 확인 및 제거 ---
    // 만약 아래와 같이 vOffset을 직접 더하는 코드가 있다면 제거합니다.
    //vPos += m_vecFrm[m_iCurFrm].vOffset; // <--- 이 줄 제거 또는 주석 처리
    // --------------------

    vPos = CCamera::GetInst()->GetRenderPos(vPos); // 카메라 변환 적용

    float rotationAngle = m_pAnimator->GetObj()->GetWorldRotation();
    float minRotationThreshold = 3.0f; // 회전 적용 임계값 (이전 코드 유지)

    // GDI+ Graphics 객체 생성 (이전 코드 유지)
    Graphics graphics(_dc);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);

    // 현재 프레임의 캐싱된 비트맵 가져오기 (이전 코드 유지)
    Bitmap* frameBitmap = m_vecFrameBitmaps[m_iCurFrm];

    // 프레임 정보 (이전 코드 유지)
    int srcWidth = frameBitmap->GetWidth();
    int srcHeight = frameBitmap->GetHeight();

    // 렌더링 위치 계산 (논리적 vPos 기준)
    // destX/destY는 논리적 위치 vPos를 기준으로 스프라이트의 좌상단 좌표를 계산합니다.
    // GDI+ 회전 중심(centerX/centerY)도 이 destX/destY 기반으로 계산됩니다.
    int destX = static_cast<INT>(vPos.x - srcWidth * m_fSizeMulti / 2.0f + 0.5f);
    int destY = static_cast<INT>(vPos.y - srcHeight * m_fSizeMulti / 2.0f + 0.5f);
    int destWidth = static_cast<INT>(srcWidth * m_fSizeMulti + 0.5f);
    int destHeight = static_cast<INT>(srcHeight * m_fSizeMulti + 0.5f);

    // 회전 및 그리기 로직 (이전 코드 유지)
    if (abs(rotationAngle) > minRotationThreshold)
    {
        float centerX = static_cast<float>(destX + destWidth / 2);
        float centerY = static_cast<float>(destY + destHeight / 2);
        Matrix rotMatrix;
        rotMatrix.RotateAt(rotationAngle, PointF(centerX, centerY));
        graphics.SetTransform(&rotMatrix);
        graphics.DrawImage(frameBitmap, Rect(destX, destY, destWidth, destHeight));
        graphics.ResetTransform();
    }
    else
    {
        graphics.DrawImage(frameBitmap, Rect(destX, destY, destWidth, destHeight));
    }
}

// 프레임 캐싱 함수
void CAnimation::CacheFrames()
{
    if (m_bCached || m_vecFrm.empty() || !m_pTex)
        return;
        
    // 기존에 캐싱된 비트맵 해제
    for (auto& bitmap : m_vecFrameBitmaps)
    {
        delete bitmap;
    }
    m_vecFrameBitmaps.clear();
    
    // 원본 비트맵 생성
    HBITMAP hBitmap = m_pTex->GetHBITMAP();
    Bitmap sourceBitmap(hBitmap, nullptr);
    
    // 각 프레임별로 처리된 비트맵 생성 및 저장
    for (size_t i = 0; i < m_vecFrm.size(); i++)
    {
        int srcX = static_cast<INT>(m_vecFrm[i].vLT.x);
        int srcY = static_cast<INT>(m_vecFrm[i].vLT.y);
        int srcWidth = static_cast<INT>(m_vecFrm[i].vSlice.x);
        int srcHeight = static_cast<INT>(m_vecFrm[i].vSlice.y);
        
        // 프레임 크기의 새 비트맵 생성
        Bitmap* frameBitmap = new Bitmap(srcWidth, srcHeight, PixelFormat32bppARGB);
        
        // 원본에서 프레임 부분만 복사
        Graphics frameGraphics(frameBitmap);
        
        // 투명 처리를 위한 이미지 속성 설정
        ImageAttributes imgAttr;
        imgAttr.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);
        
        // 원본 이미지의 해당 부분을 새 비트맵에 복사
        frameGraphics.DrawImage(
            &sourceBitmap,
            Rect(0, 0, srcWidth, srcHeight), // 대상 사각형
            srcX, srcY, srcWidth, srcHeight, // 소스 부분
            UnitPixel,
            &imgAttr
        );
        
        m_vecFrameBitmaps.push_back(frameBitmap);
    }
    
    m_bCached = true;
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
		frm.vLT = _vLT + _vStep * static_cast<float>(i);
		frm.vOffset = _vOffset;

		m_vecFrm.push_back(frm);
	}



	m_fSizeMulti = _fSizeMulti;
    // 프레임 캐싱 실행
    CacheFrames();
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
	fprintf(pFile, "%d\n", static_cast<int>(m_vecFrm.size()));


	fprintf(pFile, "[Animation_Size_Multiply]\n");
	fprintf(pFile, "%f\n", m_fSizeMulti);


	//모든 프레임 정보
	//fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
	for (size_t i = 0; i < m_vecFrm.size(); i++)
	{
		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n", static_cast<int>(i));

		fprintf(pFile, "[Left Top]\n");
		fprintf(pFile, "%d %d\n", static_cast<int>(m_vecFrm[i].vLT.x), static_cast<int>(m_vecFrm[i].vLT.y));

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d %d\n", static_cast<int>(m_vecFrm[i].vSlice.x), static_cast<int>(m_vecFrm[i].vSlice.y));

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d %d\n", static_cast<int>(m_vecFrm[i].vOffset.x), static_cast<int>(m_vecFrm[i].vOffset.y));

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
    // 프레임 로드 완료 후 캐싱 실행
    CacheFrames();
}


