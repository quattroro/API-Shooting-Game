#pragma once
#include <Windows.h>
#include "MyDoubleBuffer.h"
class BaseBullet//���ʹ� ������ ������ ������ �ֱ� ������ �˾Ƽ� �����̵��� ���ְ� ��� �Ѿ˵�� ĳ���ʹ� ������Ʈ�ѷ����� �浹�˻縦 �ϰ� ������ �ش�.
{
public:
	enum BulletKind { MYNOMAL, MYENERGY, ENOMAL, BOSS1, BOSS2,  EENERGY};//�� �Ѿ��� ��Ʈ���� ������ġ y��
	enum MYNOMALS { MNOMAL1, MNOMAL2, MNOMAL3, MNOMALMAX };
	enum MYENERGYS { MENERGY1, MENERGY2, MENERGY3, MENERGY4, MENERGY5, MENERGYMAX };
	enum ENOMALS { ENOMAL1, ENOMAL2, ENOMAL3, ENOMALMAX };


	BaseBullet(HWND hWnd, int posx, int posy, float angle, int kind, BOOL ismine);
	
	~BaseBullet();
	
	virtual void BulletMove(POINT pos);
	
	virtual void BulletRender(int scrollx, int scrolly);
	
	POINT GetPos();
	
	BOOL IsMyBullet();
	
	int GetDemage();
	
	int GetBulletSpeed();
	
	float GetBulletAngle();
	
	POINT GetHitBox_Pos();
	
	POINT GetHitBox_Pos(POINT pt);
	
	int GetHitBox_Radious();

	void BossBullet();

	int GetKind();

	void MotionMovingLoop();


protected:

	int demage;
	int kind;
	POINT pos;
	float angle;
	BOOL mybullet;//ĳ���� �Ѿ����� ���� �Ѿ����� ����
	HWND hWndMain;
	//HINSTANCE m_hInst;
	HBITMAP bulletsBit;
	int BitNum;
	int BitSize;
	int bitposy;
	int BulletSpeed;
	int bitpos;
public:

	BOOL IsDestroy;
	int HitBox_radious = 15;
	int BulletCount = 0;
	int BulletState = 0;
	int sleeptime = 0;
	int mcount1 = 0;
	int mcount2 = 0;
	int MotionCount = 0;
};

