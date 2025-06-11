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
    ,m_EndFrameEvent(nullptr)
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
		    // 애니메이션이 끝났을 때 콜백 함수 호출
		    if (m_EndFrameEvent)
		    {
		        m_EndFrameEvent();
		        m_EndFrameEvent = nullptr; // 콜백은 한 번만 호출되도록 초기화
		    }
			return;
		}

		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
		
	}
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
    // CacheFrames();  // 프레임 캐싱
}



void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// Animation의 이름을 저장한다. (데이터 직렬화)
	// SaveWString(m_strName, pFile);
	fprintf(pFile, "[Animation_Name]\n");
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	// Animation이 사용하는 텍스쳐
	// SaveWString(m_pTex->GetKey(),pFile);
	// SaveWString(m_pTex->GetRelativePath(), pFile);
	fprintf(pFile, "[Texture_Name]\n");
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture_Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	// 프레임 개수
	// size_t iFrameCount = m_vecFrm.size();
	// fwrite(&iFrameCount, sizeof(size_t), 1, pFile);
	fprintf(pFile, "[Frame_Count]\n");
	fprintf(pFile, "%d\n", static_cast<int>(m_vecFrm.size()));


	fprintf(pFile, "[Animation_Size_Multiply]\n");
	fprintf(pFile, "%f\n", m_fSizeMulti);


	// 모든 프레임 정보
	// fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
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


	// Animation의 이름을 읽어온다.
	string str;
	char szBuff[256] = {};

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile); // 한줄씩 읽어오는 함수
	
	str = szBuff;
	m_strName = wstring(str.begin(), str.end());


	// 참조하는 텍스처 이름 및 경로
	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);
    

	// 프레임 개수
	FScanf(szBuff, pFile);
	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount); // 문자를 정수로 바꿔서 읽음
	FScanf(szBuff, pFile);

	// 사이즈 배율
	FScanf(szBuff, pFile);
	fscanf_s(pFile, "%f", &m_fSizeMulti);
	FScanf(szBuff, pFile);

    
	// 모든 프레임 정보
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
				fscanf_s(pFile, "%d", &pt.y); // 정수만날때까지 ,나 공백문자를 다 읽으면서 넘김
			
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
    //CacheFrames();
}





