#include "GameMainController.h"

GameMainController* GameMainController::_Instance = NULL;
GameMainController* GameMainController::ControllerInstance()//싱글톤
{
	if (_Instance == NULL)
	{
		_Instance = new GameMainController();
	}
	return _Instance;
}
CRITICAL_SECTION GameMainController::crit;

GameMainController::GameMainController()//처음 컨드롤러가 만들어 질때 초기화들을 해줄 함수
{
	InitializeCriticalSection(&crit);
	for (int i = 0; i < 4; i++)
	{
		KeyBuffer[i] = FALSE;
	}
	mycharacter = NULL;
	for (int i = 0; i < MAINTHREADMAX; i++)
	{
		hThread[i] = NULL;
	}
	for (int i = 0; i < MAINTHREADMAX; i++)
	{
		hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	/*for (int i = 0; i < MONSTERTHREAD::MTHREADMAX; i++)
	{
		MonsterhThread[i] = NULL;
	}*/
	NowGameState = GAMESTATE::CSELLECT;
	mousepoint.x = -1;
	mousepoint.y = -1;
	//GameClear = FALSE;
	//GameOver = FALSE;
	soundplayer = NULL;
	SelectCharacter = -1;

}

void GameMainController::InitSetting(HINSTANCE hInst, HWND MhWnd, MyDoubleBuffer* doublebuffer)//컨트롤러에서 사용할 비트맵들을 불러오는 등의 작업을 해줄 함수
{
	//_Instance = this;

	NowStage = STAGE::NOTHING;
	this->doublebuffer = doublebuffer;
	//->soundplayer = new SoundPlayer();
	hWndMain = MhWnd;
	M_hInst = hInst;
	GetClientRect(hWndMain, &crt);
	

	StageBit[STAGE1] = (HBITMAP)LoadImage(hInst, TEXT("map tiles/stage1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	StageBit[STAGE2] = (HBITMAP)LoadImage(hInst, TEXT("map tiles/bossstage.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GameOverBit = (HBITMAP)LoadImage(hInst, TEXT("GameOver.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GameClearBit = (HBITMAP)LoadImage(hInst, TEXT("GameClear.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	startscreen = (HBITMAP)LoadImage(hInst, TEXT("characterselect.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CharacterCard = (HBITMAP)LoadImage(hInst, TEXT("Bitmaps/Characters/CharacterCard.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hThread[MAINTHREAD] == NULL)
	{
		hThread[MAINTHREAD] = CreateThread(NULL, 0, MainThreadFunc, this, 0, NULL);//키보드 입력을 받을 스레드
	}
	if (hThread[MAINTHREADS::MOVING] == NULL)//몬스터의 움직임을 담당할 스레드
	{
		hThread[MAINTHREADS::MOVING] = CreateThread(NULL, 0, MonsterMovingThreadFunc, this, 0, NULL);
	}
	if (hThread[MAINTHREADS::MOTION] == NULL)//몬스터의 모션변경을 담당할 스레드
	{
		hThread[MAINTHREADS::MOTION] = CreateThread(NULL, 0, MonsterMotionThreadFunc, this, 0, NULL);
	}

	if (hThread[MAINTHREADS::SHOOT] == NULL)//몬스터의 총알 발사를 담당할 스레드
	{
		hThread[MAINTHREADS::SHOOT] = CreateThread(NULL, 0, MonsterShootThreadFunc, this, 0, NULL);
	}
	
	for (int i = 0; i < MyCharacter::CHARECTERS::CMAX; i++)
	{
		SetRect(&CharacterSelectPos[i], (i * 151), (crt.bottom - 200), (i * 151) + 150, (crt.bottom - 200) + 200);
	}
	
	if (soundplayer == NULL)
	{
		soundplayer = new SoundPlayer();
	}

	
	ScreenRender();
	


}

BOOL GameMainController::SetScroll()//처음 캐릭터의 월드 좌표에서 화면크기의 절반씩 만큼 뺀 값을 스크롤값(월드에서 카메라의위치)으로 설정 해준다.
{
	
	int wx = MyPosWorldIndex.x - 9;
	int wy = MyPosWorldIndex.y - 5;
	RECT crt;
	GetClientRect(hWndMain, &crt);

	wx = MyPosWorld.x - (crt.right / 2);
	wy = MyPosWorld.y - (crt.bottom / 2);

	

	if (wx < 0)
	{
		wx = 0;
	}
	else if (wx + crt.right >= WorldMapSize.x)
	{
		wx = (MapsizeX)-PlaySizeX;
	}
	if (wy < 0)
	{
		wy = 0;
	}
	else if (wy + crt.bottom >= WorldMapSize.y)
	{
		wy = (MapsizeY)-PlaySizeY;
	}
	/*if (scrollIndex_X == wx && scrollIndex_Y == wy)
	{
		return FALSE;
	}*/
	scroll_X = wx;
	scroll_Y = wy;
	//scrollIndex_X = wx;
	//scrollIndex_Y = wy;
	return TRUE;
}

void GameMainController::StageSetting()//스테이지기 처음 생성될때 맵에있는 초기화 정보들을 미리 불러오기 위한 함수
{
	
	if (NowStage == STAGE::NOTHING)
	{
		NowStage = STAGE::STAGE1;
	}
	if (NowGameState != PLAYING)
	{
		NowGameState = PLAYING;
	}
	int info = -1;
	BOOL flag = FALSE;

	BITMAP bit;
	GetObject(StageBit[NowStage], sizeof(BITMAP), &bit);//현재 스테이지의 비트맵의 정보를 받아와서
	WorldMapSize.x = bit.bmWidth;//너비와 크기를 월드맵의 영역으로 저장해 준다.
	WorldMapSize.y = bit.bmHeight;

	//spawnlist.clear();
	for (int y = 0; y < MapsizeY; y++)
	{
		for (int x = 0; x < MapsizeX; x++)
		{
			info = WordMapInfo[NowStage][y][x];
			if (info == PSPAWN)
			{
				MyPosWorld.x = x * 60;//캐릭터의 월드 좌표를 구한다
				MyPosWorld.y = y * 60;
				SetScroll();
				
			}
			if (info == ESPAWN)
			{
				maxmonster++;
			}
		}
	}
	

	//월드맵에서의 절대좌표 (캐릭터화면내의 모든 충돌처리를 위해 월드맵에서의 좌표를 이용함 캐릭터가 움직였을때는 캐릭터의 움직임에따라서 카메라도 같이 움직인다. )
	POINT mypos;
	mypos.x = MyPosWorld.x;
	mypos.y = MyPosWorld.y;
	
	if (mycharacter == NULL)
	{
		if (SelectCharacter == MyCharacter::CHARECTERS::FISH)
		{
			mycharacter = new CharacterFish(this, hWndMain, M_hInst, mypos);//현재 나의 좌표로 캐릭터를 만들어 준다.
		}
		else if (SelectCharacter == MyCharacter::CHARECTERS::CRYSTAL)
		{
			mycharacter = new CharacterCrystal(this, hWndMain, M_hInst, mypos);//현재 나의 좌표로 캐릭터를 만들어 준다.
		}

	}
	else
	{
		mycharacter->SetPos(mypos);//만약 이미 캐릭터가 만들어져 있으면(스테이지를 끝내고 다음 스테이지로 넘어갔을때) 캐릭터의 위치만 초기화 해준다.
	}
	
	for (int i = 0; i < MAINTHREADMAX; i++)
	{
		SetEvent(hEvent[i]);
	}

	if (NowStage == STAGE1)
	{
		soundplayer->PlayBGM(SoundPlayer::BGM::DRYLAND);
	}
	else if (NowStage == STAGE2)
	{
		soundplayer->BGMStop();
		soundplayer->PlayBGM(SoundPlayer::BGM::BIGBANDIT);
	}
	LoadPlayMap();//맵을 로딩해 준다.
	//soundplayer->PlayBGM();
}

//움직임 관리 스레드
DWORD WINAPI GameMainController::MonsterMovingThreadFunc(LPVOID p)
{
	WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::MOVING], INFINITE);
	while (TRUE)
	{
		if (((GameMainController*)p)->NowGameState == GAMESTATE::GAMECLEAR)
		{
			return 0;
		}
		if (((GameMainController*)p)->NowGameState == GAMESTATE::GAMEOVER)
		{
			WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::MOVING], INFINITE);
		}
		EnterCriticalSection(&GameMainController::crit);
		for (list<BaseMonster*>::iterator itr = ((GameMainController*)p)->monsterlist.begin(); itr != ((GameMainController*)p)->monsterlist.end(); itr++)
		{
			if (!(*itr)->isDestroy)
			{
				(*itr)->Movingloop();
			}
		}
		LeaveCriticalSection(&GameMainController::crit);
		Sleep(50);
	}
	
}
//모션 관리 스레드
DWORD WINAPI GameMainController::MonsterMotionThreadFunc(LPVOID p)
{
	WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::MOTION], INFINITE);
	while (TRUE)
	{
		if (((GameMainController*)p)->NowGameState == GAMESTATE::GAMECLEAR)
		{
			return 0;
		}
		if (((GameMainController*)p)->NowGameState == GAMESTATE::GAMEOVER)
		{
			WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::MOTION], INFINITE);
		}
		EnterCriticalSection(&GameMainController::crit);
		for (list<BaseMonster*>::iterator itr = ((GameMainController*)p)->monsterlist.begin(); itr != ((GameMainController*)p)->monsterlist.end(); itr++)
		{
			if (!(*itr)->isDestroy)
			{
				(*itr)->MotionMovingLoop();
			}
			
		}
		LeaveCriticalSection(&GameMainController::crit);
		Sleep(100);
	}
}

