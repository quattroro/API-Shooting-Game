#pragma once
#include "BaseMonster.h"
#include "BaseWeapon.h"
class MaggoyNestMonster :public BaseMonster
{
public:
	
	enum MONSTERTHREAD { MOVING, MOTION, MTHREADMAX };
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
	//enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEADMAX };

	MaggoyNestMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);


	virtual void DrawMonster();


	virtual void Movingloop();//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )



	virtual void MotionMovingLoop();


	

	virtual void DropItem();

	virtual void MonsterHit(BaseBullet* p);


	virtual ~MaggoyNestMonster();


	virtual int HitBox_Radious();

	virtual POINT HitBox_Pos();

	POINT HitBox_Pos(POINT pt);



private:
	HBITMAP NestBit;
	//HANDLE hThread[MTHREADMAX];
	int BanDitSpeed;

	
	int sleeptime;
	int MotionCount;
	int spawncount;


	//�����ӿ� ����� ��ǥ
	float radian;
	float degree;

	float movingdegree[4];
	int intervaltochar;
	int HitBox_radious;
	int NestHP;

	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	//int count;

	//moving and shoot
	int count = 0;
	int mcount1 = 0;
	int mcount2 = 0;
};

