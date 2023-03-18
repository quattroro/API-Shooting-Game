#pragma once
#include "BaseMonster.h"
#include "BaseWeapon.h"
class MaggotMonster :
    public BaseMonster
{
public:
	enum STATE { WALKING, HURT, DEAD, RELOADING };
	enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//���� �Ʒ������� ��Ʈ���� ���� ������ �� �������θ� Ȯ��
	enum MONSTERTHREAD { MOVING, MOTION, MTHREADMAX };
	//enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
	enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALKMAX };
	enum HURTBIT { HURT1, HURT2, HURT3, HURTMAX };
	enum DEADBIT { DEAD1, DEAD2, DEAD3, DEAD4, DEAD5, DEAD6, DEADMAX };

	MaggotMonster(BaseMainController* p, HINSTANCE hInst, HWND hWnd, POINT pos);


	virtual void DrawMonster();


	virtual void Movingloop();//������ �����ӿ��� �ʿ��� �浹 �˻�� �ʰ� ĳ���Ϳ��� �浹 ĳ������ �����ӿ����� �˻縦 �Ѵ�. (���Ϳ� �Ѿ� �� �Ѿ��� �����ӿ��� �˻��ϰ�, ���Ϳ� ĳ������ )



	virtual void MotionMovingLoop();



	virtual void MonsterMove(float angle);//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.

	

	virtual void DropItem();
	virtual void MonsterHit(BaseBullet* p);


	virtual ~MaggotMonster();


	virtual int HitBox_Radious();

	virtual POINT HitBox_Pos();

	POINT HitBox_Pos(POINT pt);



private:
	HBITMAP MaggotBit;
	int MaggotSpeed;
	int MaggotDashSpeed;
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
	int MaggotHP;

	BaseWeapon* weapon;
	POINT WeaponPos[DIRECTIONMAX];
	

	//moving and shoot
	int count = 0;
	int mcount1 = 0;
	int mcount2 = 0;
};

