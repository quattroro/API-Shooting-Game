#pragma once
#include "BaseMonster.h"
#include "BaseWeapon.h"

class BigBanDitMonster :
    public BaseMonster
{

public:
	enum STATE { IDLE, WALKING , SHOOTING , DASH , DASHSTOP, HURT, DEAD , STATEMAX };//PATTERN1= ����, PATTERN2 = ���� ������ ������ �Ѿ�, PATTERN3 = ĳ���� �������� ����
	enum PATTERN { PATTERN1, PATTERN2, PATTERN3, PATTERN4, PATTERNMAX };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//���� �Ʒ������� ��Ʈ���� ���� ������ �� �������θ� Ȯ��
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


	virtual void Movingloop();//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )



	virtual void MotionMovingLoop();
	virtual void Shootloop();

	virtual void ShootBullet(int pattern);

	virtual BOOL MonsterMove(float angle, int distance);//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.

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

	//�����ӿ� ����� ��ǥ
	float radian;
	float degree;

	float movingdegree[4];
	int intervaltochar;
	int HitBox_radious;
	int BigBanDitHP;

	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	POINT despoint;//�������� ��ġ(ĳ������ �߾� ��ġ)
	float angle = 0;

	//moving and shoot
	int count = 0;
	int mcount1 = 0;
	int mcount2 = 0;
	int walkcount = 0;
	int shootcount = 0;


};

