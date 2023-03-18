#include "BanDitMonster.h"
#include "GameMainController.h"


BanDitMonster::BanDitMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseMonster(p, hInst, hWnd, pos)
{
	
	BanDitSpeed = 10;
	BanDitHP = 100;
	NowDirection = RIGHT;
	NowState = STATE::IDLE;
	BitNum = 0;
	demage = 20;
	//BanDitBit = ((GameMainController*)maincontroller)->MainBitmaps[GameMainController::BITMAPS::MONSTERBANDIT];
	BanDitBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::MONSTERBANDIT];

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
	weapon = new WeaponBanDit(maincontroller, hInst, hWndMain, enemypos);
	
}


void BanDitMonster::DropItem()//30%확률로 작은 총알상자, 20%확률로 큰 총알상자, 17%확률로 체력상자, 10%확률로 무기상자
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


void BanDitMonster::DrawMonster()
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
	MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, BanDitBit, enemypos.x - (scroll.x), enemypos.y - (scroll.y), 60, 60, 0 + (BitNum * 25), 0 + (((NowState * 2) + NowDirection) * 25), 24, 24);
	weapon->WeaponRender((enemypos.x + WeaponPos[NowDirection].x) - scroll.x, (enemypos.y + WeaponPos[NowDirection].y) - scroll.y, degree);
}

void BanDitMonster::Movingloop()//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
{
	
	POINT despoint;//목적지의 위치(캐릭터의 중앙 위치)
	count++;
	
	if (NowState == STATE::HURT || NowState == STATE::DEAD)
	{
		return;
	}

	if (count >= 40 && NowState != STATE::WALKING)//1.92초에 한번 2ch
	{
		despoint = ((GameMainController*)maincontroller)->GetCharacterPos();
		degree = maincontroller->GetAngle(enemypos, despoint);//현재 나의 위치에서 목적지까지의 각을 알아낸다.
		
		SetState(STATE::WALKING);
	}
	despoint = ((GameMainController*)maincontroller)->GetCharacterPos();
	if (maincontroller->GetPointDistnace(enemypos, despoint) <= intervaltochar * intervaltochar)//몬스터와 캐릭터의 거리가 interval값 이하가 되면 degree값을 -로 바꿔버린다.
	{
		if (degree >= 0)
		{
			degree = (180 - degree) * -1;
		}
		else
		{
			degree = (-180 - degree) * -1;
		}
	}

	if (NowState != STATE::HURT && NowState == STATE::WALKING)
	{
		if (walkcount >= 15 && NowState == STATE::WALKING)
		{
			SetState(STATE::IDLE);
			
			count = 0;
			walkcount = 0;
			return;
		}
		if (MonsterMove(degree, BanDitSpeed) == FALSE)
		{
			if (degree >= 0)
			{
				degree = (180 - degree) * -1;
			}
			else
			{
				degree = (-180 - degree) * -1;
			}
			walkcount--;
		}
		walkcount++;

	}
}

void BanDitMonster::MonsterHit(BaseBullet* p)
{
	if (NowState == STATE::HURT||NowState==STATE::DEAD)
	{
		return;
	}
	BanDitHP = BanDitHP - p->GetDemage();
	SetState(STATE::HURT);
	
	if (BanDitHP <= 0)
	{
		((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BANDITDIE);
		SetState(STATE::DEAD);
	}
}



void BanDitMonster::MotionMovingLoop()
{
	int count = 0;
	switch (NowState)//상황에 따라서 모션의 종류 유지될 시간 일정 시간 뒤에는 무엇을 할지 정해줌
	{
	case IDLE:
		sleeptime = 2;
		break;

	case WALKING:
		sleeptime = 2;
		break;

	case HURT:
		sleeptime = 2;
		MotionCount = 2;
		break;

	case DEAD:
		sleeptime = 2;
		MotionCount = DEADMAX;
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
				SetState(STATE::IDLE);
				
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
	else if (NowState == STATE::WALKING)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % WALKBIT::WALKMAX;
			mcount1 = 0;
		}
	}
	mcount1++;
}



