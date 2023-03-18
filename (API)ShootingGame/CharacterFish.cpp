#include "CharacterFish.h"
#include "GameMainController.h"


CharacterFish::CharacterFish(BaseMainController* controller, HWND hWnd, HINSTANCE M_hInst, POINT pos) :MyCharacter(controller, hWnd, M_hInst, pos)
{
    hWndMain = hWnd;
    
    FishBitmap = MyDoubleBuffer::GetBufferInstance()->MainBitmaps[MyDoubleBuffer::BITMAPS::CHARACTERFISH];

    FishBitmap2 = FishBitmap;
    Kind = CHARECTERS::FISH;
    pos.x = -1;
    pos.y = -1;
    //hWndMain = hWnd;
    radious = 30;
    HitBox_radious = 20;
    mevespeed = 10;
    IsDestroy = FALSE;
    NowDirection = RIGHT;
    NowState = STATE::IDLE;
    BitNum = 0;
    



    hThread[CTHREAD::MOVING] = CreateThread(NULL, 0, MovingThreadFunc, this, 0, NULL);//일정 시간마다 글자들을 만들어줄 스레드를 만들어준다
}

void CharacterFish::DrawCharacter(int posx, int posy)
{
    TCHAR str[255];
    POINT scroll = maincontroller->GetScrollPos();

    POINT pt1, pt2;
    pt1.x = posx;
    pt1.y = posy;
    pt2 = maincontroller->GetMousePos();
    float degree = maincontroller->GetAngle(pt1, pt2);
    if (NowState == STATE::DASH)
    {
        dashangle += 20;
        FishBitmap2 = MyDoubleBuffer::GetBufferInstance()->GetRotatedBitmap(hWndMain, FishBitmap, 0, 0, 24, 24, dashangle);
        MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, FishBitmap2, posx, posy, 60, 60, 0, 0, 24, 24);
       
    }
    else
    {
        FishBitmap2 = FishBitmap;
        MyDoubleBuffer::GetBufferInstance()->DrawBackBufferBitmap(hWndMain, FishBitmap2, posx, posy, 60, 60, 0 + (BitNum * 24), 0 + (((NowState * 2) + NowDirection) * 24), 24, 24);
    }
    
    
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        if ((*itr).second == TRUE)
        {
            (*itr).first->WeaponRender(posx + weaponpos[NowDirection].x, posy + weaponpos[NowDirection].y, degree);
        }
    }

}
//void Get
BOOL CharacterFish::CharacterCollotion()
{
    return FALSE;
}
POINT CharacterFish::GetPos()
{
    return pos;
}

int CharacterFish::GetRadious()
{
    return radious;
}
void CharacterFish::SetPos(POINT pos)
{
    this->pos = pos;
}
void CharacterFish::UpMove()
{
    this->pos.y -= mevespeed;
    POINT pt;
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        pt = (*itr).first->GetPos();
        pt.y -= mevespeed;
        (*itr).first->SetPos(pt);
    }

}

void CharacterFish::ShootBullet(float angle)
{
    BaseBullet* temp;
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        if ((*itr).second == TRUE)
        {
            if (hThread[CTHREAD::SHOOTING] == NULL)
            {
                hThread[CTHREAD::SHOOTING] = CreateThread(NULL, 0, ShootingThreadFunc, this, 0, NULL);//총알을 총들의 연사속도에 한번씩 발사할 수 있도록 해주는 함수
            }
           
            (*itr).first->ShootBullet(pos.x + weaponpos[NowDirection].x, pos.y + weaponpos[NowDirection].y, angle);
           
        }
    }
}


POINT CharacterFish::GetBulletPos()
{
    POINT pt;
    pt.x = pos.x + weaponpos[NowDirection].x;
    pt.y = pos.y + weaponpos[NowDirection].y;
    return pt;
}

void CharacterFish::DownMove()
{
    this->pos.y += mevespeed;
    POINT pt;
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        pt = (*itr).first->GetPos();
        pt.y += mevespeed;
        (*itr).first->SetPos(pt);
    }
}

void CharacterFish::RightMove()
{
    this->pos.x += mevespeed;
    POINT pt;
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        pt = (*itr).first->GetPos();
        pt.x += mevespeed;
        (*itr).first->SetPos(pt);
    }
}

void CharacterFish::LeftMove()
{
    this->pos.x -= mevespeed;
    POINT pt;
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        pt = (*itr).first->GetPos();
        pt.x -= mevespeed;
        (*itr).first->SetPos(pt);
    }

}

