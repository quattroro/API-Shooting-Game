#include "GameMainController.h"

GameMainController* GameMainController::_Instance = NULL;
GameMainController* GameMainController::ControllerInstance()//�̱���
{
	if (_Instance == NULL)
	{
		_Instance = new GameMainController();
	}
	return _Instance;
}
CRITICAL_SECTION GameMainController::crit;

GameMainController::GameMainController()//ó�� ����ѷ��� ����� ���� �ʱ�ȭ���� ���� �Լ�
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

void GameMainController::InitSetting(HINSTANCE hInst, HWND MhWnd, MyDoubleBuffer* doublebuffer)//��Ʈ�ѷ����� ����� ��Ʈ�ʵ��� �ҷ����� ���� �۾��� ���� �Լ�
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
		hThread[MAINTHREAD] = CreateThread(NULL, 0, MainThreadFunc, this, 0, NULL);//Ű���� �Է��� ���� ������
	}
	if (hThread[MAINTHREADS::MOVING] == NULL)//������ �������� ����� ������
	{
		hThread[MAINTHREADS::MOVING] = CreateThread(NULL, 0, MonsterMovingThreadFunc, this, 0, NULL);
	}
	if (hThread[MAINTHREADS::MOTION] == NULL)//������ ��Ǻ����� ����� ������
	{
		hThread[MAINTHREADS::MOTION] = CreateThread(NULL, 0, MonsterMotionThreadFunc, this, 0, NULL);
	}

	if (hThread[MAINTHREADS::SHOOT] == NULL)//������ �Ѿ� �߻縦 ����� ������
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

BOOL GameMainController::SetScroll()//ó�� ĳ������ ���� ��ǥ���� ȭ��ũ���� ���ݾ� ��ŭ �� ���� ��ũ�Ѱ�(���忡�� ī�޶�����ġ)���� ���� ���ش�.
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

void GameMainController::StageSetting()//���������� ó�� �����ɶ� �ʿ��ִ� �ʱ�ȭ �������� �̸� �ҷ����� ���� �Լ�
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
	GetObject(StageBit[NowStage], sizeof(BITMAP), &bit);//���� ���������� ��Ʈ���� ������ �޾ƿͼ�
	WorldMapSize.x = bit.bmWidth;//�ʺ�� ũ�⸦ ������� �������� ������ �ش�.
	WorldMapSize.y = bit.bmHeight;

	//spawnlist.clear();
	for (int y = 0; y < MapsizeY; y++)
	{
		for (int x = 0; x < MapsizeX; x++)
		{
			info = WordMapInfo[NowStage][y][x];
			if (info == PSPAWN)
			{
				MyPosWorld.x = x * 60;//ĳ������ ���� ��ǥ�� ���Ѵ�
				MyPosWorld.y = y * 60;
				SetScroll();
				
			}
			if (info == ESPAWN)
			{
				maxmonster++;
			}
		}
	}
	

	//����ʿ����� ������ǥ (ĳ����ȭ�鳻�� ��� �浹ó���� ���� ����ʿ����� ��ǥ�� �̿��� ĳ���Ͱ� ������������ ĳ������ �����ӿ����� ī�޶� ���� �����δ�. )
	POINT mypos;
	mypos.x = MyPosWorld.x;
	mypos.y = MyPosWorld.y;
	
	if (mycharacter == NULL)
	{
		if (SelectCharacter == MyCharacter::CHARECTERS::FISH)
		{
			mycharacter = new CharacterFish(this, hWndMain, M_hInst, mypos);//���� ���� ��ǥ�� ĳ���͸� ����� �ش�.
		}
		else if (SelectCharacter == MyCharacter::CHARECTERS::CRYSTAL)
		{
			mycharacter = new CharacterCrystal(this, hWndMain, M_hInst, mypos);//���� ���� ��ǥ�� ĳ���͸� ����� �ش�.
		}

	}
	else
	{
		mycharacter->SetPos(mypos);//���� �̹� ĳ���Ͱ� ������� ������(���������� ������ ���� ���������� �Ѿ����) ĳ������ ��ġ�� �ʱ�ȭ ���ش�.
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
	LoadPlayMap();//���� �ε��� �ش�.
	//soundplayer->PlayBGM();
}

