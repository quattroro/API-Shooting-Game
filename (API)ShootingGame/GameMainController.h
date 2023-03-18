#pragma once
#pragma comment(lib, "winmm.lib")
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>


#include "BaseMainController.h"
#include "MyDoubleBuffer.h"
#include "Mmsystem.h"
#include "Digitalv.h"
#include "MyCharacter.h"
#include "CharacterFish.h"
#include "BaseMonster.h"
#include "BanDitMonster.h"
#include "BaseBullet.h"
#include "ScorpionMonster.h"
#include "MaggotMonster.h"
#include "MaggoyNestMonster.h"
#include "MyChest.h"
#include "BigBanDitMonster.h"
//#include "SoundPlayer.h"
#include "CharacterCrystal.h"
#include "SoundPlayer.h"

#define MapTileSize 32
#define MaxStage 6
#define MapsizeX 40
#define MapsizeY 20
#define PlaySizeX 18
#define PlaySizeY 11

//class MyCharacter;
//class CharacterFish;

using namespace std;
class GameMainController :public BaseMainController
{
public:
	//enum BITMAPS { CHARACTERFISH, CHARACTERCRSTAL, MONSTERBANDIT, MONSTERMAGGOT, MONSTERMAGGOTNEST, MONSTERSCORPION, MONSTERBIGBANDIT, GUNS, BULLETS, AIM, CHESTS, BITMAX };
	enum MAPBITS { SAND1, SAND2, SAND3, SAND4, PEBBLE1, PEBBLE2, PEBBLE3, PEBBLE4, CLAY1, CLAY2, CLAY3, CLAY4, ROCK, GRASS, ROCKS1, ROCKS2, ROCKS3, ROCKS4, ROCKTOP1, ROCKTOP2, ROCKTOP3, ROCKTOP4, MAINBITMAX };
	enum MAPINFO { BLANK, WALL, WALL2, WALL3, PSPAWN, ESPAWN, ENDPOINT, MAPMAX };
	enum KEYINFO { RIGHT, DOWN, LEFT, UP, NUM1, NUM2, KEYMAX };
	enum MAINTHREADS { MAINTHREAD, MOVING, MOTION, SHOOT, MAINTHREADMAX };
	//enum MONSTERTHREAD {  MTHREADMAX };
	enum STAGE { NOTHING = -1, STAGE1, STAGE2, STAGEMAX };
	enum GAMESTATE { CSELLECT, PLAYING, GAMEOVER, STAGECLEAR, GAMECLEAR };
	static GameMainController* ControllerInstance();
	static GameMainController* _Instance;
	

	GameMainController();
	void InitSetting(HINSTANCE hInst, HWND MhWnd, MyDoubleBuffer* doublebuffer);
	void StageSetting();
	BOOL SetScroll();
	void LoadPlayMap();
	virtual BOOL DetectMapCollision(POINT pos, int radious);
	virtual BOOL isMapCollision(POINT rectpos, int size, POINT cpos/*중심점*/, int radious);
	virtual BOOL isCollision(POINT pos1, int radious1, POINT pos2, int radious2);
	//BOOL CameraMove(TCHAR key);
	//void KeyInput(WPARAM wParam);
	//CreateThread함수에서는 일반함수만 호출할 수 있어서 스레드 함수를 static으로 만들어 주고 파라미터로 객체 자신을 주어서 스레드 함수 안에서 객체함수를 호출 할 수 있도록 해주었다.
	static DWORD WINAPI MainThreadFunc(LPVOID p);

	void MoveThreadLoop();
	void MouseMove(LPARAM lParam);
	void MouseClick(LPARAM lParam);

	void MouseRClick();
	BOOL CharacterDash();


	void UpMove(POINT pos, int speed);
	void DownMove(POINT pos, int speed);
	void RightMove(POINT pos, int speed);
	void LeftMove(POINT pos, int speed);

	//void KeyInPut();
	void ScreenRender();
	void DrawCharacter();
	//void ScreenCulling(POINT pos);
	void KeyDownProc(WPARAM wParam);
	void KeyUpProc(WPARAM wParam);
	virtual POINT GetCharacterPos();
	void MonsterSpawn(int x, int y);
	void DrawEnemy();
	virtual POINT GetScrollindex();
	virtual float GetPointDistnace(POINT pt1, POINT pt2);
	virtual POINT GetMousePos();
	virtual float GetAngle(POINT startpoint, POINT destpoint);
	void BulletMove(BaseBullet* p);
	void BulletsMove();
	void DrawBullet();
	BOOL BulletCollisionToRef(POINT pos, BaseBullet* p);
	
	void DeleteMyObject();
	void Charactercollision();

	void DrawChest();
	void DrawUI(); 
	virtual POINT GetScrollPos();
	void GameIsOver();
	void RefClear();

