#pragma once
#include "BaseWeapon.h"
class WeaponBigBanDit :
    public BaseWeapon
{
public:
	enum PATTERN { PATTERN1, PATTERN2, PATTERN3, PATTERN4, PATTERNMAX };
	WeaponBigBanDit(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	virtual void ShootBullet(int posx, int posy, float angle);
	virtual int GetShootSpeed();
	virtual ~WeaponBigBanDit();
	virtual void WeaponRender(int posx, int posy, float aim);//총이 바라보고 있는 방향

private:
	int demage;
	int shootspeed;


};

