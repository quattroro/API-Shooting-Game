#include "BaseBullet.h"
#include "GameMainController.h"

//enum BulletKind { MYNOMAL, MYENERGY = 17, ENOMAL = 42, BOSS1 = 42, BOSS2 = 42, EENERGY = 59 };//각 총알의 비트맵의 시작위치 y값
BaseBullet::BaseBullet(HWND hWnd, int posx, int posy, float angle, int kind, BOOL ismine)
{
	
	hWndMain = hWnd;
	//m_hInst = hInst;
	pos.x = posx;
	pos.y = posy;
	this->angle = angle;
	this->kind = kind;

	IsDestroy = FALSE;
	BitSize = 16;
	
	mybullet = ismine;
	
	bulletsBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::BULLETS];
	switch (kind)
	{
	case MYNOMAL:
		BulletSpeed = 15;
		demage = 30;
		bitpos = 0;
		break;

	case MYENERGY:
		BulletSpeed = 12;
		demage = 80;
		bitpos = 17;
		break;

	case ENOMAL:
	case BOSS1:
	case BOSS2:
		BulletSpeed = 15;
		demage = 20;
		bitpos = 42;
		break;

	case EENERGY:
		BulletSpeed = 12;
		demage = 80;
		bitpos = 59;
		break;
	}
}
BaseBullet::~BaseBullet()
{

}
void BaseBullet::BulletMove(POINT pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}
void BaseBullet::BulletRender(int scrollx, int scrolly)
{
	POINT pt;
	/*pt.x = pos.x - (scrollx * 60);
	pt.y = pos.y - (scrolly * 60);*/
	pt.x = pos.x - (scrollx);
	pt.y = pos.y - (scrolly);
	//pt.x=pos.x
	MotionMovingLoop();
	switch (kind)
	{
	
	case MYENERGY:
	case EENERGY:
		if (BitNum >= 2)
		{
			BitSize = 24;
		}
		break;
	}
	MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, MyDoubleBuffer::GetBufferInstance()->GetRotatedBitmap(hWndMain, bulletsBit, BitNum * (BitSize + 1), bitpos, BitSize, BitSize, angle), pt.x, pt.y, 40, 40, 0, 0, BitSize, BitSize);
}

void BaseBullet::MotionMovingLoop()//총알이 한번 그려질 때마다 카운트를 해서 그에따라 총알의 상태를 변화해준다.
{
	switch (kind)//상황에 따라서 모션의 종류 유지될 시간 일정 시간 뒤에는 무엇을 할지 정해줌
	{
	case MYNOMAL:
		sleeptime = 2;
		MotionCount = MNOMALMAX;
		break;

	case MYENERGY:
		sleeptime = 4;
		break;

	case ENOMAL:
	case BOSS1:
	case BOSS2:
		sleeptime = 3;
		MotionCount = 2;
		MotionCount = ENOMALMAX;
		break;


	case EENERGY:
		break;
	}

	if (kind == MYNOMAL)
	{
		if (mcount1 >= sleeptime)
		{
			if (mcount2 >= MotionCount-1)
			{
				return;
				
			}
			BitNum = (BitNum + 1) % MNOMALMAX;
			mcount2++;
			mcount1 = 0;
		}
	}
	else if (kind == MYENERGY)
	{
		if (mcount1 >= sleeptime)
		{
			
			BitNum = (BitNum + 1) % 2;
			
		}

	}
	else if (kind == ENOMAL)
	{
		if (mcount1 >= sleeptime)
		{
			if (mcount2 >= MotionCount-1)
			{
				return;
				
			}
			BitNum = (BitNum + 1) % ENOMALMAX;
			mcount2++;
			mcount1 = 0;
		}
	}

	mcount1++;
}



POINT BaseBullet::GetPos()
{
	return pos;
}
BOOL BaseBullet::IsMyBullet()
{
	return mybullet;
}
int BaseBullet::GetDemage()
{
	return demage;
}
int BaseBullet::GetBulletSpeed()
{
	return BulletSpeed;
}
float BaseBullet::GetBulletAngle()
{
	return angle;
}
POINT BaseBullet::GetHitBox_Pos()
{
	POINT pt;
	pt.x = pos.x + 20;
	pt.y = pos.y + 20;
	return pt;
}
POINT BaseBullet::GetHitBox_Pos(POINT pt)
{
	pt.x = pt.x + 20;
	pt.y = pt.y + 20;
	return pt;
}
int BaseBullet::GetHitBox_Radious()
{
	return HitBox_radious;
}

int BaseBullet::GetKind()
{
	return kind;
}