DWORD WINAPI GameMainController::MonsterShootThreadFunc(LPVOID p)
{
	WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::SHOOT], INFINITE);
	while (TRUE)
	{
		if (((GameMainController*)p)->NowGameState == GAMESTATE::GAMECLEAR)
		{
			return 0;
		}
		if (((GameMainController*)p)->NowGameState==GAMESTATE::GAMEOVER)
		{
			WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::SHOOT], INFINITE);
		}
		EnterCriticalSection(&GameMainController::crit);
		for (list<BaseMonster*>::iterator itr = ((GameMainController*)p)->monsterlist.begin(); itr != ((GameMainController*)p)->monsterlist.end(); itr++)
		{
			if (!(*itr)->isDestroy)
			{
				(*itr)->Shootloop();
			}
		}
		LeaveCriticalSection(&GameMainController::crit);
		Sleep(1000);
	}
}


float GameMainController::GetAngle(POINT startpoint, POINT destpoint)
{
	float rx = destpoint.x - startpoint.x;
	float ry = destpoint.y - startpoint.y;
	//시작점을 중점으로 했을때 도착점이 위치한 사분면(각으로 움직일때는 )

	float radian = atan2(ry, rx);//아크탄젠트2 함수는 값이 -PI~PI 까지의 범위로 나오기 때문에 1,2사분면은 반시계방향으로 양수로 3,4 사분면은 시계방향으로 양수로 나오게 된다. 
	float degree = radian * (180.0f / 3.141592f);//라디안 값을 호도법으로 변환해줌
	return degree;
}




void GameMainController::LoadPlayMap()//캐릭터가 움직이면서 배열에서의 인덱스값이 변화하면 그때마다 새로운 영역의 값을 다시 읽어와서 몬스터를 스폰해준다.
{
	//srand((unsigned)time(NULL));
	int info;
	POINT pt;
	BOOL flag = true;
	scrollIndex_X= scroll_X / 60;//현재 스크롤 값을 이용해 배열정보에서의 인덱스를 알아낸다
	scrollIndex_Y= scroll_Y / 60;

	for (int y = 0; y < PlaySizeY; y++)
	{
		for (int x = 0; x < PlaySizeX; x++)
		{
			info = WordMapInfo[NowStage][scrollIndex_Y + y][scrollIndex_X + x];
			switch (info)
			{
			case PSPAWN:
				//info = BLANK;
				break;

			case ESPAWN:
				flag = true;
				pt.x = scrollIndex_X + x;
				pt.y = scrollIndex_Y + y;
				for (list<POINT>::iterator itr = spawnlist.begin() ; itr != spawnlist.end(); itr++)
				{
					if ((*itr).x == pt.x && (*itr).y == pt.y)
					{
						flag = false;
						break;
					}
				}
				if (flag)
				{
					spawnlist.push_back(pt);
					MonsterSpawn((scrollIndex_X + x) * 60, (scrollIndex_Y + y) * 60);
				}
				//info = BLANK;
				break;
			}
		}
	}
}

void GameMainController::MonsterSpawn(int x, int y)//특정 위치에 현재 스테이지에서 나올 수 있는 몬스터 중에 랜덤으로 뽑아서 스폰해줌 
{
	srand((unsigned)time(NULL));


	int enemynum = rand() % BaseMonster::ENEMYS::ENEMYMAX;
	POINT pt;
	pt.x = x;
	pt.y = y;
	if (NowStage == 1)
	{
		monsterlist.push_back(new BigBanDitMonster(this, M_hInst, hWndMain, pt));
		return;
	}
	switch (enemynum)
	{
	case BaseMonster::ENEMYS::BANDIT:
		monsterlist.push_back(new BanDitMonster(this, M_hInst, hWndMain, pt));
		break;
	
	case BaseMonster::ENEMYS::MAGGOTNEST:
		monsterlist.push_back(new MaggoyNestMonster(this, M_hInst, hWndMain, pt));
		break;
	case BaseMonster::ENEMYS::SCORPION:
		monsterlist.push_back(new ScorpionMonster(this, M_hInst, hWndMain, pt));
		break;
	}

}


