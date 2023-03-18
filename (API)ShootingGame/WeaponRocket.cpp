#include "WeaponRocket.h"
#include "GameMainController.h"
WeaponRocket::WeaponRocket(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseWeapon(p, hInst, hWnd, pos)
{
	kind = WEAPONS::ROCKET;
	bulletkind = BaseBullet::MYENERGY;
	shootspeed = 1000;
	Armor = 20;
}
void WeaponRocket::ShootBullet(int posx, int posy, float angle)
{
	//BaseBullet* temp = new BaseBullet(hWndMain, posx, posy, angle, bulletkind, true);
	((GameMainController*)controller)->bulletlist.push_back(new BaseBullet(hWndMain, posx, posy, angle, bulletkind, true));
	Armor--;
	//return temp;
}
int WeaponRocket::GetShootSpeed()
{
	return shootspeed;
}
WeaponRocket::~WeaponRocket()
{

}