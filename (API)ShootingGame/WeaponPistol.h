#pragma once
#include "BaseWeapon.h"
#include "BaseMainController.h"
class WeaponPistol :public BaseWeapon
{
public:
	WeaponPistol(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	virtual void ShootBullet(int posx, int posy, float angle);
	virtual int GetShootSpeed();
	virtual ~WeaponPistol();

private:
	int demage;
	int shootspeed;
};