POINT GameMainController::GetCharacterPos()
{
	POINT temp = mycharacter->GetPos();
	//temp.x += 30;
	//temp.y += 30;
	return temp;
}


BOOL GameMainController::DetectMapCollision(POINT pos, int radious)//하나에라도 충돌을 하면 바로 true를 리턴한다.//중심점과 반지름을 넣어주면 맵의 활성영역 안에서 충돌을 검사한다.//움직일 좌표가 활성영역 안에서 몇번째 인덱스 인지 알아내고 그 인덱스의 가로세로 2칸씩 4*4의 범위만 가지고 원과 네모의 충돌검사를 한다.
{
	POINT index;
	TCHAR str[255];
	int posx;
	int posy;
	POINT rpos;
	POINT cpos = pos;
	
	index.x = (pos.x  / 60) - 1;//움직일 좌표가 활성영역 안에서 몇번째 인덱스 인지 알아내고 그 인덱스의 가로세로 1칸씩 3*3의 범위만 가지고 원과 네모의 충돌검사를 한다.
	index.y = (pos.y  / 60) - 1;
	
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			//if (PlayMapInfo[index.y + y][index.x + x] != BLANK)
			if (WordMapInfo[NowStage][index.y + y][index.x + x] != BLANK && WordMapInfo[NowStage][index.y + y][index.x + x] != PSPAWN && WordMapInfo[NowStage][index.y + y][index.x + x] != ESPAWN)
			{
				
				rpos.x = ((index.x + x) * 60);//월드에서의 벽의 좌표를 찾아서 충돌 검사를 한다.
				rpos.y = ((index.y + y) * 60);

				//벽과의 충돌은 사각형과 원의 충돌로 검사한다. 다른 충돌들은 원과 원의 충돌로 구현
				if (isMapCollision(rpos, 60, cpos, radious) == TRUE)
				{
					
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL GameMainController::isMapCollision(POINT rectpos, int size, POINT cpos/*중심점*/, int radious)//맵과 다른 객체들의 충돌을 검사할 함수 충돌했으면 true 아니면 false를 리턴
{
	TCHAR str[255];
	BOOL flag1 = FALSE, flag2 = FALSE;
	if (((cpos.x + radious) <= rectpos.x) || ((cpos.x - radious) >= (rectpos.x + size)) || ((cpos.y + radious) <= rectpos.y) || ((cpos.y - radious) >= (rectpos.y + size)))//원이 네모보다 상하좌우로 더 위에 있으면 충돌하지 않음
	{
		return FALSE;
	}
	//x축 검사
	// 원이 오른쪽에 있을때
	if (cpos.x >= rectpos.x)
	{
		if ((cpos.x - (rectpos.x + size)) <= radious)
		{
			flag1 = TRUE;
		}
	}
	if (cpos.x <= rectpos.x)// 원이 왼쪽에 있을때
	{
		if ((rectpos.x - cpos.x) <= radious)
		{
			flag1 = TRUE;
		}
	}
	//y축 검사
	//원이 위쪽에 있을때
	if (cpos.y <= rectpos.y)
	{
		if ((rectpos.y - cpos.y) <= radious)
		{
			flag2 = TRUE;
		}
	}
	if (cpos.y >= rectpos.y)//원이 아래쪽에 있을때
	{

		if ((cpos.y - (rectpos.y + size)) <= radious)
		{
			flag2 = TRUE;
		}
	}
	if (flag1 == TRUE && flag2 == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}





void GameMainController::KeyDownProc(WPARAM wParam)
{
	if (NowGameState == CSELLECT)
	{
		if (wParam == VK_RETURN && SelectCharacter != -1)
		{
			//게임 시작
			StageSetting();
			ScreenRender();
			ShowCursor(FALSE);
			
		}
	}
	switch (wParam)
	{
	case 'W':
		if (KeyBuffer[UP] == FALSE)
		{
			KeyBuffer[UP] = TRUE;
		}
		
		break;

	case 'A':
		if (KeyBuffer[LEFT] == FALSE)
		{
			KeyBuffer[LEFT] = TRUE;
		}
		
		break;

	case 'S':
		if (KeyBuffer[DOWN] == FALSE)
		{
			KeyBuffer[DOWN] = TRUE;
		}
		
		break;

	case 'D':
		if (KeyBuffer[RIGHT] == FALSE)
		{
			KeyBuffer[RIGHT] = TRUE;
		}
		break;
	case '1':
		if (KeyBuffer[NUM1] == FALSE)
		{
			KeyBuffer[NUM1] = TRUE;
		}
		break;


	case '2':
		if (KeyBuffer[NUM2] == FALSE)
		{
			KeyBuffer[NUM2] = TRUE;
		}

		break;
	}
}

void GameMainController::KeyUpProc(WPARAM wParam)
{
	switch (wParam)
	{
	case 'W':
		if (KeyBuffer[UP] == TRUE)
		{
			KeyBuffer[UP] = FALSE;
		}
		
		break;

	case 'A':
		if (KeyBuffer[LEFT] == TRUE)
		{
			KeyBuffer[LEFT] = FALSE;
		}
		
		break;

	case 'S':
		if (KeyBuffer[DOWN] == TRUE)
		{
			KeyBuffer[DOWN] = FALSE;
		}
		
		break;

	case 'D':
		if (KeyBuffer[RIGHT] == TRUE)
		{
			KeyBuffer[RIGHT] = FALSE;
		}
		
		break;
	case '1':
		if (KeyBuffer[NUM1] == TRUE)
		{
			KeyBuffer[NUM1] = FALSE;
		}
		break;


	case '2':
		if (KeyBuffer[NUM2] == TRUE)
		{
			KeyBuffer[NUM2] = FALSE;
		}
		break;
	}
}

void GameMainController::DeleteMyObject()
{

	if (NowGameState != PLAYING)
	{
		return;
	}
	for (list<BaseBullet*>::iterator itr = bulletlist.begin(); itr != bulletlist.end(); )
	{
		if ((*itr)->IsDestroy)
		{
			BaseBullet* temp = (*itr);
			delete temp;
			bulletlist.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
	for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end();)
	{
		if ((*itr)->isDestroy)
		{
			BaseMonster* temp2 = (*itr);
			delete temp2;
			monsterlist.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
	if (monsterlist.size() == 0 && spawnlist.size() == maxmonster)//맵의 모든 몬스터가 소환 되었고 그 몬스터 들이 모두 죽었을때 -> 스테이지 클리어
	{
		if (NowStage == STAGE2)//게임 끝
		{
			GameIsClear();
			return;
		}
		NowStage++;
		StageSetting();
		LoadPlayMap();
	}

}

void GameMainController::BulletMove(BaseBullet* p)//몬스터가 스크롤 영역 밖으로 나갔으면 움직이지 않도록 해준다.
{

	POINT temppos;
	RECT playsize;
	POINT scroll;
	POINT pos = p->GetPos();
	float angle = p->GetBulletAngle();
	int movespeed = p->GetBulletSpeed();
	//scroll.x = scrollIndex_X * 60;
	//scroll.y = scrollIndex_Y * 60;

	scroll.x = scroll_X;
	scroll.y = scroll_Y;

	playsize.left = scroll.x;
	playsize.top = scroll.y;
	playsize.right = scroll.x + PlaySizeX * 60;
	playsize.bottom = scroll.y + PlaySizeY * 60;
	if (PtInRect(&playsize, pos) == FALSE)//총알이 활성영역 밖으로 나가면 총알은 사라진다.
	{
		p->IsDestroy = true;
		return;
	}

	temppos.x = pos.x + cos(angle * (3.141592f / 180.0f)) * movespeed;//호도법을 라디안으로 바꿔서 입력해준다.
	temppos.y = pos.y + (sin(angle * (3.141592f / 180.0f)) * movespeed);
	if (DetectMapCollision(p->GetHitBox_Pos(temppos) , p->GetHitBox_Radious()))
	{
		if (p->GetKind() == BaseBullet::BOSS1)
		{
			POINT startpos;
			POINT pt = p->GetPos();
			float degree = 0;
			while (degree <= 360)
			{
				degree += 20.0f;
				startpos.x = pt.x + cos((float)3.141592 * degree / 180) * 20;
				startpos.y = pt.y - sin((float)3.141592 * degree / 180) * 20;
				bulletlist.push_back(new BaseBullet(hWndMain, startpos.x, startpos.y, degree, BaseBullet::ENOMAL, FALSE));
			}
		}
		p->IsDestroy = true;
		return;
	}
	if (BulletCollisionToRef(temppos,p))
	{
		return;
	}
	p->BulletMove(temppos);
}

BOOL GameMainController::BulletCollisionToRef(POINT pos, BaseBullet* p)//충돌이 일어나면 각각의 행동을 해준다.
{
	POINT pt;
	//총알과 캐릭터의 충돌
	if (!(p->IsMyBullet()))//캐릭터가 쏜 총알이 아닐때
	{
		pt = mycharacter->GetHitBoxPos();
		if ((pos.x - 100 <= pt.x && pos.x + 100 >= pt.x) && (pos.y - 100 <= pt.y && pos.y + 100 >= pt.y))//충돌검사를 하는 객체가 총알의 반경 200만큼의 네모영역에 있을때만 충돌검사를 한다.
		{
			if (isCollision(p->GetHitBox_Pos(pos), p->GetHitBox_Radious(), mycharacter->GetHitBoxPos(), mycharacter->GetHitBoxRadious()))
			{
				if (mycharacter->GetState() != MyCharacter::STATE::DEFENSE)
				{
					mycharacter->CharacterHit(/*p->GetDemage()*/);
				}
				p->IsDestroy = TRUE;
				return TRUE;
			}
		}
	}
	//캐릭터가 쏜 총알일때
	else//총알과 몬스터의 충돌
	{
		for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
		{
			pt = (*itr)->HitBox_Pos();
			if ((pos.x - 100 <= pt.x && pos.x + 100 >= pt.x) && (pos.y - 100 <= pt.y && pos.y + 100 >= pt.y))//충돌검사를 하는 객체가 총알의 반경 200만큼의 네모영역에 있을때만 충돌검사를 한다.
			{
				if (isCollision(p->GetHitBox_Pos(pos), p->GetHitBox_Radious(), (*itr)->HitBox_Pos(), (*itr)->HitBox_Radious()))
				{
					(*itr)->MonsterHit(p);
					p->IsDestroy = TRUE;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

float GameMainController::GetPointDistnace(POINT pt1, POINT pt2)//두 점 간의 거리의 제곱을 구해주는 함수 제곱근 함수는 일부러 사용하지 않음
{
	float distance = ((pt1.x - pt2.x) * (pt1.x - pt2.x)) + ((pt1.y - pt2.y) * (pt1.y - pt2.y));
	return distance;
}

BOOL GameMainController::isCollision(POINT pos1, int radious1, POINT pos2, int radious2)
{
	float distance = GetPointDistnace(pos1, pos2);
	float radious = (radious1 + radious2) * (radious1 + radious2);
	if (distance <= radious)
	{
		return TRUE;
	}
	return FALSE;
}
void GameMainController::BulletsMove()//총알의 위치랑 각도를 받아와서 충돌검사들을 하고 처리들을 해준다.
{
	if (NowGameState != PLAYING)
	{
		return;
	}
	POINT startpos;
	POINT destpos;
	float angle;
	for (list<BaseBullet*>::iterator itr = bulletlist.begin(); itr != bulletlist.end(); itr++)
	{
		BulletMove(*itr);
	}
}

void GameMainController::Charactercollision()//캐릭터와 몬스터의 충돌을 검사할 함수 캐릭터와 몬스터가 충돌하면 캐릭터는 충돌한 몬스터의 데미지 만큼 데미지를 받는다 . 
{
	POINT mypt;
	POINT dtpt;
	
	if (NowGameState != PLAYING)
	{
		return;
	}



	//캐릭터와 몬스터의 충돌
	for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
	{
		mypt = mycharacter->GetHitBoxPos();
		dtpt = (*itr)->HitBox_Pos();
		if ((mypt.x - 100 <= dtpt.x && mypt.x + 100 >= dtpt.x) && (mypt.y - 100 <= dtpt.y && mypt.y + 100 >= dtpt.y))//충돌검사를 하는 객체가 총알의 반경 200만큼의 네모영역에 있을때만 충돌검사를 한다.
		{
			if (isCollision(mypt, mycharacter->GetHitBoxRadious(), dtpt, (*itr)->HitBox_Radious()))
			{
				if ((*itr)->GetState() != BaseMonster::STATE::DEAD)
				{
					mycharacter->CharacterHit();
				}
			}
		}
	}


	//캐릭터와 아이템박스의 충돌
	for (list<MyChest*>::iterator itr = chestlist.begin(); itr != chestlist.end();)
	{
		mypt = mycharacter->GetHitBoxPos();
		dtpt = (*itr)->GetHitBox_Pos();
		if ((mypt.x - 50 <= dtpt.x && mypt.x + 50 >= dtpt.x) && (mypt.y - 50 <= dtpt.y && mypt.y + 50 >= dtpt.y))//충돌검사를 하는 객체가 총알의 반경 100만큼의 네모영역에 있을때만 충돌검사를 한다.
		{
			if (isCollision(mypt, mycharacter->GetHitBoxRadious(), dtpt, (*itr)->GetHitBox_Radious()))
			{
				mycharacter->GetItem((*itr)->GetKind());//부딫히면 캐릭터한테 해당 상자의 카인드를 받아서 넘겨준다.
				chestlist.erase(itr++);
			}
			else
			{
				itr++;
			}
		}
		else
		{
			itr++;
		}
		
		
	}
}


void GameMainController::DrawChest()//현재 만들어진 상자들을 그려준다.
{
	POINT pt;
	for (list<MyChest*>::iterator itr = chestlist.begin(); itr != chestlist.end(); itr++)
	{
		pt = (*itr)->GetChestPos();
		//pt.x = pt.x - (scrollIndex_X * 60);
		//pt.y = pt.y - (scrollIndex_Y * 60);
		if ((pt.x >= scroll_X && pt.x <= (scroll_X + crt.right)) && (pt.y >= scroll_Y && pt.y <= (scroll_Y + crt.bottom)))//현재 화면 안에 있을때만 그려준다.
		{
			pt.x = pt.x - scroll_X;//월드좌표를 화면에서의 위치로 변환해서 
			pt.y = pt.y - scroll_Y;
			(*itr)->ChestRender(pt.x, pt.y);//해당 위치에 그려준다.
		}
		
	}
	
}

void GameMainController::DrawBullet()
{
	for (list<BaseBullet*>::iterator itr = bulletlist.begin(); itr != bulletlist.end(); itr++)
	{
		(*itr)->BulletRender(scroll_X, scroll_Y);
	}
}


void GameMainController::DrawUI()
{
	TCHAR str[10];
	for (int i = 0; i < mycharacter->GetHP(); i++)
	{
		doublebuffer->DrawBackBufferBitmap(hWndMain, doublebuffer->MainBitmaps[MyDoubleBuffer::BITMAPS::HART], ( 10 + (i * 22)), 10, 20, 20, 0, 0, 11, 10);
	}
	int num = 0;
	for (multimap<BaseWeapon*, BOOL>::iterator itr = mycharacter->weaponlist.begin(); itr != mycharacter->weaponlist.end(); itr++)
	{
		(*itr).first->WeaponRender((10 + (num * 70)), 30, 0);
		wsprintf(str, "%d", (*itr).first->GetArmor());
		doublebuffer->DrawBackBufferWords(hWndMain, str, (50 + (num * 70)),  40, 20);
		num++;
	}


}

//CreateThread함수에서는 일반함수만 호출할 수 있어서 스레드 함수를 static으로 만들어 주고 파라미터로 객체 자신을 주어서 스레드 함수 안에서 객체함수를 호출 할 수 있도록 해주었다.
DWORD WINAPI GameMainController::MainThreadFunc(LPVOID p)//게임의 모든 동작들을 관리할 스레드 함수//
{
	WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::MAINTHREAD], INFINITE);
	while (TRUE)
	{
		if (((GameMainController*)p)->NowGameState==GAMESTATE::GAMECLEAR)
		{
			return 0;
		}
		if (((GameMainController*)p)->NowGameState == GAMESTATE::GAMEOVER)
		{
			WaitForSingleObject(((GameMainController*)p)->hEvent[GameMainController::MAINTHREADS::MAINTHREAD], INFINITE);
		}
		((GameMainController*)p)->MoveThreadLoop();
		((GameMainController*)p)->BulletsMove();
		((GameMainController*)p)->Charactercollision();
		((GameMainController*)p)->DeleteMyObject();
		((GameMainController*)p)->ScreenRender();
		Sleep(20);
	}

	return 0;
}

void GameMainController::UpMove(POINT pos, int speed)
{
	POINT cantemp;
	POINT mytemp = pos;
	

	cantemp.x = scroll_X;//현재 카메라의 좌표와
	cantemp.y = scroll_Y;
	cantemp.y -= speed;//움직일 방향으로 값을 변경해준다
	mytemp.y -= speed;

	if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp), mycharacter->GetHitBoxRadious()) == FALSE)//캐릭터가 움직일 위치에 충돌이 발생하지 않았고
	{

		if ((scroll_Y != 0) && ((mytemp.y) - scroll_Y <= crt.bottom / 2))//이 경우가 참일때는 카메라와 캐릭터모두 움직인다.
		{
			scroll_Y = cantemp.y;
			if (scroll_Y / 60 != scrollIndex_Y)
			{
				scrollIndex_Y = scroll_Y / 60;
				LoadPlayMap();
			}
		}
		mycharacter->UpMove();
	}
}
void GameMainController::DownMove(POINT pos, int speed)
{
	POINT camtemp;
	POINT mytemp = pos;
	

	camtemp.x = scroll_X;
	camtemp.y = scroll_Y;
	camtemp.y += speed;
	mytemp.y += speed;


	if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp), mycharacter->GetHitBoxRadious()) == FALSE)
	{
		if (((scroll_Y + crt.bottom <= WorldMapSize.y) /*|| camtemp3.y == 120*/) && ((mytemp.y) - scroll_Y > crt.bottom / 2))
		{

			scroll_Y = camtemp.y;
			if (scroll_Y / 60 != scrollIndex_Y)
			{
				scrollIndex_Y = scroll_Y / 60;
				LoadPlayMap();
			}
		}
		mycharacter->DownMove();
	}
}
void GameMainController::RightMove(POINT pos, int speed)
{
	POINT camtemp;
	POINT mytemp = pos;
	
	camtemp.x = scroll_X;
	camtemp.y = scroll_Y;
	camtemp.x += speed;
	mytemp.x += speed;
	if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp), mycharacter->GetHitBoxRadious()) == FALSE)
	{
		if ((scroll_X + crt.right <= WorldMapSize.x) && (((mytemp.x) - scroll_X) > (crt.right / 2)))
		{
			scroll_X = camtemp.x;
			if (scroll_X / 60 != scrollIndex_X)
			{
				scrollIndex_X = scroll_X / 60;
				LoadPlayMap();
			}
		}
		mycharacter->RightMove();
	}
}
void GameMainController::LeftMove(POINT pos, int speed)
{
	POINT camtemp;
	POINT mytemp = pos;
	
	camtemp.x = scroll_X;
	camtemp.y = scroll_Y;
	camtemp.x -= speed;
	mytemp.x -= speed;
	if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp), mycharacter->GetHitBoxRadious()) == FALSE)
	{
		if ((scroll_X != 0) && ((mytemp.x) - scroll_X < crt.right / 2))
		{
			scroll_X = camtemp.x;
			if (scroll_X / 60 != scrollIndex_X)
			{
				scrollIndex_X = scroll_X / 60;
				LoadPlayMap();
			}
		}
		mycharacter->LeftMove();
	}
}

