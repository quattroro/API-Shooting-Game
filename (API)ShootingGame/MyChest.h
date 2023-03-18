#pragma once
#include "MyDoubleBuffer.h"
#include <Windows.h>
class MyChest
{
public:
	enum CHESTKIND { SMALLARMOR, BIGARMOR, HURT, WEAPON };
	enum CHESTSTATE { NOMAL, OPENED };
	MyChest(HWND hWnd, int posx, int posy, int kind)
	{
		hWndMain = hWnd;
		pos.x = posx;
		pos.y = posy;
		this->kind = kind;
		state = NOMAL;
		ChestBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::CHESTS];
		switch (kind)
		{
		case SMALLARMOR:
		case BIGARMOR:
		case HURT:
			BitSize.x = 16;
			BitSize.y = 15;
			break;

		case WEAPON:
			BitSize.x = 24;
			BitSize.y = 15;
			break;
		}
	}

	void ChestRender(int posx,int posy)
	{
		//MyDoubleBuffer::GetBufferInstance()->DrawCircle(hWndMain, posx + 20, posy + 20, 20);
		MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, ChestBit, posx, posy, 40, 40, (state * (BitSize.x + 1)), kind * (BitSize.y + 1), BitSize.x, BitSize.y);
		

	}

	POINT GetChestPos()
	{
		return pos;
	}

	POINT GetHitBox_Pos()
	{
		POINT pt;
		pt.x = pos.x + 20;
		pt.y = pos.y + 20;
		return pt;
	}

	int GetHitBox_Radious()
	{
		return 20;
	}

	int GetKind()
	{
		return kind;
	}


private:
	POINT pos;
	HWND hWndMain;
	int HitBox_Radious;
	int kind;
	HBITMAP ChestBit;
	POINT BitSize;
	int state;
};

