#include "WeaponShotGun.h"
#include "GameMainController.h"
WeaponShotGun::WeaponShotGun(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseWeapon(p, hInst, hWnd, pos)
{
	kind = WEAPONS::SHOTGUN;
	bulletkind = BaseBullet::MYNOMAL;
	shootspeed = 1000;
	Armor = 20;
}
void WeaponShotGun::ShootBullet(int posx, int posy, float angle)
{
	angle = angle - 8;
	for (int i = 0; i < 3; i++)//몬스터와 캐릭터의 각도를 중심으로 +20도,-20도 총 40도의 각도에 3발을 한번에 발사한다.
	{
		angle = angle + (i * 8);
		((GameMainController*)controller)->bulletlist.push_back(new BaseBullet(hWndMain, posx, posy, angle, bulletkind, true));
		Armor--;
	}

}
int WeaponShotGun::GetShootSpeed()
{
	return shootspeed;
}
WeaponShotGun::~WeaponShotGun()
{

}