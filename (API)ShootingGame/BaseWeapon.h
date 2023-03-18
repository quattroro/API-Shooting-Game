#pragma once
#include <Windows.h>
#include "BaseMainController.h"
#include "BaseBullet.h"
#include "MyDoubleBuffer.h"
class BaseWeapon
{
public:
	enum WEAPONS { PISTOL, RIFLE, SHOTGUN, ROCKET, BANDITGUN, SCORPIONGUN, BIGBANDITGUN, MAXWEAPON };
	//enum BulletKind { MYNOMAL, MYENERGY = 17, ENOMAL = 42, EENERGY = 59 };
	enum DIRECTION { RIGHT, LEFT };
	BaseWeapon();
	BaseWeapon(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	
	virtual void WeaponRender(int posx, int posy, float aim);//총이 바라보고 있는 방향


	//virtual BaseBullet* ShootBullet(int posx, int posy, float angle) = 0;
	virtual void ShootBullet(int posx, int posy, float angle) = 0;
	virtual void ShotGunShootBullet(int posx, int posy, float angle)
	{

	}
	virtual void SetPos(POINT pos);
	

	virtual POINT GetPos();
	

	virtual int GetKind();


	virtual int GetShootSpeed() = 0;

	virtual ~BaseWeapon();


	virtual void ArmorUp(int num);

	virtual int GetArmor()
	{
		return Armor;
	}

protected:
	POINT pos;
	HBITMAP Weaponsbit;
	HINSTANCE m_hInst;
	HWND hWndMain;
	int kind;
	int bulletkind;
	int Direction;
	int Armor;
	BaseMainController* controller;
};

