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
	for (int i = 0; i < 3; i++)//���Ϳ� ĳ������ ������ �߽����� +20��,-20�� �� 40���� ������ 3���� �ѹ��� �߻��Ѵ�.
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