void CharacterFish::SetWeapon(int num)
{   
    int a = 0;
    if (weaponlist.size() == 1)
    {
        return;
    }
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        a++;
        (*itr).second = FALSE;//일단모든 원소를 미장착으로 해주고
        if (a == num)//선택된 번호의 무기를 장착해준다.
        {
            (*itr).second = TRUE;
        }
        
    }
}

//CreateThread함수에서는 일반함수만 호출할 수 있어서 스레드 함수를 static으로 만들어 주고 파라미터로 객체 자신을 주어서 스레드 함수 안에서 객체함수를 호출 할 수 있도록 해주었다.
DWORD WINAPI CharacterFish::MovingThreadFunc(LPVOID p)//문자들을 일정 시간에 따라 만들어 주는 함수
{
    ((CharacterFish*)p)->MovingLoop();
    return 0;
}

DWORD WINAPI CharacterFish::ShootingThreadFunc(LPVOID p)
{
    ((CharacterFish*)p)->ShootingLoop();

    return 0;
}

void CharacterFish::ShootingLoop()
{
    int sleeptime;
    NowShoot = TRUE;
    for (multimap<BaseWeapon*, BOOL>::iterator itr = weaponlist.begin(); itr != weaponlist.end(); itr++)
    {
        if ((*itr).second == TRUE)
        {
            sleeptime = (*itr).first->GetShootSpeed();
            break;
        }
    }
    Sleep(sleeptime);
    NowShoot = FALSE;
    hThread[SHOOTING] = NULL;
    return;
}

void CharacterFish::MovingLoop()
{
    int count = 0;
    while (TRUE)
    {
        if (IsDestroy)
        {
            return;
        }
        switch (NowState)//상황에 따라서 모션의 종류 유지될 시간 일정 시간 뒤에는 무엇을 할지 정해줌
        {
        case IDLE:
            sleeptime = 200;
            break;

        case WALKING:
            sleeptime = 200;
            break;

        case HURT:
            sleeptime = 300;
            MotionCount = 4;
            break;

        case DEAD:
            sleeptime = 200;
            MotionCount = DEADMAX;
            break;

        case DASH:
            sleeptime = 200;
            break;

        }
        if (NowState == STATE::HURT)
        {
            BitNum = (BitNum + 1) % HURTBIT::HURTMAX;
            count++;
            if (count >= MotionCount)
            {
                NowState = STATE::IDLE;
                BitNum = 0;
                count = 0;
            }
        }
        else if (NowState == STATE::DEAD)
        {
            BitNum = (BitNum + 1) % DEADBIT::DEADMAX;
            count++;
            if (count >= MotionCount)
            {
                
                ((GameMainController*)maincontroller)->GameIsOver();
                
            }
        }
        else if (NowState == STATE::IDLE)
        {
            BitNum = (BitNum + 1) % IDLEBIT::IDLEMAX;
        }
        else if (NowState == STATE::WALKING)
        {
            BitNum = (BitNum + 1) % WALKBIT::WALKMAX;
        }
        else if (NowState == STATE::DASH)
        {
            BitNum = 0;
        }
       
        Sleep(sleeptime);
    }
}











void CharacterFish::CharacterHit(/*int demage*/)//무적상태가 있어야함
{
    if (NowState == STATE::HURT || NowState == STATE::DEAD)
    {
        return;
    }
    ((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::CHARACTERHIT);
    HP = HP--;
    NowState = STATE::HURT;
    BitNum = 0;
    if (HP <= 0)
    {
        ((GameMainController*)maincontroller)->soundplayer->PlayEffect(SoundPlayer::SOUNDS::CHARACTERDEAD);
        NowState = STATE::DEAD;
    }


}



void CharacterFish::SetBitNum(int bitnum)
{
    BitNum = bitnum;
}
void CharacterFish::SetState(int state)
{
    NowState = state;
}
int CharacterFish::GetState()
{
    return NowState;
}
void CharacterFish::SetDirection(int direction)
{
    NowDirection = direction;
}
int CharacterFish::GetDirection()
{
    return NowDirection;
}

CharacterFish::~CharacterFish()
{
    IsDestroy = TRUE;
}

POINT CharacterFish::GetHitBoxPos()
{
    POINT pt;
    pt.x = pos.x + 30;
    pt.y = pos.y + 30;
    return pt;
}
POINT CharacterFish::GetHitBoxPos(POINT pt)
{
    //POINT pt;
    pt.x = pt.x + 30;
    pt.y = pt.y + 30;
    return pt;
}
int CharacterFish::GetHitBoxRadious()
{
    return HitBox_radious;
}