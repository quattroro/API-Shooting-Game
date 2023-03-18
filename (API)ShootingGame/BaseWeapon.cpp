#include "BaseWeapon.h"
#include "GameMainController.h"

BaseWeapon::BaseWeapon()
{

}
BaseWeapon::BaseWeapon(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos)
{
	controller = p;
	m_hInst = hInst;
	hWndMain = hWnd;
	this->pos = pos;
	//Weaponsbit = GameMainController::ControllerInstance()->MainBitmaps[GameMainController::BITMAPS::GUNS];
	Weaponsbit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::GUNS];
}
void BaseWeapon::WeaponRender(int posx, int posy, float aim)//총이 바라보고 있는 방향
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
		MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, MyDoubleBuffer::GetBufferInstance()->GetRotatedBitmap(hWndMain, Weaponsbit, 0 + (kind * 23), Direction * 24, 22, 23, aim), posx, posy, 60, 60, 0 /*+ (kind * 23)*/, 0, 22, 23);
		//MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, Weaponsbit, posx, posy, 60, 60, 0 + (kind * 23), Direction * 24, 22, 23);
	}

	
}
BaseWeapon::~BaseWeapon()
{

}
void BaseWeapon::ArmorUp(int num)
{
	Armor = Armor + num;
}

void BaseWeapon::SetPos(POINT pos)
{
	this->pos = pos;
}

POINT BaseWeapon::GetPos()
{
	return pos;
}

int BaseWeapon::GetKind()
{
	return kind;
}