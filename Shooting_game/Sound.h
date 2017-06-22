#pragma once
#include <fmod.h>
#include <string>
using namespace std;

class CSound
{
private:
	FMOD_SYSTEM* gSystem;
	FMOD_SOUND** ppBGsound;
	FMOD_SOUND** ppEFFsound;
	FMOD_CHANNEL** ppBGchannel;
	int nBGsoundcount;
	int nEFFsoundcount;

public:
	void CreateEFFsound(int nCount, string *SoundFileName);
	void CreateBGsound(int nCount, string *SoundFileName);
	void PlaySoundEFF(int nindex);
	void PlaySoundBG(int nindex);
	void StopSoundBG(int nindex);
	void ReleaseSound();
	void Update();

public:
	CSound(void);
	~CSound(void);
};