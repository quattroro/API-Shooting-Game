#pragma once
#include "BaseWeapon.h"
//#include "GameMainController.h"
class WeaponShotGun :
    public BaseWeapon
{
public:
	WeaponShotGun(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	virtual void ShootBullet(int posx, int posy, float angle);
	//virtual void ShotGunShootBullet(int posx, int posy, float angle);


	virtual int GetShootSpeed();
	virtual ~WeaponShotGun();

private:
	int demage;
	int shootspeed;
};

