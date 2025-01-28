﻿#pragma once
#include "CRes.h"
#include "CSoundMgr.h"
class CSound :
    public CRes
{
private:
    LPDIRECTSOUNDBUFFER m_pSoundBuffer;
    DSBUFFERDESC        m_tBuffInfo;
    int                 m_iVolume;

public:
    int Load(const wstring& _strPath);

    //일반 재생
    void Play(bool _bLoop = false);

    //BGM으로 재생
    void PlayToBGM(bool _bLoop = false);
    
    void Stop(bool _bReset = false);

    //볼륨 범위(0~100)
    void SetVolume(float _fVolume);

    //음악파일(사운드트랙) 위치 조정
    void SetPosition(float _fPosition); //0~100사이

private:
    bool LoadWaveSound(const wstring& _strPath);
    int GetDecibel(float _fVolume);

public:
    CSound();
    virtual ~CSound();
};

