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
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//���� �Ʒ������� ��Ʈ���� ���� ������ �� �������θ� Ȯ��
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

	virtual void Movingloop()//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )
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

	virtual void DropItem() = 0;//30%Ȯ���� ���� �Ѿ˻���, 20%Ȯ���� ū �Ѿ˻���, 17%Ȯ���� ü�»���, 10%Ȯ���� �������


	virtual int GetState()
	{
		return NowState;
	}

	virtual void SetState(int m)
	{
		NowState = m;
		BitNum = 0;
	}
	BOOL DetectRay(POINT start, POINT dest)//������������ ������������ �������� �̵������� �߰��� ������ ������ Ȯ���� �Լ�(���Ͱ� ������ �ɸ��� ���� ��� ���� ��� �ִ� ���� ���� ����)
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
			start.x = start.x + cos(angle * (3.141592f / 180.0f)) * 30;//ȣ������ �������� �ٲ㼭 �Է����ش�.
			start.y = start.y + (sin(angle * (3.141592f / 180.0f)) * 30);
			if (bigx == TRUE && bigy == TRUE)//1�� �����̸鼭 �˻��ϴ°��� �ƴϱ� ������ Ȥ�� ��ǥ������ �Ѿ���� ���� �� �ְ� �ϱ� ���� ������ 4���� ��쿡 ���� ������ �ɷ��ش�.
			{//�������� x,y ��� ���������� ū ��ǥ�� ������
				if (start.x <= dest.x && start.y <= dest.y)
				{
					break;
				}
			}
			if (bigx == TRUE && bigy == FALSE)
			{//x�� ũ�� y�� ������
				if (start.x <= dest.x && start.y >= dest.y)
				{
					break;
				}
			}
			if (bigx == FALSE && bigy == TRUE)
			{//x�� �۰� y�� Ŭ��

				if (start.x >= dest.x && start.y <= dest.y)
				{
					break;
				}
			}
			if (bigx == FALSE && bigy == FALSE)
			{//x,y ��� ������

				if (start.x >= dest.x && start.y >= dest.y)
				{
					break;
				}
			}
			if ((maincontroller)->DetectMapCollision(start, 5))//������ 10�� �Ѿ��� �߻��ߴٰ� �����ϰ� ��λ� ���� �ִ��� �˻��ؼ� ���� ������ FALSE�� ����
			{
				return FALSE;
			}

		}
		return TRUE;//���� ������ TRUE�� �����Ѵ�.
	}


protected:
	HWND hWndMain;
	HINSTANCE m_hInst;
	POINT enemypos;
	POINT CenterPos;
	BaseMainController* maincontroller;
	int demage;
	//HBITMAP MonsterBitmap[ENEMYMAX];
	int BitNum;//���� ��ȣ
	int NowState;//���� ����
	int NowDirection;//���� ����
public:
	BOOL isDestroy;
	BOOL NowShoot;
};