void BanDitMonster::MonsterMove(float angle)//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
{
	POINT temppos;
	RECT playsize;
	POINT scroll = maincontroller->GetScrollPos();
	
	playsize.left = scroll.x;
	playsize.top = scroll.y;
	playsize.right = scroll.x + PlaySizeX * 60;
	playsize.bottom = scroll.y + PlaySizeY * 60;
	if (PtInRect(&playsize, enemypos) == FALSE)//몬스터가 활성영역 밖으로 나가면 움직이지 않고 가만히 있는다.
	{
		return;
	}

	temppos.x = enemypos.x + cos(angle * (3.141592f / 180.0f)) * BanDitSpeed;//호도법을 라디안으로 바꿔서 입력해준다.
	temppos.y = enemypos.y + (sin(angle * (3.141592f / 180.0f)) * BanDitSpeed);
	if (maincontroller->DetectMapCollision(HitBox_Pos(temppos), HitBox_Radious()))
	{
		return;
	}
	enemypos.x = temppos.x;
	enemypos.y = temppos.y;


}

BOOL BanDitMonster::MonsterMove(float angle, int distance)//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
{
	POINT temppos;
	RECT playsize;
	
	POINT scroll = maincontroller->GetScrollPos();

	playsize.left = scroll.x;
	playsize.top = scroll.y;
	playsize.right = scroll.x + PlaySizeX * 60;
	playsize.bottom = scroll.y + PlaySizeY * 60;
	if (PtInRect(&playsize, enemypos) == FALSE)//몬스터가 활성영역 밖으로 나가면 움직이지 않고 가만히 있는다.
	{
		return FALSE;
	}
	temppos.x = enemypos.x + (cos(angle * (3.141592f / 180.0f)) * distance);//호도법을 라디안으로 바꿔서 입력해준다.
	temppos.y = enemypos.y + (sin(angle * (3.141592f / 180.0f)) * distance);
	if (maincontroller->DetectMapCollision(HitBox_Pos(temppos), HitBox_Radious()))
	{
		return FALSE;
	}
	enemypos.x = temppos.x;
	enemypos.y = temppos.y;
	return TRUE;
}



void BanDitMonster::Shootloop()
{
	shootcount++;
	if (NowState == STATE::HURT || NowState == STATE::DEAD)
	{
		return;
	}
	if (shootcount >= 3)
	{
		POINT start = HitBox_Pos();
		POINT dest = ((GameMainController*)maincontroller)->mycharacter->GetHitBoxPos();

		if (DetectRay(start, dest))//1초에 한번씩 총을 쏠 수 있는지 검사하고 쏠 수 있으면 총알을 발사해주고 7초+1초 동안 발사하지 않음
		{
			((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BANDITGUN);
			ShootBullet();
			shootcount = 0;
		}
		
	}

}


void BanDitMonster::ShootBullet()
{
	POINT startpos = enemypos;
	POINT destpos = maincontroller->GetCharacterPos();
	float degree = maincontroller->GetAngle(startpos, destpos);
	startpos.x = startpos.x + WeaponPos[NowDirection].x;
	startpos.y = startpos.y + WeaponPos[NowDirection].y;
	weapon->ShootBullet(startpos.x, startpos.y, degree);
	//GameMainController::ControllerInstance()->bulletlist.push_back();

}


BanDitMonster::~BanDitMonster()
{
	isDestroy = TRUE;
}

int BanDitMonster::HitBox_Radious()
{
	return HitBox_radious;
}

POINT BanDitMonster::HitBox_Pos()
{
	POINT pt;
	pt.x = enemypos.x + 30;
	pt.y = enemypos.y + 30;
	return pt;
}

POINT BanDitMonster::HitBox_Pos(POINT pt)
{
	//POINT pt;
	pt.x = pt.x + 30;
	pt.y = pt.y + 30;
	return pt;
}