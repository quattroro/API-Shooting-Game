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
	virtual BOOL isMapCollision(POINT rectpos, int size, POINT cpos/*�߽���*/, int radious);
	virtual BOOL isCollision(POINT pos1, int radious1, POINT pos2, int radious2);
	//BOOL CameraMove(TCHAR key);
	//void KeyInput(WPARAM wParam);
	//CreateThread�Լ������� �Ϲ��Լ��� ȣ���� �� �־ ������ �Լ��� static���� ����� �ְ� �Ķ���ͷ� ��ü �ڽ��� �־ ������ �Լ� �ȿ��� ��ü�Լ��� ȣ�� �� �� �ֵ��� ���־���.
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


	//���͵� ������ �Ѿ� �߻�, ��� ����������Լ���
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

	

	

	//�÷��� ����
	int NowStage = 0;
	POINT MyPosIndex;//���� Ÿ�Ͽ��� ���� �ִ� �ε���
	POINT MyPos;//���� ȭ�鿡�� ���� �ִ� ��ġ
	int maxmonster = 0;
	POINT WorldMapSize;
	RECT crt;//���� ȭ���� ����
	
	
	//�Ѿ��� ���� �Ѿ�, ���� �Ѿ� ���� ���� ����

public:
	static CRITICAL_SECTION crit;

	//Ÿ�� �ø��� ���� ����//�ѹ��� �޾ƿ��� ������ 18*11 //ȭ�鿡�� �ִ� 16*9���� ������
	int scrollIndex_X;//ȭ���� ��ġ (�ε��� ��)
	int scrollIndex_Y;
	int scroll_X = 0;//ȭ���� ��ġ (��ǥ��)
	int scroll_Y = 0;
	int interval_X = 60;
	int interval_Y = 60;

	POINT MaxGameField;
	POINT MyPosWorld;//��ü ���忡�� ���� ���� �ִ� ��ġ
	POINT MyPosWorldIndex;
	int MovingDistance_X;//���� ���� ������ �Ÿ� ������ �Ÿ��� 60�̻��� �Ǹ� �� ��ġ�� ���� ���� �ٽ� �о�´�.
	int MovingDistance_Y;
	
	MyDoubleBuffer* doublebuffer;
	//Ű����
	BOOL KeyBuffer[KEYMAX];
	//�������ڵ�
	HANDLE hThread[MAINTHREADMAX];
	//HANDLE MonsterhThread[MTHREADMAX];
	//������ �̺�Ʈ �ڵ�
	HANDLE hEvent[MAINTHREADMAX];
	//HANDLE MonsterhEvent[MTHREADMAX];



	POINT mousepoint;
	//HBITMAP MainBitmaps[BITMAX];
	//������ ��ü��
	MyCharacter* mycharacter;
	list<POINT> spawnlist;
	list<BaseMonster*> monsterlist;
	list<BaseBullet*> bulletlist;
	list<MyChest*> chestlist;
	//��������
	//BOOL GameClear;
	//BOOL GameOver;
	int NowGameState;
	int SelectCharacter = -1;

	//������
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
			{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},//���� ��������
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
	//��ó�� ĳ���͸� �����ϱ� ���� ��ġ
	RECT CharacterSelectPos[MyCharacter::CHARECTERS::CMAX];

};







