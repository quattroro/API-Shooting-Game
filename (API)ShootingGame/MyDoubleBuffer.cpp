#include "MyDoubleBuffer.h"

#define PI 3.141592654

MyDoubleBuffer* MyDoubleBuffer::_Instance = NULL;
MyDoubleBuffer* MyDoubleBuffer::GetBufferInstance()
{
	if (_Instance == NULL)
	{
		_Instance = new MyDoubleBuffer();
	}
	return _Instance;
}


HBITMAP MyDoubleBuffer::GetRotatedBitmap(HWND MainhWnd, HBITMAP hBitmap, int source_x, int source_y, int dest_width, int dest_height, float angle, COLORREF bkColor)
{

	HDC hdc = GetDC(MainhWnd);

	HDC sourceDC = CreateCompatibleDC(hdc); // 회전할 비트맵 원본을 선택할 DC
	HDC destDC = CreateCompatibleDC(hdc); // 회전할 비트맵을 출력받을 DC
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, dest_width, dest_height); // 회전할 비트맵을 출력받을 메모리비트맵 핸들

	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap); // 회전할 비트맵 원본을 DC에 선택
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult); // 회전할 비트맵을 출력받을 DC에, 회전할 비트맵을 출력받을 메모리비트맵 선택

	HBRUSH hbrBack = CreateSolidBrush(bkColor); // 회전으로 인한, 공백영역을 칠할 브러시핸들 생성
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack); // 브러시핸들 선택
	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY); // 선택된 브러시로, 회전할 비트맵을 출력받을 DC에, 미리 색상을 채워 지움
	DeleteObject(SelectObject(destDC, hbrOld)); // 브러시 해제

	angle = (float)(angle * PI / 180); // 회전할 각도(360분법)를, 라디안단위의 각도로 변경
	float cosine = (float)cos(angle); // 회전이동변환 행렬에 들어갈 cos세타 값을 구함
	float sine = (float)sin(angle); // 회전이동변환 행렬에 들어갈 sin세타 값을 구함

	SetGraphicsMode(destDC, GM_ADVANCED); // 윈도우 좌표계의 회전을 위하여, 그래픽모드를 확장모드로 변경합니다.(요구사항:윈98,NT이상)

		// 회전이동변환 행렬을 정의.
		// (윈도우즈 API정복 개정판 2권 Page(1279) 참조.)
		// (게임 프로그래머를 위한 기초수학과 물리 Page(196) 참조.)

	XFORM xform; // 방정식을 표현하는 3행3열의 행렬 선언
	xform.eM11 = cosine; // 1행 1열 성분 설정 (회전성분)
	xform.eM12 = sine; // 1행 2열 성분 설정 (회전성분)
	xform.eM21 = -sine; // 2행 1열 성분 설정 (회전성분)
	xform.eM22 = cosine; // 2행 2열 성분 설정 (회전성분)
	xform.eDx = (FLOAT)dest_width / 2.0f; // 3행 1열 성분 설정 (X축 이동 성분)
	xform.eDy = (FLOAT)dest_height / 2.0f; // 3행 2열 성분 설정 (Y축 이동 성분)

		// 설정한 행렬로, 인수DC의 윈도우즈 좌표계를 변환.
		// 인수DC는 출력대상으로 메모리비트맵을 가지므로,
		// 선택하고 있는 메모리비트맵의 좌표체계가 회전된다.

	SetWorldTransform(destDC, &xform);

	// 회전된 메모리DC에, 회전할 이미지를 출력

	BitBlt(destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height, sourceDC, source_x, source_y, SRCCOPY);

	// 사용된 자원 해제

	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);
	DeleteObject(sourceDC);
	DeleteObject(destDC);
	ReleaseDC(MainhWnd, hdc);
	// 회전된 비트맵 핸들 리턴

	return hbmResult;
}