	void DrawAim();

	void GameIsClear();

	~GameMainController()
	{
		DeleteCriticalSection(&crit);
	}


	//몬스터들 움직임 총알 발사, 모션 스레드관련함수들
	static DWORD WINAPI MonsterMovingThreadFunc(LPVOID p);

	static DWORD WINAPI MonsterMotionThreadFunc(LPVOID p);

	static DWORD WINAPI MonsterShootThreadFunc(LPVOID p);

	
private:
	
	HWND hWndMain;
	HINSTANCE M_hInst;
	
	HBITMAP StageBit[STAGEMAX];
	HBITMAP GameOverBit;
	HBITMAP GameClearBit;
	RECT MapBitRect[MAINBITMAX];

	

	

	//플레이 정보
	int NowStage = 0;
	POINT MyPosIndex;//게임 타일에서 내가 있는 인덱스
	POINT MyPos;//게임 화면에서 내가 있는 위치
	int maxmonster = 0;
	POINT WorldMapSize;
	RECT crt;//게임 화면의 영역
	
	
	//총알은 유저 총알, 몬스터 총알 따로 만들어서 관리

public:
	static CRITICAL_SECTION crit;

	//타일 컬링을 위한 정보//한번에 받아오는 영역은 18*11 //화면에는 최대 16*9까지 보여짐
	int scrollIndex_X;//화면의 위치 (인덱스 값)
	int scrollIndex_Y;
	int scroll_X = 0;//화면의 위치 (좌표값)
	int scroll_Y = 0;
	int interval_X = 60;
	int interval_Y = 60;

	POINT MaxGameField;
	POINT MyPosWorld;//전체 월드에서 현재 내가 있는 위치
	POINT MyPosWorldIndex;
	int MovingDistance_X;//현재 내가 움직인 거리 움직인 거리가 60이상이 되면 내 위치에 따라 맵을 다시 읽어온다.
	int MovingDistance_Y;
	
	MyDoubleBuffer* doublebuffer;
	//키버퍼
	BOOL KeyBuffer[KEYMAX];
	//스레드핸들
	HANDLE hThread[MAINTHREADMAX];
	//HANDLE MonsterhThread[MTHREADMAX];
	//스레드 이벤트 핸들
	HANDLE hEvent[MAINTHREADMAX];
	//HANDLE MonsterhEvent[MTHREADMAX];



	POINT mousepoint;
	//HBITMAP MainBitmaps[BITMAX];
	//관리할 객체들
	MyCharacter* mycharacter;
	list<POINT> spawnlist;
	list<BaseMonster*> monsterlist;
	list<BaseBullet*> bulletlist;
	list<MyChest*> chestlist;
	//게임정보
	//BOOL GameClear;
	//BOOL GameOver;
	int NowGameState;
	int SelectCharacter = -1;

	//맵정보
	int WordMapInfo[MaxStage][MapsizeY][MapsizeX] =
	{
		{
			{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
			{2,2,3,3,3,3,3,2,2,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
			{2,2,1,0,0,0,3,3,3,3,3,3,3,3,3,0,0,0,3,3,3,3,3,3,2,2,2,2,2,2,2,2,3,3,3,3,3,3,2,2},
			{2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,3,3,3,2,2,2,2,2,3,0,0,5,0,3,2,2},
			{2,2,2,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,3,2,2,2,2,2,3,0,0,0,0,3,2,2},
			{2,2,2,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,2,2,2,3,0,0,0,3,2,2},
			{2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,3,2,2,2,2,2,3,0,0,0,3,2,2},
			{2,2,2,1,0,0,4,0,0,0,1,1,1,1,1,0,5,0,0,0,1,1,0,0,0,0,0,0,3,2,2,2,3,0,0,0,0,3,2,2},
			{2,2,2,1,0,0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,1,1,0,0,0,0,0,0,3,3,3,3,0,0,0,0,0,3,2,2},
			{1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2},
			{1,0,0,1,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,3,2,2,2,2},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2},
			{1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2},
			{2,2,2,1,0,0,0,1,1,2,2,2,2,2,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2},
			{2,2,2,1,1,1,1,1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2},
			{2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,2,2,2},
			{2,2,2,2,2,2,2,2,2,2,2,2,1,0,5,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}

		},
		{
			{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},//보스 스테이지
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,4,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2}
		}

	};
	BOOL Debuging = FALSE;


	//SoundPlayer* soundplayer;
	int dashcount = 0;
	BOOL DashBuffer[4] = { 0 };
	HBITMAP startscreen;
	HBITMAP CharacterCard;
	//sound
	SoundPlayer* soundplayer;
	//맨처음 캐릭터를 선택하기 위한 위치
	RECT CharacterSelectPos[MyCharacter::CHARECTERS::CMAX];

};







