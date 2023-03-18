#pragma once
#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include "Mmsystem.h"
#include "Digitalv.h"


class SoundPlayer
{
public:
	enum SOUNDS { PISTOL, ROCKET, BANDITGUN, SCORPIONGUN, BIGBANDITGUN, CLICK, BANDITDIE, SCORPIONDIE, MAGGOTDIE, MAGGOTNESTDIE,CHARACTERHIT,CHARACTERDEAD,FISHSKILL,CRYSTALCKILL, SOUNDMAX };
	enum BGM { CHARACTERCELECT, DRYLAND, BIGBANDIT, BGMMAX };
	SoundPlayer()
	{
		SoundFileName[PISTOL] = "BGM/gen_2E(권총).wav";
		SoundFileName[CLICK] = "BGM/ClickSound.wav";
		SoundFileName[SCORPIONGUN] = "BGM/ScorpionGun.wav";
		SoundFileName[BANDITGUN] = "BGM/BanDitGun.wav";
		SoundFileName[BIGBANDITGUN] = "BGM/BanDitGun.wav";
		SoundFileName[BANDITDIE] = "BGM/BanDitDie.wav";
		SoundFileName[SCORPIONDIE] = "BGM/ScorpionDie.wav";
		SoundFileName[MAGGOTDIE] = "BGM/MaggotDie.wav";
		SoundFileName[MAGGOTNESTDIE] = "BGM/MaggotNestDie.wav";
		SoundFileName[ROCKET] = "BGM/gen_46(레이저).wav";
		SoundFileName[CHARACTERHIT] = "gen_3D(캐릭터 피격).wav";
		SoundFileName[CHARACTERDEAD] = "gen_36(죽는소리).wav";
		SoundFileName[FISHSKILL] = "BGM/gen_40(물고기 특능).wav";
		SoundFileName[CRYSTALCKILL] = "BGM/st2_0C(크리스탈 특능).wav";

		BGMFileName[CHARACTERCELECT] = "BGM/gen_2E(권총).wav";
		BGMFileName[DRYLAND] = "BGM/DryLandBGM.mp3";
		BGMFileName[BIGBANDIT] = "BGM/BossStageBGM.mp3";

	}

	void PlayEffect(int kind)
	{
		sndPlaySoundA(SoundFileName[kind], SND_ASYNC | SND_NODEFAULT);
		//switch (kind)
		//{
		//case PISTOL:
		//	sndPlaySoundA("BGM/gen_2E(권총).wav", SND_ASYNC | SND_NODEFAULT);
		//	//mciOpen.lpstrElementName = "gen_2E(권총).wav"; // 파일 경로 입력
		//	



		//	//mciOpen.lpstrElementName = SoundFileName[PISTOL]; // 파일 경로 입력
		//	//mciOpen.lpstrDeviceType = "waveaudio";
		//	//mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
		//	//dwID = mciOpen.wDeviceID;
		//	////볼륨조절
		//	//dwVolume = 100;
		//	////MCI_DGV_SETAUDIO_PARMS SetAudio;
		//	//SetAudio.dwCallback = 0;
		//	//SetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		//	//SetAudio.dwValue = dwVolume;
		//	//SetAudio.dwOver = 0;
		//	//SetAudio.lpstrAlgorithm = NULL;
		//	//SetAudio.lpstrQuality = NULL;
		//	//mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&SetAudio);

		//	//mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY/*MCI_DGV_PLAY_REPEAT*/, (DWORD)(LPVOID)&m_mciPlayParms);
		//	//Sleep(120);
		//	//mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
		//	break;

		//case ROCKET:

		//	break;

		//case BANDITGUN:
		//	sndPlaySoundA("BGM/gen_2E(권총).wav", SND_ASYNC | SND_NODEFAULT);
		//	break;

		//case SCORPIONGUN:

		//	break;

		//case BIGBANDITGUN:

		//	break;

		//case CLICK:

		//	sndPlaySoundA(SoundFileName[CLICK], SND_ASYNC | SND_NODEFAULT);


		//	//mciOpen.lpstrElementName = SoundFileName[CLICK]; // 파일 경로 입력
		//	//mciOpen.lpstrDeviceType = "waveaudio";
		//	//mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
		//	//dwID = mciOpen.wDeviceID;
		//	////볼륨조절
		//	//dwVolume = 100;
		//	////MCI_DGV_SETAUDIO_PARMS SetAudio;
		//	//SetAudio.dwCallback = 0;
		//	//SetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		//	//SetAudio.dwValue = dwVolume;
		//	//SetAudio.dwOver = 0;
		//	//SetAudio.lpstrAlgorithm = NULL;
		//	//SetAudio.lpstrQuality = NULL;
		//	//mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&SetAudio);

		//	//mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlayParms);
		//	//Sleep(100);
		//	//mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);

		//	break;
		//default:

		//	break;
		//}
	}

