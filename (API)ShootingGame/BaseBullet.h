#pragma once
#include <Windows.h>
#include "MyDoubleBuffer.h"
class BaseBullet//몬스터는 각자의 움직임 패턴이 있기 때문에 알아서 움직이도록 해주고 모든 총알들과 캐릭터는 메인컨트롤러에서 충돌검사를 하고 움직여 준다.
{
public:
	enum BulletKind { MYNOMAL, MYENERGY, ENOMAL, BOSS1, BOSS2,  EENERGY};//각 총알의 비트맵의 시작위치 y값
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
	BOOL mybullet;//캐릭터 총알인지 적군 총알인지 구분
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