void GameMainController::MoveThreadLoop()
{
	POINT camtemp1, camtemp2, camtemp3, camtemp4;
	POINT mytemp1, mytemp2, mytemp3, mytemp4;
	if (NowGameState != PLAYING)
	{
		return;
	}
	
	int speed = mycharacter->GetSpeed();
	
	if (mycharacter->GetState() != MyCharacter::STATE::HURT && mycharacter->GetState() != MyCharacter::STATE::DEAD /*&& mycharacter->GetState() != MyCharacter::STATE::DASH*/)//캐릭터가 총에 맞아서 경직&무적 상태가 아닐때만 움직인다.
	{
		if (CharacterDash())
		{
			return;
		}
		if (KeyBuffer[UP] == TRUE)//w가 눌렸을때
		{
			if (mycharacter->GetState() != MyCharacter::STATE::WALKING)
			{
				mycharacter->SetState(MyCharacter::STATE::WALKING);
			}
			UpMove(mycharacter->GetPos(), mycharacter->GetSpeed());
		}
		if (KeyBuffer[DOWN] == TRUE)
		{
			if (mycharacter->GetState() != MyCharacter::STATE::WALKING )
			{
				mycharacter->SetState(MyCharacter::STATE::WALKING);
			}
			DownMove(mycharacter->GetPos(), mycharacter->GetSpeed());
		}
		if (KeyBuffer[RIGHT] == TRUE)
		{
			if (mycharacter->GetState() != MyCharacter::STATE::WALKING )
			{
				mycharacter->SetState(MyCharacter::STATE::WALKING);
			}
			RightMove(mycharacter->GetPos(), mycharacter->GetSpeed());
		}
		if (KeyBuffer[LEFT] == TRUE)
		{
			if (mycharacter->GetState() != MyCharacter::STATE::WALKING)
			{
				mycharacter->SetState(MyCharacter::STATE::WALKING);
			}
			LeftMove(mycharacter->GetPos(), mycharacter->GetSpeed());
		}

		if (KeyBuffer[NUM1])
		{
			mycharacter->SetWeapon(1);
		}
		if (KeyBuffer[NUM2])
		{
			mycharacter->SetWeapon(2);
		}
		if (GetKeyState(VK_F1) & 0x8000)
		{
			if (Debuging)
			{
				Debuging = FALSE;
			}
			else
			{
				Debuging = TRUE;
			}
		}

		int i = 0;
		for (i = 0; i < 4; i++)
		{
			if (KeyBuffer[i] == TRUE)
			{
				break;
			}
		}
		if (i == 4)
		{
			if (mycharacter->GetState() != MyCharacter::STATE::IDLE)
			{
				mycharacter->SetState(MyCharacter::STATE::IDLE);
				mycharacter->SetBitNum(0);
			}

		}
	}
}

