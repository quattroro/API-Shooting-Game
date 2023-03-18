#pragma once
#include <Windows.h>
#include "BaseMainController.h"
#include "MyDoubleBuffer.h"
#include "BaseBullet.h"
#include "MyChest.h"
#define MapTileSize 32
#define MaxStage 6
#define MapsizeX 40
#define MapsizeY 20
#define PlaySizeX 18
#define PlaySizeY 11

class BaseMonster
{
public:
	enum STATE { IDLE, WALKING, HURT, DEAD, RELOADING };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//위와 아래방향의 비트맵은 없기 때문에 두 방향으로만 확인
	enum ENEMYS { BANDIT, MAGGOTNEST, SCORPION, ENEMYMAX, MAGGOT };
	enum QUADRANT { Q1, Q2, Q3, Q4 };
	BaseMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos)
	{
		maincontroller = p;
		m_hInst = hInst;
		hWndMain = hWnd;
		enemypos = pos;
		CenterPos.x = enemypos.x + 30;
		CenterPos.y = enemypos.y + 30;
		NowShoot = FALSE;
		isDestroy = FALSE;

	}

	virtual void Movingloop()//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
	{

	}
	virtual void MotionMovingLoop()
	{

	}
	virtual void Shootloop()
	{

	}

	virtual void ShootBullet(/*POINT start, float angle*/)
	{

	}

	//virtual float GetAngle(POINT startpoint, POINT destpoint) = 0;
	virtual void MonsterMove(float angle)
	{
	}
	virtual void DrawMonster()
	{
	}

	virtual POINT GetEnemyPos()
	{
		return enemypos;
	}
	virtual void SetEnemyPos(int x, int y)
	{
		enemypos.x = x;
		enemypos.y = y;
	}
	virtual POINT GetConterPos()
	{
		return CenterPos;
	}
	virtual void SetConterPos(int x, int y)
	{
		CenterPos.x = x;
		CenterPos.y = y;
	}
	virtual int HitBox_Radious() = 0;
	virtual POINT HitBox_Pos() = 0;
	virtual ~BaseMonster()
	{
		
	}

	virtual void MonsterHit(BaseBullet* p) = 0;
	virtual int GetDemage()
	{
		return demage;
	}

	virtual BOOL NowShootable()
	{
		if (NowShoot)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	virtual void DropItem() = 0;//30%확률로 작은 총알상자, 20%확률로 큰 총알상자, 17%확률로 체력상자, 10%확률로 무기상자


	virtual int GetState()
	{
		return NowState;
	}

	virtual void SetState(int m)
	{
		NowState = m;
		BitNum = 0;
	}
	BOOL DetectRay(POINT start, POINT dest)//시작지점에서 도착지점까지 직선으로 이동했을때 중간에 지형이 없는지 확인할 함수(몬스터가 지형에 걸리는 데도 계속 총을 쏘고 있는 것을 막기 위해)
	{
		BOOL bigx = FALSE;
		BOOL bigy = FALSE;
		float angle;
		if (start.x - dest.x >= 0)
		{
			bigx = TRUE;
		}
		if (start.y - dest.y >= 0)
		{
			bigy = TRUE;
		}

		angle = (maincontroller)->GetAngle(start, dest);
		//MonsterMove()
		while (true)
		{
			start.x = start.x + cos(angle * (3.141592f / 180.0f)) * 30;//호도법을 라디안으로 바꿔서 입력해준다.
			start.y = start.y + (sin(angle * (3.141592f / 180.0f)) * 30);
			if (bigx == TRUE && bigy == TRUE)//1씩 움직이면서 검사하는것이 아니기 때문에 혹시 목표지점을 넘어가더라도 멈출 수 있게 하기 위해 가능한 4가지 경우에 따라서 조건을 걸러준다.
			{//시작점이 x,y 모두 도착점보다 큰 좌표에 있을때
				if (start.x <= dest.x && start.y <= dest.y)
				{
					break;
				}
			}
			if (bigx == TRUE && bigy == FALSE)
			{//x는 크고 y는 작을때
				if (start.x <= dest.x && start.y >= dest.y)
				{
					break;
				}
			}
			if (bigx == FALSE && bigy == TRUE)
			{//x는 작고 y는 클때

				if (start.x >= dest.x && start.y <= dest.y)
				{
					break;
				}
			}
			if (bigx == FALSE && bigy == FALSE)
			{//x,y 모두 작을때

				if (start.x >= dest.x && start.y >= dest.y)
				{
					break;
				}
			}
			if ((maincontroller)->DetectMapCollision(start, 5))//지름이 10인 총알을 발사했다고 생각하고 경로상에 벽이 있는지 검사해서 벽이 있으면 FALSE를 리턴
			{
				return FALSE;
			}

		}
		return TRUE;//벽이 없으면 TRUE를 리턴한다.
	}


protected:
	HWND hWndMain;
	HINSTANCE m_hInst;
	POINT enemypos;
	POINT CenterPos;
	BaseMainController* maincontroller;
	int demage;
	//HBITMAP MonsterBitmap[ENEMYMAX];
	int BitNum;//동작 번호
	int NowState;//현재 상태
	int NowDirection;//현재 방향
public:
	BOOL isDestroy;
	BOOL NowShoot;
};





