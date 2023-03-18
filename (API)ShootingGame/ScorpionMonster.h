#pragma once
#include "BaseMonster.h"
#include "BaseWeapon.h"
class ScorpionMonster :
    public BaseMonster
{
public:
	enum STATE { IDLE, WALKING, HURT, DEAD, SHOOTING, RELOADING };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//위와 아래방향의 비트맵은 없기 때문에 두 방향으로만 확인
	
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLE5, IDLE6, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURT3, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };
	enum SHOOTBIT { SHOOT1, SHOOT2, SHOOTMAX };

	ScorpionMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);


	virtual void DrawMonster();


	virtual void Movingloop();//몬스터의 움직임에서 필요한 충돌 검사는 맵과 캐릭터와의 충돌 캐릭터의 움직임에서도 검사를 한다. (몬스터와 총알 은 총알의 움직임에서 검사하고, 몬스터와 캐릭터의 )
	virtual void MotionMovingLoop();
	virtual void Shootloop();

	virtual void ShootBullet(/*POINT start, float angle*/);

	virtual void MonsterMove(float angle);//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.

	BOOL MonsterMove(float angle, int distance);

	

	virtual void DropItem();

	virtual void MonsterHit(BaseBullet* p);


	virtual ~ScorpionMonster();


	virtual int HitBox_Radious();

	virtual POINT HitBox_Pos();

	POINT HitBox_Pos(POINT pt);

	//BOOL DetectRay(POINT start, POINT dest);

	

private:
	HBITMAP ScorpionBit;
	int ScorpionSpeed;

	
	int sleeptime;
	int MotionCount;

	//움직임에 사용할 좌표
	float radian;
	float degree;

	float movingdegree[4];
	int intervaltochar;
	int HitBox_radious;
	int ScorpionHP;
	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	
	int count = 0;
	int shootcount = 0;
	int mcount1 = 0;
	int mcount2 = 0;
	
	int movecount = 0;
	int walkcount = 0;
	POINT despoint;//목적지의 위치(캐릭터의 중앙 위치)
	float angle;
};