//void GameMainController::MoveThreadLoop()
//{
//	POINT camtemp1, camtemp2, camtemp3, camtemp4;
//	POINT mytemp1, mytemp2, mytemp3, mytemp4;
//
//	int speed = mycharacter->GetSpeed();
//	if (mycharacter->GetState() != MyCharacter::STATE::HURT && mycharacter->GetState() != MyCharacter::STATE::DEAD)//캐릭터가 총에 맞아서 경직&무적 상태가 아닐때만 움직인다.
//	{
//		if (KeyBuffer[UP] == TRUE)//w가 눌렸을때
//		{
//
//
//		}
//		if (KeyBuffer[DOWN] == TRUE)
//		{
//
//			if (mycharacter->GetState() != MyCharacter::STATE::WALKING)
//			{
//				mycharacter->SetState(MyCharacter::STATE::WALKING);
//			}
//
//			camtemp3.x = scroll_X;
//			camtemp3.y = scroll_Y;
//			mytemp3 = mycharacter->GetPos();
//			camtemp3.y += speed;
//			mytemp3.y += speed;
//
//
//			if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp3), mycharacter->GetHitBoxRadious()) == FALSE)
//			{
//				if (((scroll_Y + crt.bottom <= WorldMapSize.y) /*|| camtemp3.y == 120*/) && ((mytemp3.y) - scroll_Y > crt.bottom / 2))
//				{
//
//					scroll_Y = camtemp3.y;
//					if (scroll_Y / 60 != scrollIndex_Y)
//					{
//						scrollIndex_Y = scroll_Y / 60;
//						LoadPlayMap();
//					}
//				}
//				mycharacter->DownMove();
//			}
//
//		}
//
//		if (KeyBuffer[RIGHT] == TRUE)
//		{
//			if (mycharacter->GetState() != MyCharacter::STATE::WALKING || mycharacter->GetDirection() != MyCharacter::DIRECTION::RIGHT)
//			{
//				mycharacter->SetState(MyCharacter::STATE::WALKING);
//			}
//			camtemp4.x = scroll_X;
//			camtemp4.y = scroll_Y;
//			mytemp4 = mycharacter->GetPos();
//			camtemp4.x += speed;
//			mytemp4.x += speed;
//			if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp4), mycharacter->GetHitBoxRadious()) == FALSE)
//			{
//				if ((scroll_X + crt.right <= WorldMapSize.x) && (((mytemp4.x) - scroll_X) > (crt.right / 2)))
//				{
//					scroll_X = camtemp4.x;
//					if (scroll_X / 60 != scrollIndex_X)
//					{
//						scrollIndex_X = scroll_X / 60;
//						LoadPlayMap();
//					}
//
//
//
//				}
//				mycharacter->RightMove();
//			}
//		}
//		if (KeyBuffer[LEFT] == TRUE)
//		{
//			if (mycharacter->GetState() != MyCharacter::STATE::WALKING || mycharacter->GetDirection() != MyCharacter::DIRECTION::LEFT)
//			{
//				mycharacter->SetState(MyCharacter::STATE::WALKING);
//			}
//
//
//			camtemp2.x = scroll_X;
//			camtemp2.y = scroll_Y;
//			mytemp2 = mycharacter->GetPos();
//			camtemp2.x -= speed;
//			mytemp2.x -= speed;
//			if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp2), mycharacter->GetHitBoxRadious()) == FALSE)
//			{
//				if ((scroll_X != 0) && ((mytemp2.x) - scroll_X < crt.right / 2))
//				{
//
//					scroll_X = camtemp2.x;
//					if (scroll_X / 60 != scrollIndex_X)
//					{
//						scrollIndex_X = scroll_X / 60;
//						LoadPlayMap();
//					}
//
//
//
//				}
//				mycharacter->LeftMove();
//			}
//		}
//
//		if (KeyBuffer[NUM1])
//		{
//			mycharacter->SetWeapon(1);
//		}
//		if (KeyBuffer[NUM2])
//		{
//			mycharacter->SetWeapon(2);
//		}
//		if (GetKeyState(VK_F1) & 0x8000)
//		{
//			if (Debuging)
//			{
//				Debuging = FALSE;
//			}
//			else
//			{
//				Debuging = TRUE;
//			}
//		}
//
//		int i = 0;
//		for (i = 0; i < 4; i++)
//		{
//			if (KeyBuffer[i] == TRUE)
//			{
//				break;
//			}
//		}
//		if (i == 4)
//		{
//			if (mycharacter->GetState() != MyCharacter::STATE::IDLE)
//			{
//				mycharacter->SetState(MyCharacter::STATE::IDLE);
//				mycharacter->SetBitNum(0);
//			}
//
//		}
//	}
//
//
//
//}


