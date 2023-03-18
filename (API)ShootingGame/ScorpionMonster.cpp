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


void ScorpionMonster::DropItem()//30%Ȯ���� ���� �Ѿ˻���, 20%Ȯ���� ū �Ѿ˻���, 17%Ȯ���� ü�»���, 10%Ȯ���� �������
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


void ScorpionMonster::Movingloop()//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )
{
	
	if (NowState == STATE::HURT || NowState == STATE::SHOOTING || NowState == STATE::DEAD)
	{
		return;
	}

	if (count >= /*120*/40 && NowState != STATE::WALKING)//1.92�ʿ� �ѹ� 2ch
	{
		angle = 45 + ((rand() % 8) * 45);//360���� 8�� ������ 8���� ���⿡�� �������� ������ ���� �̴´�.
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
			angle = 45 + ((rand() % 8) * 45);//360���� 8�� ������ 8���� ���⿡�� �������� ������ ���� �̴´�.
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
	for (int i = 1; i <= 5; i++)//���Ϳ� ĳ������ ������ �߽����� +20��,-20�� �� 40���� ������ 5���� �ѹ��� �߻��Ѵ�.
	{
		degree = degree + (i * 8);
		weapon->ShootBullet(startpos.x, startpos.y, degree);
		//GameMainController::ControllerInstance()->bulletlist.push_back(weapon->ShootBullet(startpos.x, startpos.y, degree));
	}
	

}

void ScorpionMonster::MotionMovingLoop()//���¿� ���� ����� �ٲ��ִ� ������ �ϴ� �Լ� //0.1�ʿ� �ѹ��� ���� 
{
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


void ScorpionMonster::Shootloop()//1�ʿ� �ѹ��� ����
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

		if (DetectRay(start, dest) && NowState != STATE::SHOOTING)//1�ʿ� �ѹ��� ���� �� �� �ִ��� �˻��ϰ� �� �� ������ �Ѿ��� �߻����ְ� 7��+1�� ���� �߻����� ����
		{
			((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::SCORPIONGUN);
			
			SetState(SHOOTING);
			ShootBullet();
			shootcount = 0;
		}
		
	}

}

void ScorpionMonster::MonsterMove(float angle)//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
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

	temppos.x = enemypos.x + (cos(angle * (3.141592f / 180.0f)) * ScorpionSpeed);//ȣ������ �������� �ٲ㼭 �Է����ش�.
	temppos.y = enemypos.y + (sin(angle * (3.141592f / 180.0f)) * ScorpionSpeed);
	if (maincontroller->DetectMapCollision(HitBox_Pos(temppos), HitBox_Radious()))
	{
		return;
	}
	enemypos.x = temppos.x;
	enemypos.y = temppos.y;


}


BOOL ScorpionMonster::MonsterMove(float angle, int distance)//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
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