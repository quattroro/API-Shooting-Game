#pragma once
#include "MyCharacter.h"
#include "MyDoubleBuffer.h"
//#include "BaseMainController.h"
//#include "GameMainController.h"
//using GameMainController;

//class GameMainController;

class CharacterFish :public MyCharacter
{
public:
    
    enum DIRECTION { RIGHT = 0, LEFT = 1, DIRECTIONMAX };//���� �Ʒ������� ��Ʈ���� ���� ������ �� �������θ� Ȯ��
    enum IDLEBIT { IDLE1, IDLE2, IDLE3, IDLE4, IDLEMAX };
    enum WALKBIT { WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALKMAX };
    enum HURTBIT { HURT1, HURT2, HURTMAX };
    enum DEADBIT { DEAD1, DEAD2, DEAD3, DEADMAX };
    

    CharacterFish(BaseMainController* controller, HWND hWnd, HINSTANCE M_hInst, POINT pos);/* :MyCharacter(controller, hWnd, M_hInst, pos)*/
   
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



    //CreateThread�Լ������� �Ϲ��Լ��� ȣ���� �� �־ ������ �Լ��� static���� ����� �ְ� �Ķ���ͷ� ��ü �ڽ��� �־ ������ �Լ� �ȿ��� ��ü�Լ��� ȣ�� �� �� �ֵ��� ���־���.
    static DWORD WINAPI MovingThreadFunc(LPVOID p);//���ڵ��� ���� �ð��� ���� ����� �ִ� �Լ�
   
    static DWORD WINAPI ShootingThreadFunc(LPVOID p);

    void MovingLoop();
 
    //virtual BaseBullet* ShootBullet(float angle);
    virtual void ShootBullet(float angle);
    virtual void CharacterHit(/*int demage*/);//�������°� �־����
  
    virtual void SetBitNum(int bitnum);
   
    virtual void SetState(int state);
  
    virtual int GetState();
    
    virtual void SetDirection(int direction);
  
    virtual int GetDirection();
   

    virtual ~CharacterFish();
   
    virtual POINT GetBulletPos();
    virtual POINT GetHitBoxPos();
  
    virtual POINT GetHitBoxPos(POINT pt);
   
    virtual int GetHitBoxRadious();
    

private:
    HBITMAP FishBitmap;
    HBITMAP FishBitmap2;
    HWND hWndMain;
    RECT HitBox;
    int radious;
    int sleeptime;
    BOOL IsDestroy;
    HANDLE hEvent;
    float dashangle;
    int dashspeed;

public:
    int MotionCount;
    
};