	void PlayBGM(int kind)
	{
		/*for (int i = 0; i < BGMMAX; i++)
		{
			mciSendCommand(dwID[i], MCI_CLOSE, 0, (DWORD)NULL);
			mciSendCommand(dwID[i], MCI_CLOSE, 0, (DWORD)NULL);
		}*/
		/*if (NowPlayedBGM == BGM::DRYLAND)
		{
			mciSendCommand(dwID[DRYLAND], MCI_CLOSE, 0, (DWORD)NULL);
		}
		else if (NowPlayedBGM == BGM::BIGBANDIT)
		{
			mciSendCommand(dwID[BIGBANDIT], MCI_CLOSE, 0, (DWORD)NULL);
		}*/
		//mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, MCI_WAIT, (DWORD)NULL);
		mciSendCommand(1, MCI_CLOSE, MCI_NOTIFY, (DWORD)NULL);
		mciSendCommand(2, MCI_CLOSE, MCI_NOTIFY, (DWORD)NULL);
		switch (kind)
		{
		case BGM::DRYLAND:
		
			if (NowPlayedBGM != BGM::DRYLAND)
			{
				NowPlayedBGM = BGM::DRYLAND;
			}
			mciOpen[DRYLAND].lpstrElementName = BGMFileName[DRYLAND]; // 파일 경로 입력
			mciOpen[DRYLAND].lpstrDeviceType = "mpegvideo";
			mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen[DRYLAND]);//오픈
			dwID[DRYLAND] = mciOpen[DRYLAND].wDeviceID;
			//볼륨조절
			dwVolume = 1000;

			SetAudio[DRYLAND].dwCallback = 0;
			SetAudio[DRYLAND].dwItem = MCI_DGV_SETAUDIO_VOLUME;
			SetAudio[DRYLAND].dwValue = dwVolume;
			SetAudio[DRYLAND].dwOver = 0;
			SetAudio[DRYLAND].lpstrAlgorithm = NULL;
			SetAudio[DRYLAND].lpstrQuality = NULL;
			mciSendCommand(dwID[DRYLAND], MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&SetAudio[DRYLAND]);//설정 입력

			mciSendCommand(dwID[DRYLAND], MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay[DRYLAND]);//플레이


			//mciSendCommand(dwID[DRYLAND], MCI_CLOSE, 0, (DWORD)NULL);

			//mciOpen[BIGBANDIT].lpstrElementName = BGMFileName[BIGBANDIT]; // 파일 경로 입력
			//mciOpen[BIGBANDIT].lpstrDeviceType = "mpegvideo";
			//mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen[BIGBANDIT]);
			//dwID[BIGBANDIT] = mciOpen[BIGBANDIT].wDeviceID;
			////볼륨조절
			//dwVolume = 1000;
			////MCI_DGV_SETAUDIO_PARMS SetAudio;
			//SetAudio[BIGBANDIT].dwCallback = 0;
			//SetAudio[BIGBANDIT].dwItem = MCI_DGV_SETAUDIO_VOLUME;
			//SetAudio[BIGBANDIT].dwValue = dwVolume;
			//SetAudio[BIGBANDIT].dwOver = 0;
			//SetAudio[BIGBANDIT].lpstrAlgorithm = NULL;
			//SetAudio[BIGBANDIT].lpstrQuality = NULL;
			//mciSendCommand(dwID[BIGBANDIT], MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&SetAudio[BIGBANDIT]);

			//mciSendCommand(dwID[BIGBANDIT], MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay[BIGBANDIT]);
			//mciSendCommand(dwID[DRYLAND], MCI_CLOSE, 0, (DWORD)NULL);

			//mciSendCommand(dwID[DRYLAND], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay[DRYLAND]);
			break;

		case BGM::BIGBANDIT:
		
			if (NowPlayedBGM != BGM::BIGBANDIT)
			{
				NowPlayedBGM = BGM::BIGBANDIT;
			}
			mciOpen[BIGBANDIT].lpstrElementName = BGMFileName[BIGBANDIT]; // 파일 경로 입력
			mciOpen[BIGBANDIT].lpstrDeviceType = "mpegvideo";
			mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen[BIGBANDIT]);
			dwID[BIGBANDIT] = mciOpen[BIGBANDIT].wDeviceID;
			//볼륨조절
			dwVolume = 1000;
			//MCI_DGV_SETAUDIO_PARMS SetAudio;
			SetAudio[BIGBANDIT].dwCallback = 0;
			SetAudio[BIGBANDIT].dwItem = MCI_DGV_SETAUDIO_VOLUME;
			SetAudio[BIGBANDIT].dwValue = dwVolume;
			SetAudio[BIGBANDIT].dwOver = 0;
			SetAudio[BIGBANDIT].lpstrAlgorithm = NULL;
			SetAudio[BIGBANDIT].lpstrQuality = NULL;
			mciSendCommand(dwID[BIGBANDIT], MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&SetAudio[BIGBANDIT]);

