#include "BigBanDitMonster.h"
#include "GameMainController.h"
#include "WeaponBigBanDit.h"

// IDLE, WALKING = 58, SHOOTING = 118, DASH = 172, DASHSTOP = 226, HURT = 284, DEAD = 344, STATEMAX = 7

BigBanDitMonster::BigBanDitMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseMonster(p, hInst, hWnd, pos)
{

	BigBanDitSpeed = 10;
	BigBanDitHP = 1000;
	NowDirection = RIGHT;
	NowState = STATE::IDLE;
	BitNum = 0;
	demage = 20;
	BigBanDitBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::MONSTERBIGBANDIT];

	movingdegree[0] = 20;
	movingdegree[1] = 160;
	movingdegree[2] = -160;
	movingdegree[3] = -20;
	intervaltochar = 200;
	HitBox_radious = 40;
	WeaponPos[RIGHT].x = 15;
	WeaponPos[RIGHT].y = 2;
	WeaponPos[LEFT].x = -20;
	WeaponPos[LEFT].y = 2;



	//동작별로 비트맵의 사이즈가 다르기 때문데 사이즈를 저장해 준다.
	BitSize[IDLE].x = 26;
	BitSize[IDLE].y = 28;
	BitSize[WALKING].x = 26;
	BitSize[WALKING].y = 29;
	BitSize[SHOOTING].x = 24;
	BitSize[SHOOTING].y = 26;
	BitSize[DASH].x = 31;
	BitSize[DASH].y = 26;
	BitSize[DASHSTOP].x = 25;
	BitSize[DASHSTOP].y = 28;
	BitSize[HURT].x = 27;
	BitSize[HURT].y = 29;
	BitSize[DEAD].x = 31;
	BitSize[DEAD].y = 29;

	//동작별로 비트맵의 시작 y 값을 넣어준다.
	BitPos[IDLE] = 0;
	BitPos[WALKING] = 58;
	BitPos[SHOOTING] = 118;
	BitPos[DASH] = 172;
	BitPos[DASHSTOP] = 226;
	BitPos[HURT] = 284;
	BitPos[DEAD] = 344;

	

	weapon = new WeaponBigBanDit(maincontroller, hInst, hWndMain, enemypos);

}

void BigBanDitMonster::DrawMonster()
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
	MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, BigBanDitBit, enemypos.x - (scroll.x), enemypos.y - (scroll.y), 60, 60, 0 + (BitNum * (BitSize[NowState].x + 1)), 0 + (BitPos[NowState] + (NowDirection * (BitSize[NowState].y + 1))), BitSize[NowState].x, BitSize[NowState].y);
	weapon->WeaponRender((enemypos.x + WeaponPos[NowDirection].x) - scroll.x, (enemypos.y + WeaponPos[NowDirection].y) - scroll.y, degree);
}

void BigBanDitMonster::DropItem()//30%확률로 작은 총알상자, 20%확률로 큰 총알상자, 17%확률로 체력상자, 10%확률로 무기상자
{
	int num;
	num = rand() % 101;
	if (num <= 30)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain, enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::SMALLARMOR));
	}
	else if (num <= 50)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain, enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::BIGARMOR));
	}
	else if (num <= 67)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain, enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::HURT));
	}
	else if (num <= 77)
	{
		((GameMainController*)maincontroller)->chestlist.push_back(new MyChest(hWndMain, enemypos.x + 20, enemypos.y + 40, MyChest::CHESTKIND::WEAPON));
	}

}




void BigBanDitMonster::Movingloop()//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
{
	srand((unsigned)time(NULL));

	count++;
	if (NowState == STATE::SHOOTING || NowState == STATE::DASH || NowState == STATE::DASHSTOP || NowState == STATE::HURT || NowState == STATE::DEAD)
	{
		return;
	}

	if (count >= /*120*/20 && NowState != STATE::WALKING)//1초에 한번
	{
		angle = 45 + ((rand() % 8) * 45);//360도를 8로 나눠서 8개의 방향에서 랜덤으로 움직일 곳을 뽑는다.
		SetState(WALKING);
	}
	

	if (NowState != STATE::HURT && NowState == STATE::WALKING)
	{
		if (walkcount >= 25 && NowState == STATE::WALKING)
		{
			SetState(IDLE);
			
			count = 0;
			walkcount = 0;
			return;
		}
		if (MonsterMove(angle, BigBanDitSpeed) == FALSE)
		{
			angle = 45 + ((rand() % 8) * 45);//360도를 8로 나눠서 8개의 방향에서 랜덤으로 움직일 곳을 뽑는다.
			walkcount--;
		}
		walkcount++;
	}

}