void GameMainController::MouseMove(LPARAM lParam)
{
	if (NowStage == STAGE::NOTHING)
	{
		return;
	}
	if (NowGameState == GAMESTATE::GAMECLEAR || NowGameState == GAMESTATE::GAMEOVER)
	{
		return;
	}
	mousepoint.x = LOWORD(lParam);
	mousepoint.y = HIWORD(lParam);
	POINT pt = mycharacter->GetPos();
	if (pt.x - (scrollIndex_X * 60) <= mousepoint.x)
	{
		mycharacter->SetDirection(MyCharacter::DIRECTION::RIGHT);
	}
	else
	{
		mycharacter->SetDirection(MyCharacter::DIRECTION::LEFT);
	}
}


void  GameMainController::DrawAim()
{
	doublebuffer->DrawBackBufferBitmap(hWndMain, doublebuffer->MainBitmaps[MyDoubleBuffer::BITMAPS::CURSOR], mousepoint.x, mousepoint.y, 16, 16, 0, 0, 16, 16);
}

void GameMainController::MouseRClick()//마우스 오른클릭을 하면 키보드의 입력 방향으로 대쉬를 하도록 해준다.
{
	if (mycharacter->GetKind() == MyCharacter::CHARECTERS::FISH)
	{
		soundplayer->PlayEffect(SoundPlayer::SOUNDS::FISHSKILL);
		if (mycharacter->GetState() != MyCharacter::STATE::DASH)
		{
			mycharacter->SetState(MyCharacter::STATE::DASH);
		}
		else
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			DashBuffer[i] = FALSE;
		}
		if (KeyBuffer[UP] == TRUE)
		{
			DashBuffer[UP] = TRUE;
		}
		if (KeyBuffer[RIGHT] == TRUE)
		{
			DashBuffer[RIGHT] = TRUE;
		}
		if (KeyBuffer[DOWN] == TRUE)
		{
			DashBuffer[DOWN] = TRUE;
		}
		if (KeyBuffer[LEFT] == TRUE)
		{
			DashBuffer[LEFT] = TRUE;
		}
	}
	else if (mycharacter->GetKind() == MyCharacter::CHARECTERS::CRYSTAL)
	{
		if (mycharacter->GetState() != MyCharacter::STATE::DEFENSE)
		{
			mycharacter->SetState(MyCharacter::STATE::DEFENSE);
		}
		else
		{
			return;
		}
		soundplayer->PlayEffect(SoundPlayer::SOUNDS::CRYSTALCKILL);
		mycharacter->Defense();


	}
	
	
}



