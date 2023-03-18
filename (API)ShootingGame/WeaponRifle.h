#pragma once
#include "BaseWeapon.h"
#include "BaseMainController.h"
class WeaponRifle :public BaseWeapon
{
public:
	WeaponRifle(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	virtual void ShootBullet(int posx, int posy, float angle);
	virtual int GetShootSpeed();
	virtual ~WeaponRifle();

private:
	int demage;
	int shootspeed;
};

