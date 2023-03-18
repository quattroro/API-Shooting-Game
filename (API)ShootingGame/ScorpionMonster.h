#pragma once
#include "BaseMonster.h"
#include "BaseWeapon.h"
class ScorpionMonster :
    public BaseMonster
{
public:
	enum STATE { IDLE, WALKING, HURT, DEAD, SHOOTING, RELOADING };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//���� �Ʒ������� ��Ʈ���� ���� ������ �� �������θ� Ȯ��
	
	enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLE5, IDLE6, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURT3, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };
	enum SHOOTBIT { SHOOT1, SHOOT2, SHOOTMAX };

	ScorpionMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);


	virtual void DrawMonster();


	virtual void Movingloop();//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )
	virtual void MotionMovingLoop();
	virtual void Shootloop();

	virtual void ShootBullet(/*POINT start, float angle*/);

	virtual void MonsterMove(float angle);//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.

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

	//�����ӿ� ����� ��ǥ
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
	POINT despoint;//�������� ��ġ(ĳ������ �߾� ��ġ)
	float angle;
};

