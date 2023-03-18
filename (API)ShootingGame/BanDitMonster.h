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
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//���� �Ʒ������� ��Ʈ���� ���� ������ �� �������θ� Ȯ��
	enum MONSTERTHREAD { MOVING, MOTION, SHOOT, MTHREADMAX };
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };

	BanDitMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);
	

	virtual void DrawMonster();


	virtual void Movingloop();//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )
	
	virtual void MotionMovingLoop();
	virtual void Shootloop();

	virtual void ShootBullet(/*POINT start, float angle*/);
	
	virtual void MonsterMove(float angle);//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
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

	//int BitNum;//���� ��ȣ
	//int NowState;//���� ����
	//int NowDirection;//���� ����
	int sleeptime;
	int MotionCount;

	//�����ӿ� ����� ��ǥ
	float radian;
	float degree;

	float movingdegree[4];
	int intervaltochar;
	int HitBox_radious;
	int BanDitHP;

	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	
	
	//moving and shoot �� �����ϴµ� ����� ī��Ʈ�� ��� ���͵��� �����Ӱ� ���� �߻�� ��Ǻ����� �ϳ��� ������� �ϱ� ���� ��� ���͵��� ������ �ʿ��� ī��Ʈ���� ������ �ִ´�
	int count = 0;
	int mcount1 = 0;
	int mcount2 = 0;
	int walkcount = 0;
	int shootcount = 0;
};

