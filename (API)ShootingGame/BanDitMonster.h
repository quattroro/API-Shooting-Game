#pragma once
#include "BaseMonster.h"
#include "BaseBullet.h"
#include "BaseWeapon.h"
#include "WeaponBanDit.h"
#include <tgmath.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


class BanDitMonster : public BaseMonster
{
public:
	enum STATE { IDLE, WALKING, HURT, DEAD, RELOADING };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//위와 아래방향의 비트맵은 없기 때문에 두 방향으로만 확인
	enum MONSTERTHREAD { MOVING, MOTION, SHOOT, MTHREADMAX };
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };

	BanDitMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	

	virtual void DrawMonster();


	virtual void Movingloop();//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
	
	virtual void MotionMovingLoop();
	virtual void Shootloop();

	virtual void ShootBullet(/*POINT start, float angle*/);
	
	virtual void MonsterMove(float angle);//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
	BOOL MonsterMove(float angle, int distance);

	virtual void MonsterHit(BaseBullet* p);
	

	virtual ~BanDitMonster();
	

	virtual int HitBox_Radious();
	
	virtual POINT HitBox_Pos();
	
	POINT HitBox_Pos(POINT pt);

	virtual void DropItem();

private:
	HBITMAP BanDitBit;
	int BanDitSpeed;

	//int BitNum;//동작 번호
	//int NowState;//현재 상태
	//int NowDirection;//현재 방향
	int sleeptime;
	int MotionCount;

	//움직임에 사용할 좌표
	float radian;
	float degree;

	float movingdegree[4];
	int intervaltochar;
	int HitBox_radious;
	int BanDitHP;

	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	
	
	//moving and shoot 을 관리하는데 사용할 카운트들 모든 몬스터들의 움직임과 무기 발사와 모션변경을 하나의 스레드로 하기 위해 모든 몬스터들을 각각의 필요한 카운트들을 가지고 있는다
	int count = 0;
	int mcount1 = 0;
	int mcount2 = 0;
	int walkcount = 0;
	int shootcount = 0;
};

