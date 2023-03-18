#include "MaggotMonster.h"
#include "GameMainController.h"


MaggotMonster::MaggotMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos) :BaseMonster(p, hInst, hWnd, pos)
{
	
	MaggotSpeed = 2;
	MaggotHP = 40;
	NowDirection = RIGHT;
	NowState = STATE::WALKING;
	BitNum = 0;

	MaggotBit = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::MONSTERMAGGOT];

	movingdegree[0] = 20;
	movingdegree[1] = 160;
	movingdegree[2] = -160;
	movingdegree[3] = -20;
	intervaltochar = 300;
	HitBox_radious = 10;
	WeaponPos[RIGHT].x = 15;
	WeaponPos[RIGHT].y = 2;
	WeaponPos[LEFT].x = -20;
	WeaponPos[LEFT].y = 2;
	//weapon = new WeaponBanDit(maincontroller, hInst, hWndMain, enemypos);
	
}

void MaggotMonster::DrawMonster()
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
	//MyDoubleBuffer::GetBufferInstance()->DrawCircle(hWndMain, (enemypos.x - (scroll.x)) + 15, (enemypos.y - (scroll.y)) + 15, 10);
	MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, MaggotBit, enemypos.x - (scroll.x), enemypos.y - (scroll.y), 30, 30, 0 + (BitNum * 17), 0 + (((NowState * 2) + NowDirection) * 17), 16, 16);
	
}

void MaggotMonster::Movingloop()//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )
{

	POINT despoint;//�������� ��ġ(ĳ������ �߾� ��ġ)
	//int count = 0;
	despoint = ((GameMainController*)maincontroller)->GetCharacterPos();
	if (maincontroller->GetPointDistnace(enemypos, despoint) <= intervaltochar * intervaltochar)//���Ϳ� ĳ������ �Ÿ��� interval�� ���ϰ� �Ǹ� ĳ���͸� ���󰣴�.
	{
		if (NowState != STATE::HURT && NowState != STATE::DEAD)
		{
			despoint = ((GameMainController*)maincontroller)->GetCharacterPos();
			degree = maincontroller->GetAngle(enemypos, despoint);//���� ���� ��ġ���� ������������ ���� �˾Ƴ���.
			MonsterMove(degree);
		}
	}
	
	
}


void MaggotMonster::DropItem()//30%Ȯ���� ���� �Ѿ˻���, 20%Ȯ���� ū �Ѿ˻���, 17%Ȯ���� ü�»���, 10%Ȯ���� �������
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




void MaggotMonster::MonsterHit(BaseBullet* p)
{
	if (NowState == STATE::HURT||NowState==STATE::DEAD)
	{
		return;
	}
	MaggotHP = MaggotHP - p->GetDemage();
	SetState(HURT);
	BitNum = 0;
	if (MaggotHP <= 0)
	{
		((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::MAGGOTDIE);
		SetState(DEAD);
	
	}
}



void MaggotMonster::MotionMovingLoop()
{
	
	switch (NowState)//��Ȳ�� ���� ����� ���� ������ �ð� ���� �ð� �ڿ��� ������ ���� ������
	{
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
				SetState(WALKING);
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
				mcount2 = 0;
				isDestroy = TRUE;
			}
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



void MaggotMonster::MonsterMove(float angle)//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
{
	if (NowState != STATE::WALKING)
	{
		SetState(WALKING);

	}

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

	temppos.x = enemypos.x + cos(angle * (3.141592f / 180.0f)) * MaggotSpeed;//ȣ������ �������� �ٲ㼭 �Է����ش�.
	temppos.y = enemypos.y + (sin(angle * (3.141592f / 180.0f)) * MaggotSpeed);
	if (maincontroller->DetectMapCollision(HitBox_Pos(temppos), HitBox_Radious()))
	{
		return;
	}
	enemypos.x = temppos.x;
	enemypos.y = temppos.y;
}


MaggotMonster::~MaggotMonster()
{
	
}

int MaggotMonster::HitBox_Radious()
{
	return HitBox_radious;
}

POINT MaggotMonster::HitBox_Pos()
{
	POINT pt;
	pt.x = enemypos.x + 15;
	pt.y = enemypos.y + 15;
	return pt;
}

POINT MaggotMonster::HitBox_Pos(POINT pt)
{
	pt.x = pt.x + 15;
	pt.y = pt.y + 15;
	return pt;
}