void CAnimation::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
    CTimeMgr::StartTimer(L"AnimationComp_DXRender");

    if (m_bFinish || m_iCurFrm < 0 || m_iCurFrm >= static_cast<int>(m_vecFrm.size()) || !_pRenderTarget || !m_pTex)
    {
        CTimeMgr::EndTimer(L"AnimationComp_DXRender");
        return;
    }

    // 현재 프레임 정보 가져오기
    tAnimFrm& curFrame = m_vecFrm[m_iCurFrm];
    
    // 소스 사각형 계산
    D2D1_RECT_F srcRect = D2D1::RectF(
        curFrame.vLT.x,
        curFrame.vLT.y,
        curFrame.vLT.x + curFrame.vSlice.x,
        curFrame.vLT.y + curFrame.vSlice.y
    );
    
    // 목적지 크기 계산 (크기 배율 적용)
    D2D1_SIZE_F dstSize = D2D1::SizeF(
        curFrame.vSlice.x * m_fSizeMulti,
        curFrame.vSlice.y * m_fSizeMulti
    );
    
    // 캐시 키 생성 (애니메이션이름_프레임인덱스)
    wstring strCacheKey = m_strName + L"_" + to_wstring(m_iCurFrm);
    
    // CTexture의 GetSlicedBitmap을 통해 비트맵 가져오기
    ID2D1Bitmap* pFrameBitmap = m_pTex->GetSlicedBitmap(strCacheKey, srcRect, dstSize);
    if (!pFrameBitmap)
    {
        CTimeMgr::EndTimer(L"AnimationComp_DXRender");
        return;
    }

    // 게임 오브젝트 정보 가져오기
    GameObject* pObj = m_pAnimator->GetObj();
    Vec2 vLogicalPos = pObj->GetWorldPos();
    bool isFacingRight = pObj->GetIsFacingRight();
    
    Vec2 vOffset = curFrame.vOffset;

    // 왼쪽을 볼 경우 오프셋 X값 반전
    if (!isFacingRight)
        vOffset.x *= -1.f;

    float worldRotationAngle = pObj->GetWorldRotation();
    float rotationRad = worldRotationAngle * (3.14159f / 180.f);
    float cosAngle = cosf(rotationRad);
    float sinAngle = sinf(rotationRad);

    Vec2 vRotatedOffset;
    vRotatedOffset.x = vOffset.x * cosAngle - vOffset.y * sinAngle;
    vRotatedOffset.y = vOffset.x * sinAngle + vOffset.y * cosAngle;

    Vec2 vVisualPos = vLogicalPos + vRotatedOffset;
    Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vVisualPos);

    D2D1_SIZE_F bitmapSize = pFrameBitmap->GetSize();

    // originalTransform에 원본 변환 행렬 저장해 놓기
    D2D1_MATRIX_3X2_F originalTransform;
    _pRenderTarget->GetTransform(&originalTransform);

    // SRT 변환 행렬 생성 및 곱하기
    D2D1_MATRIX_3X2_F matScale = isFacingRight ? D2D1::Matrix3x2F::Identity() : D2D1::Matrix3x2F::Scale(-1.f, 1.f);
    D2D1_MATRIX_3X2_F matRotation = D2D1::Matrix3x2F::Rotation(worldRotationAngle);
    D2D1_MATRIX_3X2_F matTranslation = D2D1::Matrix3x2F::Translation(vRenderPos.x, vRenderPos.y);
    
    _pRenderTarget->SetTransform(matScale * matRotation * matTranslation);

    // 0,0 중심점으로 이미지 렌더링
    D2D1_RECT_F destRect = D2D1::RectF(
        -bitmapSize.width / 2.0f,
        -bitmapSize.height / 2.0f,
        bitmapSize.width / 2.0f,
        bitmapSize.height / 2.0f
    );

    _pRenderTarget->DrawBitmap(pFrameBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

    // originalTransform(원본 변환 행렬)로 복원
    _pRenderTarget->SetTransform(originalTransform);
    CTimeMgr::EndTimer(L"AnimationComp_DXRender");
}





// void CAnimation::Render(HDC _dc)
// {
//     CTimeMgr::StartTimer(L"AnimationComp_Render");
//     if (m_bFinish || m_iCurFrm < 0 || m_iCurFrm >= m_vecFrameBitmaps.size())
//         return;
//
//     // 이미지 캐싱
//     if (!m_bCached)
//         CacheFrames();
//     
//     
//     GameObject* pObj = m_pAnimator->GetObj();
//     Vec2 vLogicalPos = pObj->GetWorldPos();
//     bool isFacingRight = pObj->GetIsFacingRight();
//
//     tAnimFrm& curFrame = m_vecFrm[m_iCurFrm];
//     Vec2 vOffset = curFrame.vOffset; // 오른쪽 기준 오프셋
//
//     //    캐릭터가 왼쪽을 볼 경우 애니메이션 오프셋 X값 반전
//     if (!isFacingRight)
//         vOffset.x *= -1.f;
//
//     float worldRotationAngle = pObj->GetWorldRotation();
//     float rotationRad = worldRotationAngle * (3.14159f / 180.f);
//     float cosAngle = cosf(rotationRad);
//     float sinAngle = sinf(rotationRad);
//
//     Vec2 vRotatedOffset;
//     vRotatedOffset.x = vOffset.x * cosAngle - vOffset.y * sinAngle;
//     vRotatedOffset.y = vOffset.x * sinAngle + vOffset.y * cosAngle;
//
//     Vec2 vVisualPos = vLogicalPos + vRotatedOffset;
//     Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vVisualPos); // 화면상 중심, 렌더링할 위치
//
//     float minRotationThreshold = 3.0f;
//
//     Graphics graphics(_dc);
//     graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
//     graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
//
//     Bitmap* frameBitmap = m_vecFrameBitmaps[m_iCurFrm]; // 항상 오른쪽 이미지
//     if (!frameBitmap) return;
//
//     float fDestWidth = static_cast<float>(frameBitmap->GetWidth());
//     float fDestHeight = static_cast<float>(frameBitmap->GetHeight());
//
//     
//     Matrix transformMatrix; // 단위 행렬로 시작
//
//     // 변환의 중심 위치
//     PointF centerPoint(vRenderPos.x, vRenderPos.y);
//
//     //  최종 위치로 이동 (이동 행렬을 먼저 적용)
//     //  이렇게 하면 이후의 모든 변환(회전, 스케일)이 centerPoint를 기준으로 적용됨
//     transformMatrix.Translate(centerPoint.X, centerPoint.Y, MatrixOrderPrepend);
//
//     // 회전 적용 (centerPoint가 원점이 된 상태에서 회전)
//     if (abs(worldRotationAngle) > minRotationThreshold)
//         transformMatrix.Rotate(worldRotationAngle, MatrixOrderPrepend); // 중심이 (0,0)으로 옮겨졌으므로 그냥 Rotate
//     
//
//     // 좌우 반전 적용 (centerPoint가 원점이 된 상태에서 X축 스케일)
//     if (!isFacingRight)
//         transformMatrix.Scale(-1.0f, 1.0f, MatrixOrderPrepend);
//
//     // DrawImage는 변환된 좌표계의 (0,0)을 기준으로 이미지의 중심이 오도록 그린다.
//     // 따라서 이미지의 좌상단은 (-width/2, -height/2)가 된다.
//     // 이 (0,0)은 이미 centerPoint로 이동되었고, 회전/반전이 적용된 상태
//     // DrawImage에 전달하는 좌표는 이미지의 로컬 중심을 (0,0)에 맞추는 값이어야 함
//     RectF drawingRect(-fDestWidth / 2.f, -fDestHeight / 2.f, fDestWidth, fDestHeight);
//     graphics.SetTransform(&transformMatrix);
//     graphics.DrawImage(frameBitmap, drawingRect);
//  
//     graphics.ResetTransform();
//     
//     CTimeMgr::EndTimer(L"AnimationComp_Render");
// }