			mciSendCommand(dwID[BIGBANDIT], MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay[BIGBANDIT]);
			//mciSendCommand(dwID[BIGBANDIT], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay[BIGBANDIT]);
			break;

		case BGM::CHARACTERCELECT:

			break;
		}
		
	}

	void BGMStop()
	{
		if (NowPlayedBGM == BGM::DRYLAND)
		{
			mciSendCommand(dwID[DRYLAND], MCI_CLOSE, MCI_NOTIFY, (DWORD)NULL);
		}
		else if (NowPlayedBGM == BGM::BIGBANDIT)
		{
			mciSendCommand(dwID[BIGBANDIT], MCI_CLOSE, MCI_NOTIFY, (DWORD)NULL);
		}
		else if (NowPlayedBGM == BGM::CHARACTERCELECT)
		{
			mciSendCommand(dwID[CHARACTERCELECT], MCI_CLOSE, MCI_NOTIFY, (DWORD)NULL);
		}
	}

	BOOL GetNowPlayedBGM()
	{
		return NowPlayedBGM;
	}


public:
	/*MCI_OPEN_PARMS m_mciOpenParms;
	MCI_PLAY_PARMS m_mciPlayParms;
	DWORD m_dwDeviceID;*/



	MCI_OPEN_PARMS mciOpen[BGMMAX];
	MCI_PLAY_PARMS mciPlay[BGMMAX];
	DWORD dwID[BGMMAX];
	//TCHAR FileName[SOUNDMAX];

	DWORD dwVolume;
	MCI_DGV_SETAUDIO_PARMS SetAudio[BGMMAX];
	const TCHAR* SoundFileName[SOUNDMAX];
	const TCHAR* BGMFileName[BGMMAX];


	int NowPlayedBGM;
};

