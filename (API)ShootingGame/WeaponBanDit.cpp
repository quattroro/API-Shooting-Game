#include "WeaponBanDit.h"
#include "GameMainController.h"

WeaponBanDit::WeaponBanDit(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseWeapon(p, hInst, hWnd, pos)
{
	kind = WEAPONS::BANDITGUN;
	bulletkind = BaseBullet::ENOMAL;
	shootspeed = 2000;
}

void WeaponBanDit::ShootBullet(int posx, int posy, float angle)
{
	//BaseBullet* temp =;
	((GameMainController*)controller)->bulletlist.push_back(new BaseBullet(hWndMain, posx, posy, angle, bulletkind, FALSE));
	//return temp;
}
int WeaponBanDit::GetShootSpeed()
{
	return shootspeed;
}
WeaponBanDit::~WeaponBanDit()
{

}

