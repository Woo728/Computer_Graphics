#include "Sound.h"

CSound::CSound(void)
{
	FMOD_System_Create(&gSystem);
	FMOD_System_Init(gSystem, 32, FMOD_INIT_NORMAL, NULL);
}

CSound::~CSound(void)
{
	FMOD_System_Close(gSystem);
	FMOD_System_Release(gSystem);
}

void CSound::CreateBGsound(int nCount, string *SoundFileName)
{
	nBGsoundcount = nCount;
	ppBGsound = new FMOD_SOUND*[nCount];
	ppBGchannel = new FMOD_CHANNEL*[nCount];

	for (int i = 0; i < nCount; i++)
		FMOD_System_CreateSound(gSystem, SoundFileName[i].data(), FMOD_LOOP_NORMAL, 0, &ppBGsound[i]);
}

void CSound::CreateEFFsound(int nCount, string *SoundFileName)
{
	nEFFsoundcount = nCount;
	ppEFFsound = new FMOD_SOUND*[nCount];

	for (int i = 0; i < nCount; i++)
	{
		FMOD_Channel_SetVolume(ppBGchannel[i], 1.0f);
		FMOD_System_CreateSound(gSystem, SoundFileName[i].data(), FMOD_DEFAULT, 0, &ppEFFsound[i]);
	}
}

void CSound::PlaySoundEFF(int nindex)
{
	if (nindex < nEFFsoundcount)
	{
		FMOD_CHANNEL *pChannel;
		FMOD_Channel_SetVolume(pChannel, 1.0f);
		FMOD_System_PlaySound(gSystem, ppEFFsound[nindex], 0, 0, &pChannel);
	}
}

void CSound::PlaySoundBG(int nindex)
{
	if (nindex < nBGsoundcount)
	{
		FMOD_System_PlaySound(gSystem, ppEFFsound[nindex], 0, 0, &ppBGchannel[nindex]);
	}
}

void CSound::StopSoundBG(int nindex)
{
	if (nindex < nBGsoundcount)
	{
		FMOD_Channel_Stop(ppBGchannel[nindex]);
	}
}

void CSound::ReleaseSound()
{
	int i;

	delete[] ppBGchannel;

	for (i = 0; i < nBGsoundcount; i++)
		FMOD_Sound_Release(ppBGsound[i]);
	delete[] ppBGsound;
	for (i = 0; i < nBGsoundcount; i++)
		FMOD_Sound_Release(ppEFFsound[i]);
	delete[] ppEFFsound;
}

void CSound::Update()
{
	if (!gSystem)
		FMOD_System_Update(gSystem);
}