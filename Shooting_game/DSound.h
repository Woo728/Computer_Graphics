#pragma once
#include <windows.h>
#include <mmsystem.h> //�� ��������� windows.h�� ������ ��ũ�ο����� �ޱ�
//������ �ݵ�� windows.h�� ���Ե� ������ ��Ŭ���
//�� ��� �Ѵ�.
#include <dsound.h>

#ifndef DSBCAPS_CTRLDEFAULT  //6.0���� ���� ���������Ƿ� define���� 
#define DSBCAPS_CTRLDEFAULT  (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif

//�Է°��� dB(decibel)�� �ٲ��ִ� ��ũ�� �Լ�
#define DSVOLUME_TO_DB(volume)   ((DWORD)(-30*(100-volume))) 

BOOL CreateDirectSound(HWND hWnd);
BOOL LoadWave(LPWSTR lpFileName, LPDIRECTSOUNDBUFFER* lpDSBuffer);
BOOL SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume);
BOOL SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lPan);

void DeleteDirectSound();
void Play(LPDIRECTSOUNDBUFFER lpDSBuffer, BOOL Loop);
void Stop(LPDIRECTSOUNDBUFFER lpDSBuffer);