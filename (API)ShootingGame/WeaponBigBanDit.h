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
	virtual void WeaponRender(int posx, int posy, float aim);//���� �ٶ󺸰� �ִ� ����

private:
	int demage;
	int shootspeed;


};

