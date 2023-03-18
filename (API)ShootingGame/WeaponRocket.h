#pragma once
#include "BaseWeapon.h"
class WeaponRocket :
    public BaseWeapon
{
public:
	WeaponRocket(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	virtual void ShootBullet(int posx, int posy, float angle);
	virtual int GetShootSpeed();
	virtual ~WeaponRocket();

private:
	int demage;
	int shootspeed;
};