BOOL GameMainController::CharacterDash()//상태가 대쉬여서 대쉬를 하면 TRUE를 리턴해서 움직이지 않도록 해준다.
{
	if (mycharacter->GetState() != MyCharacter::STATE::DASH)
	{
		return FALSE;
	}
	if (dashcount >= 20)
	{
		dashcount = 0;
		mycharacter->SetState(MyCharacter::STATE::IDLE);
		return FALSE;
	}
	POINT temppt;
	POINT tempcam;
	if (DashBuffer[UP] == TRUE)
	{
		UpMove(mycharacter->GetPos(), mycharacter->GetSpeed() + 10);
		//dashcount++;
	}
	if (DashBuffer[DOWN] == TRUE)
	{
		DownMove(mycharacter->GetPos(), mycharacter->GetSpeed() + 10);
		//dashcount++;
	}
	if (DashBuffer[RIGHT] == TRUE)
	{
		RightMove(mycharacter->GetPos(), mycharacter->GetSpeed() + 10);
		//dashcount++;
	}
	if (DashBuffer[LEFT] == TRUE)
	{
		LeftMove(mycharacter->GetPos(), mycharacter->GetSpeed() + 10);
		
	}
	dashcount++;
	return TRUE;
}

POINT GameMainController::GetMousePos()
{
	return mousepoint;
}

void GameMainController::MouseClick(LPARAM lParam)
{
	/*if (NowStage == STAGE::NOTHING)
	{

		StageSetting();
		ScreenRender();
		return;
	}*/
	POINT pt;
	if (NowGameState == GAMESTATE::CSELLECT)
	{
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		for (int i = 0; i < MyCharacter::CHARECTERS::CMAX; i++)
		{
			if (PtInRect(&CharacterSelectPos[i], pt))
			{
				if (i == MyCharacter::CHARECTERS::FISH)
				{
					SelectCharacter = MyCharacter::CHARECTERS::FISH;
					soundplayer->PlayEffect(SoundPlayer::SOUNDS::CLICK);
					ScreenRender();
					return;
				}
				else if (i == MyCharacter::CHARECTERS::CRYSTAL)
				{
					SelectCharacter = MyCharacter::CHARECTERS::CRYSTAL;
					soundplayer->PlayEffect(SoundPlayer::SOUNDS::CLICK);
					ScreenRender();
					return;
				}
			}
			
		}
		return;
	}
	if (NowGameState == GAMESTATE::GAMEOVER)
	{
		StageSetting();
		ScreenRender();
	}
	if (NowGameState == GAMESTATE::GAMECLEAR)
	{
		return;
	}
	/*if(GameOver)
	{
		GameOver = FALSE;
		StageSetting();
		ScreenRender();
	}*/
	/*if (GameClear)
	{
		return;
	}*/
	
	if (mycharacter->NowShootable() && mycharacter->GetArmor() != 0)//총알을 쏠 수 있는 상태고 총알이 제로가 아닐때
	{
		
		//이미 마우스무브에서 받아놓은 위치로 캐릭터에서 가진 총의 총알 정보로 총알을 생성해준다.
		
		if (mycharacter->GetNowWeapon() == BaseWeapon::WEAPONS::ROCKET)
		{
			soundplayer->PlayEffect(SoundPlayer::SOUNDS::ROCKET);
		}
		else
		{
			soundplayer->PlayEffect(SoundPlayer::SOUNDS::PISTOL);
		}
		POINT pt1 = mycharacter->GetBulletPos();//마우스가 클릭되면 현재 화면에서의 캐릭터의 위치와
		
		pt1.x = pt1.x - (scroll_X);
		pt1.y = pt1.y - (scroll_Y);

		POINT pt2 = mousepoint;//마우스 포인트는 화면 안에서의 위치

		float angle = GetAngle(pt1, pt2);//두 점의 각도를 구해서 
		mycharacter->ShootBullet(angle);
		//bulletlist.push_back(mycharacter->ShootBullet(angle));//해당 방향으로 총알을 발사해 준다.
	}
}


