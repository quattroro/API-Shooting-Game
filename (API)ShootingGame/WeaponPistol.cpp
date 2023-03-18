#include "WeaponPistol.h"
#include "GameMainController.h"
WeaponPistol::WeaponPistol(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseWeapon(p, hInst, hWnd, pos)
{
	kind = WEAPONS::PISTOL;
	bulletkind = BaseBullet::MYNOMAL;
	shootspeed = 500;
	Armor = 100;
}
void WeaponPistol::ShootBullet(int posx, int posy, float angle)
{
	//BaseBullet* temp = ;
	((GameMainController*)controller)->bulletlist.push_back(new BaseBullet(hWndMain, posx, posy, angle, bulletkind, true));
	Armor--;
	//return temp;
}
int WeaponPistol::GetShootSpeed()
{
	return shootspeed;
}
WeaponPistol::~WeaponPistol()
{

}