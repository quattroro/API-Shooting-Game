#pragma once
#include "BaseMonster.h"
#include "BaseWeapon.h"

class BigBanDitMonster :
    public BaseMonster
{

public:
	enum STATE { IDLE, WALKING , SHOOTING , DASH , DASHSTOP, HURT, DEAD , STATEMAX };//PATTERN1= 돌진, PATTERN2 = 벽에 닿으면 퍼지는 총알, PATTERN3 = 캐릭터 방향으로 난사
	enum PATTERN { PATTERN1, PATTERN2, PATTERN3, PATTERN4, PATTERNMAX };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//위와 아래방향의 비트맵은 없기 때문에 두 방향으로만 확인
	enum MONSTERTHREAD { MOVING, MOTION, SHOOT, MTHREADMAX };
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALK7, WALK8, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURT3, HURTMAX };
	
	enum SHOOTINGBIT { SHOOTING1, SHOOTING2, SHOOTINGMAX };
	enum DASHBIT { DASH1, DASH2, DASHMAX };
	enum DASHSTOPBIT { DASHSTOP1, DASHSTOP2, DASHSTOPMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEAD4, DEAD5, DEAD6, DEAD7, DEAD8, DEAD9, DEADMAX };



	BigBanDitMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);


	virtual void DrawMonster();


	virtual void Movingloop();//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )



	virtual void MotionMovingLoop();
	virtual void Shootloop();

	virtual void ShootBullet(int pattern);

	virtual BOOL MonsterMove(float angle, int distance);//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.

	void MonsterDash();

	virtual void MonsterHit(BaseBullet* p);


	virtual ~BigBanDitMonster();


	virtual int HitBox_Radious();

	virtual POINT HitBox_Pos();

	POINT HitBox_Pos(POINT pt);

	virtual void DropItem();

private:
	HBITMAP BigBanDitBit;
	int BigBanDitSpeed;
	POINT BitSize[STATE::STATEMAX];
	int BitPos[STATE::STATEMAX];
	
	int sleeptime;
	int MotionCount;

	//움직임에 사용할 좌표
	float radian;
	float degree;

	float movingdegree[4];
	int intervaltochar;
	int HitBox_radious;
	int BigBanDitHP;

	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	POINT despoint;//목적지의 위치(캐릭터의 중앙 위치)
	float angle = 0;

	//moving and shoot
	int count = 0;
	int mcount1 = 0;
	int mcount2 = 0;
	int walkcount = 0;
	int shootcount = 0;


};

