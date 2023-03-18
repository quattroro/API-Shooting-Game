#include "WeaponBigBanDit.h"
#include "GameMainController.h"

WeaponBigBanDit::WeaponBigBanDit(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseWeapon(p, hInst, hWnd, pos)
{
	kind = WEAPONS::BIGBANDITGUN;
	bulletkind = BaseBullet::ENOMAL;
	shootspeed = 2000;
}
void WeaponBigBanDit::ShootBullet(int posx, int posy, float angle)
{
	//BaseBullet* temp = new BaseBullet(hWndMain, posx, posy, angle, bulletkind, FALSE);
	((GameMainController*)controller)->bulletlist.push_back(new BaseBullet(hWndMain, posx, posy, angle, bulletkind, FALSE));
	//return temp;
}
int WeaponBigBanDit::GetShootSpeed()
{
	return shootspeed;
}
WeaponBigBanDit::~WeaponBigBanDit()
{

}

void WeaponBigBanDit::WeaponRender(int posx, int posy, float aim)//총이 바라보고 있는 방향
{
	if (Direction != DIRECTION::RIGHT)
	{
		Direction = DIRECTION::RIGHT;
	}
	if (aim >= 0)
	{
		if (aim > 90)
		{
			if (Direction != DIRECTION::LEFT)
			{
				Direction = DIRECTION::LEFT;
			}
			aim = (180 - aim) * -1;//방향이 왼쪽으로 바꼈을때 각도도 왼쪽에서의 각도로 바꿔준다.
		}
	}
	else if (aim < 0)
	{
		if (aim < -90)
		{
			if (Direction != DIRECTION::LEFT)
			{
				Direction = DIRECTION::LEFT;
			}
			aim = (-180 - aim) * -1;
		}
	}
	if (aim == 0)
	{
		MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, Weaponsbit, posx, posy, 40, 40, 0 + (kind * 23), 0, 22, 23);
	}
	else
	{
		MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, MyDoubleBuffer::GetBufferInstance()->GetRotatedBitmap(hWndMain, Weaponsbit, 117, Direction * 24, 32, 23, aim), posx, posy, 60, 60, 0 /*+ (kind * 23)*/, 0, 32, 23);

	}
}