void BigBanDitMonster::MonsterHit(BaseBullet* p)
{
	if (NowState == STATE::HURT || NowState == STATE::DEAD)
	{
		return;
	}
	BigBanDitHP = BigBanDitHP - p->GetDemage();
	SetState(HURT);
	
	if (BigBanDitHP <= 0)
	{
		((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BANDITDIE);
		//DropItem();
		SetState(DEAD);
	}
}



void BigBanDitMonster::MotionMovingLoop()
{
	switch (NowState)//상황에 따라서 모션의 종류 유지될 시간 일정 시간 뒤에는 무엇을 할지 정해줌
	{
	case IDLE:
		sleeptime = 2;
		break;

	case WALKING:
		sleeptime = 2;
		break;

	case HURT:
		sleeptime = 3;
		MotionCount = 2;
		break;

	case DEAD:
		sleeptime = 2;//0.2초에 한번씩
		MotionCount = DEADMAX;
		break;

	case DASH:
		sleeptime = 2;
		break;

	case DASHSTOP:
		sleeptime = 4;
		MotionCount = DASHSTOPMAX;
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
	else if (NowState == STATE::WALKING)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % WALKBIT::WALKMAX;
			mcount1 = 0;
		}
	}
	else if (NowState == STATE::DASH)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % DASHBIT::DASHMAX;
			mcount1 = 0;
		}
	}
	else if (NowState == STATE::DASHSTOP)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % DASHSTOPBIT::DASHSTOPMAX;
			mcount2++;
			if (mcount2 >= MotionCount)
			{
				SetState(IDLE);
				
				mcount2 = 0;
			}
			mcount1 = 0;
		}
	}
	mcount1++;
}



BOOL BigBanDitMonster::MonsterMove(float angle, int distance)//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
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

void BigBanDitMonster::Shootloop()
{
	shootcount++;
	if (NowState == STATE::DEAD || NowState == STATE::HURT)
	{
		return;
	}

	if (shootcount >= 5)
	{
		int pattern = rand() % PATTERNMAX;
		if (pattern == PATTERN1)
		{
			MonsterDash();
			return;
		}
		//((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BIGBANDITGUN);
		ShootBullet(pattern);
	}
}


void BigBanDitMonster::MonsterDash()
{

	POINT startpos = enemypos;
	POINT destpos = maincontroller->GetCharacterPos();
	float degree = maincontroller->GetAngle(startpos, destpos);
	int dashcount = 0;
	while (dashcount <= 30)
	{
		SetState(DASH);
		if (MonsterMove(degree, BigBanDitSpeed + 10) == FALSE)
		{
			degree = degree * -1;
		}
		dashcount++;
		Sleep(16);
	}
	SetState(DASHSTOP);
	BitNum = 0;
	return;
}



void BigBanDitMonster::ShootBullet(int pattern)
{
	SetState(SHOOTING);
	POINT startpos = enemypos;
	POINT destpos = maincontroller->GetCharacterPos();
	float degree = maincontroller->GetAngle(startpos, destpos);
	startpos.x = startpos.x + WeaponPos[NowDirection].x;
	startpos.y = startpos.y + WeaponPos[NowDirection].y;
	int count = 0;
	float angle = 0;


	if (pattern == PATTERN2)//쏘고서 어딘가에 부딫히면 원형으로 퍼져나간다.
	{
		GameMainController::ControllerInstance()->bulletlist.push_back(new BaseBullet(hWndMain, startpos.x, startpos.y, degree, BaseBullet::BOSS1, FALSE));
		((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BIGBANDITGUN);
		SetState(IDLE);
	}
	else if (pattern == PATTERN3)//플레이어를 향해 10도 명중률로 난사
	{
		while (count <= 15)
		{
			startpos = enemypos;
			destpos = maincontroller->GetCharacterPos();
			degree = maincontroller->GetAngle(startpos, destpos);
			startpos.x = startpos.x + WeaponPos[NowDirection].x;
			startpos.y = startpos.y + WeaponPos[NowDirection].y;
			GameMainController::ControllerInstance()->bulletlist.push_back(new BaseBullet(hWndMain, startpos.x, startpos.y, degree, BaseBullet::ENOMAL, FALSE));
			((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BIGBANDITGUN);
			count++;
			Sleep(100);
		}
		SetState(IDLE);
		
	}
	else if (pattern == PATTERN4)//보스를 중심으로 원형으로 퍼져나가는 총알
	{
		while (angle <= 360)
		{
			angle += 20.0f;
			startpos.x = enemypos.x + cos((float)3.141592 * angle / 180) * 20;
			startpos.y = enemypos.y - sin((float)3.141592 * angle / 180) * 20;
			GameMainController::ControllerInstance()->bulletlist.push_back(new BaseBullet(hWndMain, startpos.x, startpos.y, angle, BaseBullet::ENOMAL, FALSE));
			//((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::BIGBANDITGUN);
		}
		SetState(IDLE);
	}


}



BigBanDitMonster::~BigBanDitMonster()
{ 

}

int BigBanDitMonster::HitBox_Radious()
{
	return HitBox_radious;
}

POINT BigBanDitMonster::HitBox_Pos()
{
	POINT pt;
	pt.x = enemypos.x + 30;
	pt.y = enemypos.y + 30;
	return pt;
}

POINT BigBanDitMonster::HitBox_Pos(POINT pt)
{
	//POINT pt;
	pt.x = pt.x + 30;
	pt.y = pt.y + 30;
	return pt;
}