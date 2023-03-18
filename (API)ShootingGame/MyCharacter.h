#pragma once
#include <Windows.h>
#include "BaseWeapon.h"
#include "WeaponPistol.h"
#include "WeaponBanDit.h"
#include "BaseMainController.h"
#include "BaseBullet.h"
#include "MyChest.h"
#include "WeaponRocket.h"
#include "WeaponRifle.h"
#include "WeaponShotGun.h"


#include <time.h>
#include <map>
using namespace std;
//#include "GameMainController.h"

//class GameMainController;
#define MapTileSize 32
#define MaxStage 6
#define MapsizeX 40
#define MapsizeY 20
#define PlaySizeX 18
#define PlaySizeY 11

class MyCharacter
{
public:
	enum CHARECTERS { FISH, CRYSTAL, CMAX };
	enum STATE { IDLE, WALKING, HURT, DEAD, DASH, DEFENSE = DASH };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//위와 아래방향의 비트맵은 없기 때문에 두 방향으로만 확인
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };
	enum CTHREAD { MOVING, SHOOTING, CTHREADMAX };
	MyCharacter()
	{
		hWndMain = NULL;
		this->m_hInst = NULL;
		mevespeed = 10;
	}
	MyCharacter(BaseMainController* controller, HWND hWnd,HINSTANCE M_hInst,POINT pos)
	{
		for (int i = 0; i < CTHREAD::CTHREADMAX; i++)
		{
			hThread[i] = NULL;
		}
		IsDestroy = FALSE;
		HP = 4;
		maincontroller = controller;
		hWndMain = hWnd;
		this->m_hInst = M_hInst;
		mevespeed = 10;
		this->pos = pos;
		POINT pt;
		pt = pos;
		pt.x + 10;
		pt.y + 10;
		NowShoot = FALSE;
		weaponpos[RIGHT].x = 15;
		weaponpos[RIGHT].y = 2;
		weaponpos[LEFT].x = -20;
		weaponpos[LEFT].y = 2;
		weaponlist.insert(make_pair(new WeaponPistol(maincontroller, m_hInst, hWndMain, pos), TRUE));//현재 장착하고 있는 무기를 TRUE 장착하지 않은 무기는 FALSE로 한다.
		//weaponlist.insert(make_pair(new WeaponRocket(m_hInst, hWndMain, pos), TRUE));
	}
	virtual BOOL CharacterCollotion() = 0;
	virtual void SetWeapon(int num) = 0;
	//virtual BaseBullet* ShootBullet(float angle) = 0;
	virtual void ShootBullet(float angle) = 0;
	virtual void UpMove()
	{
	}

	virtual void DownMove()
	{
	
	}

	virtual void RightMove()
	{

	}

	virtual void LeftMove()
	{

	}

	virtual void SetState(int state)
	{
	}
	virtual void SetDirection(int direction)
	{
	}

	virtual void SetBitNum(int bitnum)
	{
	}
	virtual int GetDirection() = 0;
	virtual int GetState() = 0;
	virtual POINT GetPos()
	{
		return pos;
	}
	
	virtual int GetRadious() = 0;
	virtual void SetPos(POINT pos)
	{
		this->pos = pos;
		for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
		{
			(*itr).first->SetPos(pos);
		}
	}

	virtual void DrawCharacter(int posx, int posy) = 0;


	virtual ~MyCharacter()
	{

	}

	virtual void CharacterHit(/*int demage*/) = 0;
	virtual POINT GetBulletPos() = 0;
	virtual POINT GetHitBoxPos()
	{
		POINT pt;
		pt.x = pos.x + 30;
		pt.y = pos.y + 30;
		return pt;
	}
	virtual POINT GetHitBoxPos(POINT pt)
	{
		//POINT pt;
		pt.x = pos.x + 30;
		pt.y = pos.y + 30;
		return pt;
	}
	virtual int GetHitBoxRadious()
	{
		return HitBox_radious;
	}
	virtual BOOL NowShootable()//쏠 수 있으면 true 쏠 수 없으면 false를 리턴함
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

	virtual void GetItem(int kind)//캐릭터와 상자가 충돌했을때 충돌한 상자의 종류를 받아와서 종류에 따라 동작을 해준다.
	{
		srand((unsigned)time(NULL));
		switch (kind)
		{
		case MyChest::CHESTKIND::SMALLARMOR:
			for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
			{
				if ((*itr).second == TRUE)
				{
					return (*itr).first->ArmorUp(50);
				}
			}
			break;

		case MyChest::CHESTKIND::BIGARMOR:
			for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
			{
				(*itr).first->ArmorUp(50);
			}
			break;

		case MyChest::CHESTKIND::HURT:
			HP++;
			break;

		case MyChest::CHESTKIND::WEAPON:
			int weapon = rand() % 4;
			BaseWeapon* temp;
			switch (weapon)
			{
			case BaseWeapon::WEAPONS::PISTOL:
				temp = new WeaponPistol(maincontroller, m_hInst, hWndMain, pos);

				break;
			case BaseWeapon::WEAPONS::RIFLE:
				temp = new WeaponRifle(maincontroller, m_hInst, hWndMain, pos);
				break;
			case BaseWeapon::WEAPONS::ROCKET:
				temp = new WeaponRocket(maincontroller, m_hInst, hWndMain, pos);
				break;

			case BaseWeapon::WEAPONS::SHOTGUN:
				temp = new WeaponShotGun(maincontroller, m_hInst, hWndMain, pos);
				break;
			}
			if (weaponlist.size() == 1)
			{
				weaponlist.insert(make_pair(temp, false));
				SetWeapon(2);
			}
			else if (weaponlist.size() == 2)
			{
				for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
				{
					if ((*itr).second == TRUE)
					{
						weaponlist.erase(itr);
						weaponlist.insert(make_pair(temp, TRUE));
						break;
					}
				}
			}
			break;
		}
	}

	virtual void Defense()
	{

	}

	virtual int GetArmor()
	{
		for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
		{
			if ((*itr).second == TRUE)
			{
				return (*itr).first->GetArmor();
			}
		}
	}

	virtual int GetHP()
	{
		return HP;
	}


	virtual int GetSpeed()
	{
		return mevespeed;
	}
	virtual int GetKind()
	{
		return Kind;
	}

	int GetNowWeapon()
	{
		for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
		{
			if ((*itr).second == TRUE)
			{
				return (*itr).first->GetKind();
			}
		}
	}

protected:

	int Kind;
	int HP;
	POINT pos;
	HWND hWndMain;
	HINSTANCE m_hInst;
	int mevespeed;
	int BitNum;
	//int WalkingNum;
	int NowState;
	int NowDirection;
	//BaseWeapon* Weapon1;
	
	BaseMainController* maincontroller;
	POINT weaponpos[DIRECTIONMAX];
	//POINT HitBox_pos;
	int HitBox_radious;
	
	BOOL NowShoot;
	HANDLE hThread[CTHREAD::CTHREADMAX];

public:
	multimap<BaseWeapon*, BOOL> weaponlist;
	BOOL IsDestroy;

};