//������ ���� ������
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
//��� ���� ������
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
	//�������� �������� ������ �������� ��ġ�� ��и�(������ �����϶��� )

	float radian = atan2(ry, rx);//��ũź��Ʈ2 �Լ��� ���� -PI~PI ������ ������ ������ ������ 1,2��и��� �ݽð�������� ����� 3,4 ��и��� �ð�������� ����� ������ �ȴ�. 
	float degree = radian * (180.0f / 3.141592f);//���� ���� ȣ�������� ��ȯ����
	return degree;
}




void GameMainController::LoadPlayMap()//ĳ���Ͱ� �����̸鼭 �迭������ �ε������� ��ȭ�ϸ� �׶����� ���ο� ������ ���� �ٽ� �о�ͼ� ���͸� �������ش�.
{
	//srand((unsigned)time(NULL));
	int info;
	POINT pt;
	BOOL flag = true;
	scrollIndex_X= scroll_X / 60;//���� ��ũ�� ���� �̿��� �迭���������� �ε����� �˾Ƴ���
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

void GameMainController::MonsterSpawn(int x, int y)//Ư�� ��ġ�� ���� ������������ ���� �� �ִ� ���� �߿� �������� �̾Ƽ� �������� 
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


BOOL GameMainController::DetectMapCollision(POINT pos, int radious)//�ϳ����� �浹�� �ϸ� �ٷ� true�� �����Ѵ�.//�߽����� �������� �־��ָ� ���� Ȱ������ �ȿ��� �浹�� �˻��Ѵ�.//������ ��ǥ�� Ȱ������ �ȿ��� ���° �ε��� ���� �˾Ƴ��� �� �ε����� ���μ��� 2ĭ�� 4*4�� ������ ������ ���� �׸��� �浹�˻縦 �Ѵ�.
{
	POINT index;
	TCHAR str[255];
	int posx;
	int posy;
	POINT rpos;
	POINT cpos = pos;
	
	index.x = (pos.x  / 60) - 1;//������ ��ǥ�� Ȱ������ �ȿ��� ���° �ε��� ���� �˾Ƴ��� �� �ε����� ���μ��� 1ĭ�� 3*3�� ������ ������ ���� �׸��� �浹�˻縦 �Ѵ�.
	index.y = (pos.y  / 60) - 1;
	
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			//if (PlayMapInfo[index.y + y][index.x + x] != BLANK)
			if (WordMapInfo[NowStage][index.y + y][index.x + x] != BLANK && WordMapInfo[NowStage][index.y + y][index.x + x] != PSPAWN && WordMapInfo[NowStage][index.y + y][index.x + x] != ESPAWN)
			{
				
				rpos.x = ((index.x + x) * 60);//���忡���� ���� ��ǥ�� ã�Ƽ� �浹 �˻縦 �Ѵ�.
				rpos.y = ((index.y + y) * 60);

				//������ �浹�� �簢���� ���� �浹�� �˻��Ѵ�. �ٸ� �浹���� ���� ���� �浹�� ����
				if (isMapCollision(rpos, 60, cpos, radious) == TRUE)
				{
					
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL GameMainController::isMapCollision(POINT rectpos, int size, POINT cpos/*�߽���*/, int radious)//�ʰ� �ٸ� ��ü���� �浹�� �˻��� �Լ� �浹������ true �ƴϸ� false�� ����
{
	TCHAR str[255];
	BOOL flag1 = FALSE, flag2 = FALSE;
	if (((cpos.x + radious) <= rectpos.x) || ((cpos.x - radious) >= (rectpos.x + size)) || ((cpos.y + radious) <= rectpos.y) || ((cpos.y - radious) >= (rectpos.y + size)))//���� �׸𺸴� �����¿�� �� ���� ������ �浹���� ����
	{
		return FALSE;
	}
	//x�� �˻�
	// ���� �����ʿ� ������
	if (cpos.x >= rectpos.x)
	{
		if ((cpos.x - (rectpos.x + size)) <= radious)
		{
			flag1 = TRUE;
		}
	}
	if (cpos.x <= rectpos.x)// ���� ���ʿ� ������
	{
		if ((rectpos.x - cpos.x) <= radious)
		{
			flag1 = TRUE;
		}
	}
	//y�� �˻�
	//���� ���ʿ� ������
	if (cpos.y <= rectpos.y)
	{
		if ((rectpos.y - cpos.y) <= radious)
		{
			flag2 = TRUE;
		}
	}
	if (cpos.y >= rectpos.y)//���� �Ʒ��ʿ� ������
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
			//���� ����
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
	if (monsterlist.size() == 0 && spawnlist.size() == maxmonster)//���� ��� ���Ͱ� ��ȯ �Ǿ��� �� ���� ���� ��� �׾����� -> �������� Ŭ����
	{
		if (NowStage == STAGE2)//���� ��
		{
			GameIsClear();
			return;
		}
		NowStage++;
		StageSetting();
		LoadPlayMap();
	}

}

void GameMainController::BulletMove(BaseBullet* p)//���Ͱ� ��ũ�� ���� ������ �������� �������� �ʵ��� ���ش�.
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
	if (PtInRect(&playsize, pos) == FALSE)//�Ѿ��� Ȱ������ ������ ������ �Ѿ��� �������.
	{
		p->IsDestroy = true;
		return;
	}

	temppos.x = pos.x + cos(angle * (3.141592f / 180.0f)) * movespeed;//ȣ������ �������� �ٲ㼭 �Է����ش�.
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

BOOL GameMainController::BulletCollisionToRef(POINT pos, BaseBullet* p)//�浹�� �Ͼ�� ������ �ൿ�� ���ش�.
{
	POINT pt;
	//�Ѿ˰� ĳ������ �浹
	if (!(p->IsMyBullet()))//ĳ���Ͱ� �� �Ѿ��� �ƴҶ�
	{
		pt = mycharacter->GetHitBoxPos();
		if ((pos.x - 100 <= pt.x && pos.x + 100 >= pt.x) && (pos.y - 100 <= pt.y && pos.y + 100 >= pt.y))//�浹�˻縦 �ϴ� ��ü�� �Ѿ��� �ݰ� 200��ŭ�� �׸𿵿��� �������� �浹�˻縦 �Ѵ�.
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
	//ĳ���Ͱ� �� �Ѿ��϶�
	else//�Ѿ˰� ������ �浹
	{
		for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
		{
			pt = (*itr)->HitBox_Pos();
			if ((pos.x - 100 <= pt.x && pos.x + 100 >= pt.x) && (pos.y - 100 <= pt.y && pos.y + 100 >= pt.y))//�浹�˻縦 �ϴ� ��ü�� �Ѿ��� �ݰ� 200��ŭ�� �׸𿵿��� �������� �浹�˻縦 �Ѵ�.
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

float GameMainController::GetPointDistnace(POINT pt1, POINT pt2)//�� �� ���� �Ÿ��� ������ �����ִ� �Լ� ������ �Լ��� �Ϻη� ������� ����
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
void GameMainController::BulletsMove()//�Ѿ��� ��ġ�� ������ �޾ƿͼ� �浹�˻���� �ϰ� ó������ ���ش�.
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

void GameMainController::Charactercollision()//ĳ���Ϳ� ������ �浹�� �˻��� �Լ� ĳ���Ϳ� ���Ͱ� �浹�ϸ� ĳ���ʹ� �浹�� ������ ������ ��ŭ �������� �޴´� . 
{
	POINT mypt;
	POINT dtpt;
	
	if (NowGameState != PLAYING)
	{
		return;
	}



	//ĳ���Ϳ� ������ �浹
	for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
	{
		mypt = mycharacter->GetHitBoxPos();
		dtpt = (*itr)->HitBox_Pos();
		if ((mypt.x - 100 <= dtpt.x && mypt.x + 100 >= dtpt.x) && (mypt.y - 100 <= dtpt.y && mypt.y + 100 >= dtpt.y))//�浹�˻縦 �ϴ� ��ü�� �Ѿ��� �ݰ� 200��ŭ�� �׸𿵿��� �������� �浹�˻縦 �Ѵ�.
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


	//ĳ���Ϳ� �����۹ڽ��� �浹
	for (list<MyChest*>::iterator itr = chestlist.begin(); itr != chestlist.end();)
	{
		mypt = mycharacter->GetHitBoxPos();
		dtpt = (*itr)->GetHitBox_Pos();
		if ((mypt.x - 50 <= dtpt.x && mypt.x + 50 >= dtpt.x) && (mypt.y - 50 <= dtpt.y && mypt.y + 50 >= dtpt.y))//�浹�˻縦 �ϴ� ��ü�� �Ѿ��� �ݰ� 100��ŭ�� �׸𿵿��� �������� �浹�˻縦 �Ѵ�.
		{
			if (isCollision(mypt, mycharacter->GetHitBoxRadious(), dtpt, (*itr)->GetHitBox_Radious()))
			{
				mycharacter->GetItem((*itr)->GetKind());//�΋H���� ĳ�������� �ش� ������ ī�ε带 �޾Ƽ� �Ѱ��ش�.
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


void GameMainController::DrawChest()//���� ������� ���ڵ��� �׷��ش�.
{
	POINT pt;
	for (list<MyChest*>::iterator itr = chestlist.begin(); itr != chestlist.end(); itr++)
	{
		pt = (*itr)->GetChestPos();
		//pt.x = pt.x - (scrollIndex_X * 60);
		//pt.y = pt.y - (scrollIndex_Y * 60);
		if ((pt.x >= scroll_X && pt.x <= (scroll_X + crt.right)) && (pt.y >= scroll_Y && pt.y <= (scroll_Y + crt.bottom)))//���� ȭ�� �ȿ� �������� �׷��ش�.
		{
			pt.x = pt.x - scroll_X;//������ǥ�� ȭ�鿡���� ��ġ�� ��ȯ�ؼ� 
			pt.y = pt.y - scroll_Y;
			(*itr)->ChestRender(pt.x, pt.y);//�ش� ��ġ�� �׷��ش�.
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

//CreateThread�Լ������� �Ϲ��Լ��� ȣ���� �� �־ ������ �Լ��� static���� ����� �ְ� �Ķ���ͷ� ��ü �ڽ��� �־ ������ �Լ� �ȿ��� ��ü�Լ��� ȣ�� �� �� �ֵ��� ���־���.
DWORD WINAPI GameMainController::MainThreadFunc(LPVOID p)//������ ��� ���۵��� ������ ������ �Լ�//
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
	

	cantemp.x = scroll_X;//���� ī�޶��� ��ǥ��
	cantemp.y = scroll_Y;
	cantemp.y -= speed;//������ �������� ���� �������ش�
	mytemp.y -= speed;

	if (DetectMapCollision(mycharacter->GetHitBoxPos(mytemp), mycharacter->GetHitBoxRadious()) == FALSE)//ĳ���Ͱ� ������ ��ġ�� �浹�� �߻����� �ʾҰ�
	{

		if ((scroll_Y != 0) && ((mytemp.y) - scroll_Y <= crt.bottom / 2))//�� ��찡 ���϶��� ī�޶�� ĳ���͸�� �����δ�.
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
	
	if (mycharacter->GetState() != MyCharacter::STATE::HURT && mycharacter->GetState() != MyCharacter::STATE::DEAD /*&& mycharacter->GetState() != MyCharacter::STATE::DASH*/)//ĳ���Ͱ� �ѿ� �¾Ƽ� ����&���� ���°� �ƴҶ��� �����δ�.
	{
		if (CharacterDash())
		{
			return;
		}
		if (KeyBuffer[UP] == TRUE)//w�� ��������
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
//	if (mycharacter->GetState() != MyCharacter::STATE::HURT && mycharacter->GetState() != MyCharacter::STATE::DEAD)//ĳ���Ͱ� �ѿ� �¾Ƽ� ����&���� ���°� �ƴҶ��� �����δ�.
//	{
//		if (KeyBuffer[UP] == TRUE)//w�� ��������
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

void GameMainController::MouseRClick()//���콺 ����Ŭ���� �ϸ� Ű������ �Է� �������� �뽬�� �ϵ��� ���ش�.
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



BOOL GameMainController::CharacterDash()//���°� �뽬���� �뽬�� �ϸ� TRUE�� �����ؼ� �������� �ʵ��� ���ش�.
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
	
	if (mycharacter->NowShootable() && mycharacter->GetArmor() != 0)//�Ѿ��� �� �� �ִ� ���°� �Ѿ��� ���ΰ� �ƴҶ�
	{
		
		//�̹� ���콺���꿡�� �޾Ƴ��� ��ġ�� ĳ���Ϳ��� ���� ���� �Ѿ� ������ �Ѿ��� �������ش�.
		
		if (mycharacter->GetNowWeapon() == BaseWeapon::WEAPONS::ROCKET)
		{
			soundplayer->PlayEffect(SoundPlayer::SOUNDS::ROCKET);
		}
		else
		{
			soundplayer->PlayEffect(SoundPlayer::SOUNDS::PISTOL);
		}
		POINT pt1 = mycharacter->GetBulletPos();//���콺�� Ŭ���Ǹ� ���� ȭ�鿡���� ĳ������ ��ġ��
		
		pt1.x = pt1.x - (scroll_X);
		pt1.y = pt1.y - (scroll_Y);

		POINT pt2 = mousepoint;//���콺 ����Ʈ�� ȭ�� �ȿ����� ��ġ

		float angle = GetAngle(pt1, pt2);//�� ���� ������ ���ؼ� 
		mycharacter->ShootBullet(angle);
		//bulletlist.push_back(mycharacter->ShootBullet(angle));//�ش� �������� �Ѿ��� �߻��� �ش�.
	}
}


void GameMainController::ScreenRender()//ĳ������ ����� ��ġ���� ���� ����ִ� �Լ�// �����̸� ĳ������ ���忡���� ��ġ�� �����̰� �� �ű⿡������ �о�ͼ� ȭ�鿡 �׷��ش�.
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
	//����� �������� ��ũ�� ������������ ȭ�� ũ����� �׷��ش�.
	
	
	InvalidateRect(hWndMain, NULL, FALSE);
}

void GameMainController::GameIsOver()//���� ������ �Ǹ� ��� ��ü���� ���δ� �����ְ� ��������� ��� �����ְ� 
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
	mycharacter->DrawCharacter(mypos.x - (scroll_X), mypos.y - (scroll_Y));//������ġ�� Ȱ��ȭ����ġ�� �ٲ��ش�. 


}

void GameMainController::DrawEnemy()
{
	POINT monsterpt;
	for (list<BaseMonster*>::iterator itr = monsterlist.begin(); itr != monsterlist.end(); itr++)
	{
		if (!(*itr)->isDestroy)
		{
			monsterpt = (*itr)->HitBox_Pos();
			if ((monsterpt.x >= scroll_X && monsterpt.x <= (scroll_X + crt.right)) && (monsterpt.y >= scroll_Y && monsterpt.y <= (scroll_Y + crt.bottom)))//���� ȭ�� �ȿ� �������� �׷��ش�.
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



