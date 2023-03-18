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


void BanDitMonster::DropItem()//30%Ȯ���� ���� �Ѿ˻���, 20%Ȯ���� ū �Ѿ˻���, 17%Ȯ���� ü�»���, 10%Ȯ���� �������
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

void BanDitMonster::Movingloop()//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )
{
	
	POINT despoint;//�������� ��ġ(ĳ������ �߾� ��ġ)
	count++;
	
	if (NowState == STATE::HURT || NowState == STATE::DEAD)
	{
		return;
	}

	if (count >= 40 && NowState != STATE::WALKING)//1.92�ʿ� �ѹ� 2ch
	{
		despoint = ((GameMainController*)maincontroller)->GetCharacterPos();
		degree = maincontroller->GetAngle(enemypos, despoint);//���� ���� ��ġ���� ������������ ���� �˾Ƴ���.
		
		SetState(STATE::WALKING);
	}
	despoint = ((GameMainController*)maincontroller)->GetCharacterPos();
	if (maincontroller->GetPointDistnace(enemypos, despoint) <= intervaltochar * intervaltochar)//���Ϳ� ĳ������ �Ÿ��� interval�� ���ϰ� �Ǹ� degree���� -�� �ٲ������.
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
	switch (NowState)//��Ȳ�� ���� ����� ���� ������ �ð� ���� �ð� �ڿ��� ������ ���� ������
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



void BanDitMonster::MonsterMove(float angle)//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
{
	POINT temppos;
	RECT playsize;
	POINT scroll = maincontroller->GetScrollPos();
	
	playsize.left = scroll.x;
	playsize.top = scroll.y;
	playsize.right = scroll.x + PlaySizeX * 60;
	playsize.bottom = scroll.y + PlaySizeY * 60;
	if (PtInRect(&playsize, enemypos) == FALSE)//���Ͱ� Ȱ������ ������ ������ �������� �ʰ� ������ �ִ´�.
	{
		return;
	}

	temppos.x = enemypos.x + cos(angle * (3.141592f / 180.0f)) * BanDitSpeed;//ȣ������ �������� �ٲ㼭 �Է����ش�.
	temppos.y = enemypos.y + (sin(angle * (3.141592f / 180.0f)) * BanDitSpeed);
	if (maincontroller->DetectMapCollision(HitBox_Pos(temppos), HitBox_Radious()))
	{
		return;
	}
	enemypos.x = temppos.x;
	enemypos.y = temppos.y;


}

BOOL BanDitMonster::MonsterMove(float angle, int distance)//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
{
	POINT temppos;
	RECT playsize;
	
	POINT scroll = maincontroller->GetScrollPos();

	playsize.left = scroll.x;
	playsize.top = scroll.y;
	playsize.right = scroll.x + PlaySizeX * 60;
	playsize.bottom = scroll.y + PlaySizeY * 60;
	if (PtInRect(&playsize, enemypos) == FALSE)//���Ͱ� Ȱ������ ������ ������ �������� �ʰ� ������ �ִ´�.
	{
		return FALSE;
	}
	temppos.x = enemypos.x + (cos(angle * (3.141592f / 180.0f)) * distance);//ȣ������ �������� �ٲ㼭 �Է����ش�.
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

		if (DetectRay(start, dest))//1�ʿ� �ѹ��� ���� �� �� �ִ��� �˻��ϰ� �� �� ������ �Ѿ��� �߻����ְ� 7��+1�� ���� �߻����� ����
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