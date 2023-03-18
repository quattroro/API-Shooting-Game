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


	virtual void Movingloop();//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )



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


	//움직임에 사용할 좌표
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

