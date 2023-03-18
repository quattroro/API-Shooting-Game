#include "WeaponRifle.h"
#include "GameMainController.h"
WeaponRifle::WeaponRifle(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseWeapon(p, hInst, hWnd, pos)
{
	kind = WEAPONS::RIFLE;
	bulletkind = BaseBullet::MYNOMAL;
	shootspeed = 200;
	Armor = 120;
}
void WeaponRifle::ShootBullet(int posx, int posy, float angle)
{
	//BaseBullet* temp = ;
	((GameMainController*)controller)->bulletlist.push_back(new BaseBullet(hWndMain, posx, posy, angle, bulletkind, true));
	Armor--;
	//return temp;
}
int WeaponRifle::GetShootSpeed()
{
	return shootspeed;
}
WeaponRifle::~WeaponRifle()
{

}