// 프레임 캐싱 함수
// void CAnimation::CacheFrames()
// {
//     if (m_bCached || m_vecFrm.empty() || !m_pTex)
//         return;
//
//     // 기존에 캐싱된 비트맵 해제
//     for (auto& bitmap : m_vecFrameBitmaps)
//     {
//         delete bitmap;
//     }
//     m_vecFrameBitmaps.clear();
//
//     // 원본 비트맵 생성
//     HBITMAP hBitmap = m_pTex->GetHBITMAP();
//     Bitmap sourceBitmap(hBitmap, nullptr);
//
//     // 각 프레임별로 처리된 비트맵 생성 및 저장
//     for (size_t i = 0; i < m_vecFrm.size(); i++)
//     {
//         int srcX = static_cast<INT>(m_vecFrm[i].vLT.x);
//         int srcY = static_cast<INT>(m_vecFrm[i].vLT.y);
//         int srcWidth = static_cast<INT>(m_vecFrm[i].vSlice.x);
//         int srcHeight = static_cast<INT>(m_vecFrm[i].vSlice.y);
//
//         // 확대/축소될 최종 너비와 높이 계산
//         int destWidth = static_cast<int>(srcWidth * m_fSizeMulti);
//         int destHeight = static_cast<int>(srcHeight * m_fSizeMulti);
//
//         // 확대/축소된 크기의 새 비트맵 생성
//         Bitmap* frameBitmap = new Bitmap(destWidth, destHeight, PixelFormat32bppARGB);
//
//         Graphics frameGraphics(frameBitmap);
//         // 확대/축소 시 보간 품질 설정 (필요에 따라 NearestNeighbor 외 다른 값 사용 가능)
//         frameGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
//
//         ImageAttributes imgAttr;
//         imgAttr.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);
//
//         // 원본 이미지의 해당 부분을 확대/축소하여 새 비트맵에 복사
//         frameGraphics.DrawImage(
//             &sourceBitmap,
//             Rect(0, 0, destWidth, destHeight), // 대상 사각형 (확대/축소된 크기)
//             srcX, srcY, srcWidth, srcHeight,   // 소스 부분 (원본 이미지에서 가져올 영역)
//             UnitPixel,
//             &imgAttr
//         );
//
//         m_vecFrameBitmaps.push_back(frameBitmap);
//     }
//
//     m_bCached = true;
// }
