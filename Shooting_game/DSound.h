#pragma once
#include <windows.h>
#include <mmsystem.h> //이 헤더파일은 windows.h에 정의한 매크로영향을 받기
//때문에 반드시 windows.h를 포함된 다음에 인클루드
//해 줘야 한다.
#include <dsound.h>

#ifndef DSBCAPS_CTRLDEFAULT  //6.0이후 부터 없어졌으므로 define해줌 
#define DSBCAPS_CTRLDEFAULT  (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif

//입력값을 dB(decibel)로 바꿔주는 매크로 함수
#define DSVOLUME_TO_DB(volume)   ((DWORD)(-30*(100-volume))) 

BOOL CreateDirectSound(HWND hWnd);
BOOL LoadWave(LPWSTR lpFileName, LPDIRECTSOUNDBUFFER* lpDSBuffer);
BOOL SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume);
BOOL SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lPan);

void DeleteDirectSound();
void Play(LPDIRECTSOUNDBUFFER lpDSBuffer, BOOL Loop);
void Stop(LPDIRECTSOUNDBUFFER lpDSBuffer);