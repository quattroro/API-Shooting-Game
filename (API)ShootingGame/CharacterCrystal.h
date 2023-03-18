#pragma once
#include "MyCharacter.h"
#include "MyDoubleBuffer.h"


class CharacterCrystal :public MyCharacter
{
public:

    enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//위와 아래방향의 비트맵은 없기 때문에 두 방향으로만 확인
    enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
    enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
    enum HURTBIT { HURT1, HURT2, HURTMAX };
    enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };


    CharacterCrystal(BaseMainController* controller, HWND hWnd, HINSTANCE M_hInst, POINT pos);/* :MyCharacter(controller, hWnd, M_hInst, pos)*/

    void ShootingLoop();
    virtual void DrawCharacter(int posx, int posy);

    virtual BOOL CharacterCollotion();

    virtual POINT GetPos();


    virtual int GetRadious();

    virtual void SetPos(POINT pos);

    virtual void UpMove();


    virtual void DownMove();


    virtual void RightMove();


    virtual void LeftMove();


    virtual void SetWeapon(int num);

    virtual void Defense();

    //CreateThread함수에서는 일반함수만 호출할 수 있어서 스레드 함수를 static으로 만들어 주고 파라미터로 객체 자신을 주어서 스레드 함수 안에서 객체함수를 호출 할 수 있도록 해주었다.
    static DWORD WINAPI MovingThreadFunc(LPVOID p);//문자들을 일정 시간에 따라 만들어 주는 함수

    static DWORD WINAPI ShootingThreadFunc(LPVOID p);

    void MovingLoop();

    virtual void ShootBullet(float angle);
    virtual void CharacterHit(/*int demage*/);//무적상태가 있어야함

    virtual void SetBitNum(int bitnum);

    virtual void SetState(int state);

    virtual int GetState();

    virtual void SetDirection(int direction);

    virtual int GetDirection();


    virtual ~CharacterCrystal();

    virtual POINT GetBulletPos();
    virtual POINT GetHitBoxPos();

    virtual POINT GetHitBoxPos(POINT pt);

    virtual int GetHitBoxRadious();


private:
    HBITMAP CrystalBitmap;
    HWND hWndMain;
    RECT HitBox;
    int radious;
    int sleeptime;
    BOOL IsDestroy;
    HANDLE hEvent;

    int dashspeed;
    
    POINT BitSize;

public:
    int MotionCount;

};

