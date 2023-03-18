#include "ScorpionMonster.h"
#include "GameMainController.h"


ScorpionMonster::ScorpionMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseMonster(p, hInst, hWnd, pos)
{

	ScorpionSpeed = 10;
	ScorpionHP = 100;
	NowDirection = RIGHT;
	NowState = STATE::IDLE;
	BitNum = 0;
	demage = 20;
	
	ScorpionBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::MONSTERSCORPION];
	
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
	weapon = new WeaponBanDit(maincontroller, hInst, hWndMain, enemypos);
	
}

void ScorpionMonster::DrawMonster()
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
	//MyDoubleBuffer::GetBufferInstance()->DrawCircle(hWndMain, (enemypos.x - (scroll.x)) + 60, (enemypos.y - (scroll.y)) + 60, 40);
	MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, ScorpionBit, enemypos.x - (scroll.x), enemypos.y - (scroll.y), 120, 120, 0 + (BitNum * 49), 0 + (((NowState * 2) + NowDirection) * 49), 48, 48);
	
}


void ScorpionMonster::DropItem()//30%확률로 작은 총알상자, 20%확률로 큰 총알상자, 17%확률로 체력상자, 10%확률로 무기상자
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


void ScorpionMonster::Movingloop()//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
{
	
	if (NowState == STATE::HURT || NowState == STATE::SHOOTING || NowState == STATE::DEAD)
	{
		return;
	}

	if (count >= /*120*/40 && NowState != STATE::WALKING)//1.92초에 한번 2ch
	{
		angle = 45 + ((rand() % 8) * 45);//360도를 8로 나눠서 8개의 방향에서 랜덤으로 움직일 곳을 뽑는다.
		SetState(WALKING);
	}
	else if (NowState != WALKING)
	{
		count++;
		return;
	}

	if (NowState != STATE::HURT && NowState == STATE::WALKING)
	{
	
		if (walkcount >= 15 && NowState == STATE::WALKING)
		{
			SetState(IDLE);
			count = 0;
			walkcount = 0;
			return;
		}
		if (MonsterMove(angle, ScorpionSpeed) == FALSE)
		{
			angle = 45 + ((rand() % 8) * 45);//360도를 8로 나눠서 8개의 방향에서 랜덤으로 움직일 곳을 뽑는다.
			walkcount--;
		}
		walkcount++;
		
	}
	
}


void ScorpionMonster::MonsterHit(BaseBullet* p)
{
	if (NowState == STATE::HURT || NowState == STATE::DEAD)
	{
		return;
	}
	ScorpionHP = ScorpionHP - p->GetDemage();
	NowState = STATE::HURT;
	BitNum = 0;
	if (ScorpionHP <= 0)
	{
		((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::SCORPIONDIE);
		SetState(DEAD);
	}
}



void ScorpionMonster::ShootBullet(/*POINT start, float angle*/)
{
	POINT startpos = HitBox_Pos();
	POINT destpos = ((GameMainController*)maincontroller)->mycharacter->GetHitBoxPos();
	float degree = maincontroller->GetAngle(startpos, destpos) - 20;
	for (int i = 1; i <= 5; i++)//몬스터와 캐릭터의 각도를 중심으로 +20도,-20도 총 40도의 각도에 5발을 한번에 발사한다.
	{
		degree = degree + (i * 8);
		weapon->ShootBullet(startpos.x, startpos.y, degree);
		//GameMainController::ControllerInstance()->bulletlist.push_back(weapon->ShootBullet(startpos.x, startpos.y, degree));
	}
	

}

void ScorpionMonster::MotionMovingLoop()//상태에 따라 모션을 바꿔주는 역할을 하는 함수 //0.1초에 한번씩 실행 
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
		sleeptime = 2;
		MotionCount = 2;
		break;

	case DEAD:
		sleeptime = 2;
		MotionCount = DEADMAX;
		break;
	case SHOOTING:
		sleeptime = 2;
		MotionCount = 2;
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
				BitNum = 0;
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
	else if (NowState == STATE::SHOOTING)
	{
		if (mcount1 >= sleeptime)
		{
			BitNum = (BitNum + 1) % SHOOTBIT::SHOOTMAX;
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


void ScorpionMonster::Shootloop()//1초에 한번씩 실행
{
	shootcount++;
	if (NowState == STATE::HURT || NowState == STATE::SHOOTING || NowState == STATE::DEAD)
	{
		return;
	}
	if (shootcount >= 5)
	{
		POINT start = HitBox_Pos();
		POINT dest = ((GameMainController*)maincontroller)->mycharacter->GetHitBoxPos();

		if (DetectRay(start, dest) && NowState != STATE::SHOOTING)//1초에 한번씩 총을 쏠 수 있는지 검사하고 쏠 수 있으면 총알을 발사해주고 7초+1초 동안 발사하지 않음
		{
			((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::SCORPIONGUN);
			
			SetState(SHOOTING);
			ShootBullet();
			shootcount = 0;
		}
		
	}

}

void ScorpionMonster::MonsterMove(float angle)//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
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

	temppos.x = enemypos.x + (cos(angle * (3.141592f / 180.0f)) * ScorpionSpeed);//호도법을 라디안으로 바꿔서 입력해준다.
	temppos.y = enemypos.y + (sin(angle * (3.141592f / 180.0f)) * ScorpionSpeed);
	if (maincontroller->DetectMapCollision(HitBox_Pos(temppos), HitBox_Radious()))
	{
		return;
	}
	enemypos.x = temppos.x;
	enemypos.y = temppos.y;


}


BOOL ScorpionMonster::MonsterMove(float angle, int distance)//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
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



ScorpionMonster::~ScorpionMonster()
{
	isDestroy = TRUE;
}

int ScorpionMonster::HitBox_Radious()
{
	return HitBox_radious;
}

POINT ScorpionMonster::HitBox_Pos()
{
	POINT pt;
	pt.x = enemypos.x + 60;
	pt.y = enemypos.y + 60;
	return pt;
}

POINT ScorpionMonster::HitBox_Pos(POINT pt)
{
	pt.x = pt.x + 60;
	pt.y = pt.y + 60;
	return pt;
}