void GameMainController::ScreenRender()//캐릭터의 월드맵 위치에서 부터 찍어주는 함수// 움직이면 캐릭터의 월드에서의 위치가 움직이고 또 거기에서부터 읽어와서 화면에 그려준다.
{
	int info = -1;
	POINT rpos;
	int m1 = 0, m2 = 0;
	if (NowGameState == GAMESTATE::GAMECLEAR || NowGameState == GAMESTATE::GAMEOVER)
	{
		return;
	}
	if (NowStage == STAGE::NOTHING)
	{
		doublebuffer->DrawRect(hWndMain, 0, 0, crt.right, crt.bottom, RGB(0, 0, 0));
		doublebuffer->DrawRect(hWndMain, 0, crt.bottom - 200, crt.right, 200, RGB(38, 57, 83));
		doublebuffer->DrawBackBufferBitmap(hWndMain, startscreen, 100, 0, crt.right-200, crt.bottom-200, 0, 0, 620, 248);
		doublebuffer->DrawBackBufferBitmap(hWndMain, doublebuffer->MainBitmaps[MyDoubleBuffer::BITMAPS::ENTERICON], crt.right - 80, crt.bottom - 40, 50, 20, 0, 0, 50, 20);
		for (int i = 0; i < MyCharacter::CHARECTERS::CMAX; i++)
		{
			if (SelectCharacter == i)
			{
				doublebuffer->DrawBackBufferBitmap(hWndMain, CharacterCard, i * 151, crt.bottom - 200, 150, 200, (i * 200) + 100, 0, 100, 100);
			}
			else
			{
				doublebuffer->DrawBackBufferBitmap(hWndMain, CharacterCard, i * 151, crt.bottom - 200, 150, 200, (i * 200), 0, 100, 100);
			}
			
		}
		//doublebuffer->DrawBackBufferBitmap(hWndMain, CharacterCard, CharacterSelectPos[0].left, CharacterSelectPos[0].top, CharacterSelectPos[0].right-CharacterSelectPos[0].left, CharacterSelectPos[0].bottom - CharacterSelectPos[0].top, 0, 0, 100, 100);
		//doublebuffer->DrawBackBufferBitmap(hWndMain, CharacterCard, CharacterSelectPos[1].left, CharacterSelectPos[1].top, CharacterSelectPos[1].right - CharacterSelectPos[1].left, CharacterSelectPos[1].bottom - CharacterSelectPos[1].top, 200, 0, 100, 100);
	}
	else
	{
		doublebuffer->DrawBitBlt(hWndMain, StageBit[NowStage], 0, 0, crt.right, crt.bottom, scroll_X, scroll_Y);
		
		DrawChest();
		DrawCharacter();
		DrawEnemy();
		DrawBullet();
		DrawUI();
		DrawAim();
	}
	//배경은 빗블릿으로 스크롤 영역에서부터 화면 크기까지 그려준다.
	
	
	InvalidateRect(hWndMain, NULL, FALSE);
}

void GameMainController::GameIsOver()//게임 오버가 되면 모든 객체들을 전부다 없애주고 스레드들을 모두 없애주고 
{
	//GameOver = TRUE;
	NowGameState = GAMESTATE::GAMEOVER;
	mycharacter->IsDestroy = TRUE;
	maxmonster = 0;
	NowStage = 0;
	delete mycharacter;
	mycharacter = NULL;
	for (int i = 0; i < MAINTHREADMAX; i++)
	{
		hThread[i] = NULL;
	}
	/*for (int i = 0; i < MTHREADMAX; i++)
	{
		MonsterhThread[i] = NULL;
	}*/
	RefClear();
	BITMAP bit;
	GetObject(GameOverBit, sizeof(BITMAP), &bit);
	doublebuffer->DrawBackBufferBitmap(hWndMain, GameOverBit, 0, 0, crt.right, crt.bottom, 0, 0, bit.bmWidth, bit.bmHeight);
	InvalidateRect(hWndMain, NULL, FALSE);
}

void GameMainController::RefClear()
{
	for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
	{
		delete (*itr);
	}
	for (list<BaseBullet*>::iterator itr = bulletlist.begin(); itr != bulletlist.end(); itr++)
	{
		delete (*itr);
	}
	for (list<MyChest*>::iterator itr = chestlist.begin(); itr != chestlist.end(); itr++)
	{
		delete (*itr);
	}
	monsterlist.clear();
	bulletlist.clear();
	chestlist.clear();
	spawnlist.clear();
}


void GameMainController::GameIsClear()
{
	//GameClear = TRUE;
	NowGameState = GAMESTATE::GAMECLEAR;
	mycharacter->IsDestroy = TRUE;
	maxmonster = 0;
	NowStage = 0;
	delete mycharacter;
	mycharacter = NULL;
	for (int i = 0; i < MAINTHREADMAX; i++)
	{
		hThread[i] = NULL;
	}
	/*for (int i = 0; i < MTHREADMAX; i++)
	{
		MonsterhThread[i] = NULL;
	}*/
	RefClear();
	BITMAP bit;
	GetObject(GameClearBit, sizeof(BITMAP), &bit);
	doublebuffer->DrawBackBufferBitmap(hWndMain, GameClearBit, 100, 0, crt.right - 200, crt.bottom, 0, 0, bit.bmWidth - 100, bit.bmHeight);
	InvalidateRect(hWndMain, NULL, FALSE);
}

void GameMainController::DrawCharacter()
{
	POINT mypos;
	mypos = mycharacter->GetPos();
	mycharacter->DrawCharacter(mypos.x - (scroll_X), mypos.y - (scroll_Y));//월드위치를 활성화면위치로 바꿔준다. 


}

void GameMainController::DrawEnemy()
{
	POINT monsterpt;
	for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
	{
		if (!(*itr)->isDestroy)
		{
			monsterpt = (*itr)->HitBox_Pos();
			if ((monsterpt.x >= scroll_X && monsterpt.x <= (scroll_X + crt.right)) && (monsterpt.y >= scroll_Y && monsterpt.y <= (scroll_Y + crt.bottom)))//현재 화면 안에 있을때만 그려준다.
			{
				(*itr)->DrawMonster();
			}
		}
	}
}



POINT GameMainController::GetScrollindex()
{
	POINT pt;
	pt.x = scrollIndex_X;
	pt.y = scrollIndex_Y;
	return pt;
}


POINT GameMainController::GetScrollPos()
{
	POINT pt;
	pt.x = scroll_X;
	pt.y = scroll_Y;
	return pt;
}