void MyDoubleBuffer::BufferInit(HINSTANCE hInst, HWND MainhWnd, int x, int y, int Width, int Height)//입력받은 위치값과 크기로 백버퍼를 만들어주고 초기화해주는 함수
{
	HDC hDC = GetDC(MainhWnd);
	HDC hMemDC = CreateCompatibleDC(hDC);
	BackBit = CreateCompatibleBitmap(hDC, Width, Height);//더블버퍼링의 백버퍼로 사용할 BackBit를 사용가능하게 해주고
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)BackBit);
	BufferClear(hMemDC);//BackBit는 전역변수 이므로 처음에는 검은색이기때문에 흰색으로 칠해준다
	

	MainBitmaps[CHARACTERFISH] = (HBITMAP)LoadImage(hInst, TEXT("Bitmaps/Characters/Character_Fish.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[MONSTERBANDIT] = (HBITMAP)LoadImage(hInst, TEXT("Monster_BanDit.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[MONSTERMAGGOT] = (HBITMAP)LoadImage(hInst, TEXT("Monster_Maggot.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[MONSTERMAGGOTNEST] = (HBITMAP)LoadImage(hInst, TEXT("Monster_MaggotNest.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[MONSTERSCORPION] = (HBITMAP)LoadImage(hInst, TEXT("Monster_Scorpion.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[GUNS] = (HBITMAP)LoadImage(hInst, TEXT("Guns.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[BULLETS] = (HBITMAP)LoadImage(hInst, TEXT("Bullets.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//MainBitmaps[AIM] = (HBITMAP)LoadImage(hInst, TEXT("AIM.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[CHESTS] = (HBITMAP)LoadImage(hInst, TEXT("ChestAndItems.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[HART] = (HBITMAP)LoadImage(hInst, TEXT("hart.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[MONSTERBIGBANDIT] = (HBITMAP)LoadImage(hInst, TEXT("Monster_BigBandit.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[STARTSCREEN] = (HBITMAP)LoadImage(hInst, TEXT("characterselect.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[CHARACTERCARD] = (HBITMAP)LoadImage(hInst, TEXT("Bitmaps/Characters/CharacterCard.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[CHARACTERCRYSTAL] = (HBITMAP)LoadImage(hInst, TEXT("Bitmaps/Characters/Character_Crystal.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[CURSOR] = (HBITMAP)LoadImage(hInst, TEXT("Bitmaps/Cursor/AIM.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MainBitmaps[ENTERICON] = (HBITMAP)LoadImage(hInst, TEXT("Bitmaps/Icons/enter.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SelectObject(hMemDC, hOldBit);
	DeleteDC(hMemDC);
	ReleaseDC(MainhWnd, hDC);

}

void MyDoubleBuffer::BufferClear(HDC hdc)//백버퍼를 초기화 해주는 함수
{
	BITMAP bit;//백버퍼의 크기를 받아올 변수
	HBITMAP hOldBit = (HBITMAP)SelectObject(hdc, (HBITMAP)BackBit);//백버퍼를 연결해서
	GetObject(BackBit, sizeof(BITMAP), &bit);
	RECT crt;
	SetRect(&crt, 0, 0, bit.bmWidth, bit.bmHeight);//
	FillRect(hdc, &crt, GetSysColorBrush(COLOR_WINDOW));//백버퍼의 크기만큼 흰색으로 칠해준다.
	SelectObject(hdc, hOldBit);
}

void MyDoubleBuffer::BufferClear(HWND hWndMain, int rgb)//백버퍼를 초기화 해주는 함수
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	BITMAP bit;//백버퍼의 크기를 받아올 변수
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//백버퍼를 연결해서
	HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hMemDC, MyBrush);
	GetObject(MyDoubleBuffer::BackBit, sizeof(BITMAP), &bit);
	RECT crt;
	SetRect(&crt, 0, 0, bit.bmWidth, bit.bmHeight);//
	FillRect(hMemDC, &crt, /*GetSysColorBrush(COLOR_WINDOW)*/MyBrush);//백버퍼의 크기만큼 흰색으로 칠해준다.
	SelectObject(hMemDC, OldBrush);
	DeleteObject(MyBrush);
	SelectObject(hMemDC, hOldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}


void MyDoubleBuffer::DrawBitBlt(HWND hWndMain, HBITMAP bitmap, int x, int y, int width, int height, int originx, int originy)
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HDC MemDC = CreateCompatibleDC(hMemDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//백버퍼와 그려줄 비트맵을 연결해준 다음네
	HBITMAP OldBit = (HBITMAP)SelectObject(MemDC, (HBITMAP)bitmap);


	BitBlt(hMemDC, x, y, width, height, MemDC, originx, originy, SRCCOPY);

	SelectObject(MemDC, OldBit);//뒷정리들
	SelectObject(hMemDC, hOldBit);
	DeleteDC(MemDC);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}


void MyDoubleBuffer::DrawBackBufferBitmap(HWND hWndMain, HBITMAP bitmap, int x, int y, int Width, int Height, int originx, int originy, int originwidth, int originheight, int Mode)//백버퍼에 받아온 비트맵을 받아온 위치와 크기로 그려주는 함수 
{
	HDC hdc = GetDC(hWndMain);//메인 dc를 만들어줌
	HDC hMemDC = CreateCompatibleDC(hdc);//백버퍼를 연결해줄 dc를 만들어줌
	HDC MemDC = CreateCompatibleDC(hMemDC);//비트맵을 연결해줄 dc를 만들어줌
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//백버퍼와 그려줄 비트맵을 연결해준 다음네
	HBITMAP OldBit = (HBITMAP)SelectObject(MemDC, (HBITMAP)bitmap);
	BITMAP bit;
	GetObject(bitmap, sizeof(BITMAP), &bit);//비트맵의 정보를 받아와서 
	//BufferClear(hMemDC);//그려주기 전에 백버퍼를 클리어시켜준다

	switch (Mode)
	{
	case DMOD_NOMAL:
		TransparentBlt(hMemDC, x, y, Width, Height, MemDC, originx, originy, originwidth, originheight, RGB(63, 73, 127));//그러고는 받아온 위치에 크기만큼 비트맵을 그려준다
		//TransparentBlt(hMemDC, x, y, Width, Height, MemDC, originx, originy, originwidth, originheight, RGB(63, 73, 127));//그러고는 받아온 위치에 크기만큼 비트맵을 그려준다
		break;

	case DMOD_FLIP_VERTICAL:
		//TransparentBlt(hMemDC, x, y, Width, Height, MemDC, 0, bit.bmHeight - 1, bit.bmWidth, (bit.bmHeight * -1), RGB(255, 0, 255));//그러고는 받아온 위치에 크기만큼 비트맵을 그려준다
		StretchBlt(hMemDC, x, y, Width, Height, MemDC, 0, bit.bmHeight - 1, bit.bmWidth, (bit.bmHeight * -1), SRCCOPY);//세로반전으로 그림을 그려줌 (비트맵의 맨 아래에서부터 -비트맵의 높이만큼 불러오면서 그려준다.)
		break;

	case DMOD_FLIP_HORIZONTAL:
		//TransparentBlt(hMemDC, x, y, Width, Height, MemDC, bit.bmWidth - 1, 0, (bit.bmWidth * -1), bit.bmHeight, RGB(255, 0, 255));//그러고는 받아온 위치에 크기만큼 비트맵을 그려준다
		StretchBlt(hMemDC, x, y, Width, Height, MemDC, bit.bmWidth - 1, 0, (bit.bmWidth * -1), bit.bmHeight, SRCCOPY);//가로반전으로 그림을 그려줌 (비트맵의 맨 오른쪽에서 부터 -비트맵의 너비만큼 불러오면서 그려준다.)
		break;
	}


	SelectObject(MemDC, OldBit);//뒷정리들
	SelectObject(hMemDC, hOldBit);
	DeleteDC(MemDC);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}

void MyDoubleBuffer::DrawBackBufferBitmap(HDC hMemDC, HBITMAP bitmap, int x, int y, int Width, int Height, int originx, int originy, int originwidth, int originheight)//백버퍼에 그려줄때 함수안에서 함수를 실행시킬때 hMemDC를 중복해서 만들면 비트맵 출력이 안되서 hMemDC를 매개변수로 받아오는 함수를 오버로딩함
{
	HDC MemDC = CreateCompatibleDC(hMemDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);
	HBITMAP OldBit = (HBITMAP)SelectObject(MemDC, (HBITMAP)bitmap);
	BITMAP bit;
	GetObject(bitmap, sizeof(BITMAP), &bit);
	TransparentBlt(hMemDC, x, y, Width, Height, MemDC, originx, originy, originwidth, originheight, RGB(63, 73, 127));
	SelectObject(MemDC, OldBit);
	SelectObject(hMemDC, hOldBit);
	DeleteDC(MemDC);
}

void MyDoubleBuffer::DrawScreen(HDC hdc)//메인화면의 dc를 받아와서 백버퍼의 내용을 화면으로 그려주는 함수
{
	HDC hMemDC = CreateCompatibleDC(hdc);//백버퍼를 연결할 DC
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, MyDoubleBuffer::BackBit);//만들어준 DC에 비트맵을 연결해줌 
	BITMAP bit;//비트맵의 정보를 가지고 올 변수
	GetObject(BackBit, sizeof(BITMAP), &bit);//백버퍼의 정보를 가지고 오고
	BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, hMemDC, 0, 0, SRCCOPY);//백버퍼의 내용을 메인 화면에 그려준다
	SelectObject(hMemDC, hOldBit);//뒷정리
	DeleteDC(hMemDC);
}

void MyDoubleBuffer::DrawScreen(HDC hdc,int x,int y,int width,int height)//메인화면의 dc를 받아와서 백버퍼의 내용을 화면으로 그려주는 함수
{
	HDC hMemDC = CreateCompatibleDC(hdc);//백버퍼를 연결할 DC
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, MyDoubleBuffer::BackBit);//만들어준 DC에 비트맵을 연결해줌 
	BITMAP bit;//비트맵의 정보를 가지고 올 변수
	GetObject(BackBit, sizeof(BITMAP), &bit);//백버퍼의 정보를 가지고 오고
	BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, hMemDC, x, y, SRCCOPY);//백버퍼의 내용을 메인 화면에 그려준다
	SelectObject(hMemDC, hOldBit);//뒷정리
	DeleteDC(hMemDC);
}

void MyDoubleBuffer::DrawBackBufferWords(HWND hWndMain, TCHAR* str, int x, int y, int size)
{
	RECT rt;
	HDC hdc = GetDC(hWndMain);//메인 dc를 만들어줌
	HDC hMemDC = CreateCompatibleDC(hdc);//백버퍼를 연결해줄 dc를 만들어줌
	HFONT hFont, OldFont;
	//TCHAR printstr[255] = { 0 };
	//HDC MemDC = CreateCompatibleDC(hMemDC);//비트맵을 연결해줄 dc를 만들어줌
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//백버퍼와 그려줄 비트맵을 연결해준 다음네
	hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("GULIM"));
	OldFont = (HFONT)SelectObject(hMemDC, hFont);
	SetBkMode(hMemDC, TRANSPARENT);

	rt.left = x;
	rt.top = y;
	rt.right = x + 500;
	rt.bottom = y + 500;
	//TextOut(hMemDC, x, y, str, lstrlen(str));
	DrawText(hMemDC, str, lstrlen(str), &rt, DT_LEFT | DT_WORDBREAK);


	SelectObject(hMemDC, hOldBit);
	SelectObject(hMemDC, OldFont);
	//DeleteDC(MemDC);
	DeleteObject(hFont);
	DeleteDC(hMemDC);

	ReleaseDC(hWndMain, hdc);
}



void MyDoubleBuffer::DrawRect(HWND hWndMain, int x, int y, int width, int height, int rgb)
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hMemDC, BackBit);
	HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hMemDC, MyBrush);
	Rectangle(hMemDC, x, y, x + width, y + height);
	SelectObject(hMemDC, OldBrush);
	DeleteObject(OldBrush);
	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}

void MyDoubleBuffer::DrawCircle(HWND hWndMain, int x, int y, int radious, int rgb)
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hMemDC, BackBit);
	HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hMemDC, MyBrush);
	Ellipse(hMemDC, x - radious, y - radious, x + radious, y + radious);
	SelectObject(hMemDC, OldBrush);
	DeleteObject(OldBrush);
	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}

void MyDoubleBuffer::DrawInfo(HWND hWndMain, int x, int y,TCHAR* str)
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hMemDC, BackBit);
	TextOut(hMemDC, x, y, str, lstrlen(str));
	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);

}

