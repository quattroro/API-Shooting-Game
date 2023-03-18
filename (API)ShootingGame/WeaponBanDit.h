#pragma once
#include "BaseWeapon.h"
#include "BaseMainController.h"
class WeaponBanDit :
    public BaseWeapon
{
public:
	WeaponBanDit(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	virtual void ShootBullet(int posx, int posy, float angle);
	virtual int GetShootSpeed();
	virtual ~WeaponBanDit();


private:
	int demage;
	int shootspeed;


};

