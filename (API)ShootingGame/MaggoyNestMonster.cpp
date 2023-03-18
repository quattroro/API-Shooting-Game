#include "MaggoyNestMonster.h"
#include "MaggotMonster.h"
#include "GameMainController.h"


MaggoyNestMonster::MaggoyNestMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseMonster(p, hInst, hWnd, pos)
{

	BanDitSpeed = 10;
	NestHP = 100;
	NowDirection = RIGHT;
	NowState = STATE::IDLE;
	BitNum = 0;
	spawncount = 400;//20초에 한번씩 에벌레를 3마리씩 스폰한다.
	count = 400;
	//NestBit = ((GameMainController*)maincontroller)->MainBitmaps[GameMainController::BITMAPS::MONSTERMAGGOTNEST];
	NestBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::MONSTERMAGGOTNEST];

	movingdegree[0] = 20;
	movingdegree[1] = 160;
	movingdegree[2] = -160;
	movingdegree[3] = -20;
	intervaltochar = 200;
	HitBox_radious = 20;
	WeaponPos[RIGHT].x = 15;
	WeaponPos[RIGHT].y = 2;
	WeaponPos[LEFT].x = -20;
	WeaponPos[LEFT].y = 2;
	//weapon = new WeaponBanDit(hInst, hWndMain, enemypos);

}

void MaggoyNestMonster::DrawMonster()
{
	

	POINT scroll = maincontroller->GetScrollPos();
	POINT startpos = enemypos;
	POINT destpos = maincontroller->GetCharacterPos();
	float degree = maincontroller->GetAngle(startpos, destpos);
	if (degree >= -90 && degree <= 90)
	{
		NowDirection = DIRECTION::RIGHT;
	}
	else
	{
		NowDirection = DIRECTION::LEFT;
	}
	//MyDoubleBuffer::GetBufferInstance()->DrawCircle(hWndMain, (enemypos.x - (scroll.x)) + 30, (enemypos.y - (scroll.y)) + 30, 20);
	MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, NestBit, enemypos.x - (scroll.x), enemypos.y - (scroll.y), 60, 60, 0 + (BitNum * 33), 0 + (/*((NowState * 2) + NowDirection)*/NowState * 33), 32, 32);
	//weapon->WeaponRender((enemypos.x + WeaponPos[NowDirection].x) - scroll.x, (enemypos.y + WeaponPos[NowDirection].y) - scroll.y, degree);
}

void MaggoyNestMonster::DropItem()//30%확률로 작은 총알상자, 20%확률로 큰 총알상자, 17%확률로 체력상자, 10%확률로 무기상자
{
	int num;
	num = rand() % 101;
	if (num <= 30)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain,  enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::SMALLARMOR));
	}
	else if (num <= 50)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain,  enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::BIGARMOR));
	}
	else if (num <= 67)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain,  enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::HURT));
	}
	else if (num <= 77)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain,  enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::WEAPON));
	}

}

void MaggoyNestMonster::Movingloop()//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
{
	
	POINT pt;
	count++;
	if (count >= spawncount)
	{
		pt.x = enemypos.x - 40;
		pt.y = enemypos.y + 30;
		for (int i = 0; i < 3; i++)
		{
			((GameMainController*)maincontroller)->monsterlist.push_back(new MaggotMonster(maincontroller, m_hInst, hWndMain, pt));
			pt.x = pt.x + 40;
		}
		count = 0;
	}

}

void MaggoyNestMonster::MonsterHit(BaseBullet* p)
{
	if (NowState == STATE::HURT||NowState==STATE::DEAD)
	{
		return;
	}
	NestHP = NestHP - p->GetDemage();
	SetState(HURT);

	if (NestHP <= 0)
	{
		((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::MAGGOTNESTDIE);
		NowState = STATE::DEAD;
	}
}

void MaggoyNestMonster::MotionMovingLoop()
{
	
	switch (NowState)//상황에 따라서 모션의 종류 유지될 시간 일정 시간 뒤에는 무엇을 할지 정해줌
	{
	case IDLE:
		sleeptime = 2;
		break;

	case HURT:
		sleeptime = 2;
		MotionCount = 2;
		break;

	case DEAD:
		sleeptime = 2;
		MotionCount = DEADBIT::DEADMAX;
		break;
	}

	if (NowState == STATE::HURT)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % HURTBIT::HURTMAX;
			mcount2++;
			if (mcount2 >= MotionCount)
			{
				SetState(IDLE);
				
				mcount2 = 0;
			}
			mcount1 = 0;
		}
		
	}
	else if (NowState == STATE::DEAD)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % DEADBIT::DEADMAX;
			mcount2++;
			if (mcount2 >= MotionCount)
			{
				DropItem();
				mcount2 = 0;
				isDestroy = TRUE;
			}
			mcount1 = 0;
		}
		

	}
	else if (NowState == STATE::IDLE)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % IDLEBIT::IDLEMAX;
			mcount1 = 0;
		}
		
	}
	mcount1++;
}




MaggoyNestMonster::~MaggoyNestMonster()
{
	isDestroy = TRUE;
}

int MaggoyNestMonster::HitBox_Radious()
{
	return HitBox_radious;
}

POINT MaggoyNestMonster::HitBox_Pos()
{
	POINT pt;
	pt.x = enemypos.x + 30;
	pt.y = enemypos.y + 30;
	return pt;
}

POINT MaggoyNestMonster::HitBox_Pos(POINT pt)
{
	pt.x = pt.x + 30;
	pt.y = pt.y + 30;
	return pt;
}