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

	HDC sourceDC = CreateCompatibleDC(hdc); // ȸ���� ��Ʈ�� ������ ������ DC
	HDC destDC = CreateCompatibleDC(hdc); // ȸ���� ��Ʈ���� ��¹��� DC
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, dest_width, dest_height); // ȸ���� ��Ʈ���� ��¹��� �޸𸮺�Ʈ�� �ڵ�

	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap); // ȸ���� ��Ʈ�� ������ DC�� ����
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult); // ȸ���� ��Ʈ���� ��¹��� DC��, ȸ���� ��Ʈ���� ��¹��� �޸𸮺�Ʈ�� ����

	HBRUSH hbrBack = CreateSolidBrush(bkColor); // ȸ������ ����, ���鿵���� ĥ�� �귯���ڵ� ����
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack); // �귯���ڵ� ����
	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY); // ���õ� �귯�÷�, ȸ���� ��Ʈ���� ��¹��� DC��, �̸� ������ ä�� ����
	DeleteObject(SelectObject(destDC, hbrOld)); // �귯�� ����

	angle = (float)(angle * PI / 180); // ȸ���� ����(360�й�)��, ���ȴ����� ������ ����
	float cosine = (float)cos(angle); // ȸ���̵���ȯ ��Ŀ� �� cos��Ÿ ���� ����
	float sine = (float)sin(angle); // ȸ���̵���ȯ ��Ŀ� �� sin��Ÿ ���� ����

	SetGraphicsMode(destDC, GM_ADVANCED); // ������ ��ǥ���� ȸ���� ���Ͽ�, �׷��ȸ�带 Ȯ����� �����մϴ�.(�䱸����:��98,NT�̻�)

		// ȸ���̵���ȯ ����� ����.
		// (�������� API���� ������ 2�� Page(1279) ����.)
		// (���� ���α׷��Ӹ� ���� ���ʼ��а� ���� Page(196) ����.)

	XFORM xform; // �������� ǥ���ϴ� 3��3���� ��� ����
	xform.eM11 = cosine; // 1�� 1�� ���� ���� (ȸ������)
	xform.eM12 = sine; // 1�� 2�� ���� ���� (ȸ������)
	xform.eM21 = -sine; // 2�� 1�� ���� ���� (ȸ������)
	xform.eM22 = cosine; // 2�� 2�� ���� ���� (ȸ������)
	xform.eDx = (FLOAT)dest_width / 2.0f; // 3�� 1�� ���� ���� (X�� �̵� ����)
	xform.eDy = (FLOAT)dest_height / 2.0f; // 3�� 2�� ���� ���� (Y�� �̵� ����)

		// ������ ��ķ�, �μ�DC�� �������� ��ǥ�踦 ��ȯ.
		// �μ�DC�� ��´������ �޸𸮺�Ʈ���� �����Ƿ�,
		// �����ϰ� �ִ� �޸𸮺�Ʈ���� ��ǥü�谡 ȸ���ȴ�.

	SetWorldTransform(destDC, &xform);

	// ȸ���� �޸�DC��, ȸ���� �̹����� ���

	BitBlt(destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height, sourceDC, source_x, source_y, SRCCOPY);

	// ���� �ڿ� ����

	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);
	DeleteObject(sourceDC);
	DeleteObject(destDC);
	ReleaseDC(MainhWnd, hdc);
	// ȸ���� ��Ʈ�� �ڵ� ����

	return hbmResult;
}

void MyDoubleBuffer::BufferInit(HINSTANCE hInst, HWND MainhWnd, int x, int y, int Width, int Height)//�Է¹��� ��ġ���� ũ��� ����۸� ������ְ� �ʱ�ȭ���ִ� �Լ�
{
	HDC hDC = GetDC(MainhWnd);
	HDC hMemDC = CreateCompatibleDC(hDC);
	BackBit = CreateCompatibleBitmap(hDC, Width, Height);//������۸��� ����۷� ����� BackBit�� ��밡���ϰ� ���ְ�
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)BackBit);
	BufferClear(hMemDC);//BackBit�� �������� �̹Ƿ� ó������ �������̱⶧���� ������� ĥ���ش�
	

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

void MyDoubleBuffer::BufferClear(HDC hdc)//����۸� �ʱ�ȭ ���ִ� �Լ�
{
	BITMAP bit;//������� ũ�⸦ �޾ƿ� ����
	HBITMAP hOldBit = (HBITMAP)SelectObject(hdc, (HBITMAP)BackBit);//����۸� �����ؼ�
	GetObject(BackBit, sizeof(BITMAP), &bit);
	RECT crt;
	SetRect(&crt, 0, 0, bit.bmWidth, bit.bmHeight);//
	FillRect(hdc, &crt, GetSysColorBrush(COLOR_WINDOW));//������� ũ�⸸ŭ ������� ĥ���ش�.
	SelectObject(hdc, hOldBit);
}

void MyDoubleBuffer::BufferClear(HWND hWndMain, int rgb)//����۸� �ʱ�ȭ ���ִ� �Լ�
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	BITMAP bit;//������� ũ�⸦ �޾ƿ� ����
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//����۸� �����ؼ�
	HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hMemDC, MyBrush);
	GetObject(MyDoubleBuffer::BackBit, sizeof(BITMAP), &bit);
	RECT crt;
	SetRect(&crt, 0, 0, bit.bmWidth, bit.bmHeight);//
	FillRect(hMemDC, &crt, /*GetSysColorBrush(COLOR_WINDOW)*/MyBrush);//������� ũ�⸸ŭ ������� ĥ���ش�.
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
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//����ۿ� �׷��� ��Ʈ���� �������� ������
	HBITMAP OldBit = (HBITMAP)SelectObject(MemDC, (HBITMAP)bitmap);


	BitBlt(hMemDC, x, y, width, height, MemDC, originx, originy, SRCCOPY);

	SelectObject(MemDC, OldBit);//��������
	SelectObject(hMemDC, hOldBit);
	DeleteDC(MemDC);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}


void MyDoubleBuffer::DrawBackBufferBitmap(HWND hWndMain, HBITMAP bitmap, int x, int y, int Width, int Height, int originx, int originy, int originwidth, int originheight, int Mode)//����ۿ� �޾ƿ� ��Ʈ���� �޾ƿ� ��ġ�� ũ��� �׷��ִ� �Լ� 
{
	HDC hdc = GetDC(hWndMain);//���� dc�� �������
	HDC hMemDC = CreateCompatibleDC(hdc);//����۸� �������� dc�� �������
	HDC MemDC = CreateCompatibleDC(hMemDC);//��Ʈ���� �������� dc�� �������
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//����ۿ� �׷��� ��Ʈ���� �������� ������
	HBITMAP OldBit = (HBITMAP)SelectObject(MemDC, (HBITMAP)bitmap);
	BITMAP bit;
	GetObject(bitmap, sizeof(BITMAP), &bit);//��Ʈ���� ������ �޾ƿͼ� 
	//BufferClear(hMemDC);//�׷��ֱ� ���� ����۸� Ŭ��������ش�

	switch (Mode)
	{
	case DMOD_NOMAL:
		TransparentBlt(hMemDC, x, y, Width, Height, MemDC, originx, originy, originwidth, originheight, RGB(63, 73, 127));//�׷���� �޾ƿ� ��ġ�� ũ�⸸ŭ ��Ʈ���� �׷��ش�
		//TransparentBlt(hMemDC, x, y, Width, Height, MemDC, originx, originy, originwidth, originheight, RGB(63, 73, 127));//�׷���� �޾ƿ� ��ġ�� ũ�⸸ŭ ��Ʈ���� �׷��ش�
		break;

	case DMOD_FLIP_VERTICAL:
		//TransparentBlt(hMemDC, x, y, Width, Height, MemDC, 0, bit.bmHeight - 1, bit.bmWidth, (bit.bmHeight * -1), RGB(255, 0, 255));//�׷���� �޾ƿ� ��ġ�� ũ�⸸ŭ ��Ʈ���� �׷��ش�
		StretchBlt(hMemDC, x, y, Width, Height, MemDC, 0, bit.bmHeight - 1, bit.bmWidth, (bit.bmHeight * -1), SRCCOPY);//���ι������� �׸��� �׷��� (��Ʈ���� �� �Ʒ��������� -��Ʈ���� ���̸�ŭ �ҷ����鼭 �׷��ش�.)
		break;

	case DMOD_FLIP_HORIZONTAL:
		//TransparentBlt(hMemDC, x, y, Width, Height, MemDC, bit.bmWidth - 1, 0, (bit.bmWidth * -1), bit.bmHeight, RGB(255, 0, 255));//�׷���� �޾ƿ� ��ġ�� ũ�⸸ŭ ��Ʈ���� �׷��ش�
		StretchBlt(hMemDC, x, y, Width, Height, MemDC, bit.bmWidth - 1, 0, (bit.bmWidth * -1), bit.bmHeight, SRCCOPY);//���ι������� �׸��� �׷��� (��Ʈ���� �� �����ʿ��� ���� -��Ʈ���� �ʺ�ŭ �ҷ����鼭 �׷��ش�.)
		break;
	}


	SelectObject(MemDC, OldBit);//��������
	SelectObject(hMemDC, hOldBit);
	DeleteDC(MemDC);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}

void MyDoubleBuffer::DrawBackBufferBitmap(HDC hMemDC, HBITMAP bitmap, int x, int y, int Width, int Height, int originx, int originy, int originwidth, int originheight)//����ۿ� �׷��ٶ� �Լ��ȿ��� �Լ��� �����ų�� hMemDC�� �ߺ��ؼ� ����� ��Ʈ�� ����� �ȵǼ� hMemDC�� �Ű������� �޾ƿ��� �Լ��� �����ε���
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

void MyDoubleBuffer::DrawScreen(HDC hdc)//����ȭ���� dc�� �޾ƿͼ� ������� ������ ȭ������ �׷��ִ� �Լ�
{
	HDC hMemDC = CreateCompatibleDC(hdc);//����۸� ������ DC
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, MyDoubleBuffer::BackBit);//������� DC�� ��Ʈ���� �������� 
	BITMAP bit;//��Ʈ���� ������ ������ �� ����
	GetObject(BackBit, sizeof(BITMAP), &bit);//������� ������ ������ ����
	BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, hMemDC, 0, 0, SRCCOPY);//������� ������ ���� ȭ�鿡 �׷��ش�
	SelectObject(hMemDC, hOldBit);//������
	DeleteDC(hMemDC);
}

void MyDoubleBuffer::DrawScreen(HDC hdc,int x,int y,int width,int height)//����ȭ���� dc�� �޾ƿͼ� ������� ������ ȭ������ �׷��ִ� �Լ�
{
	HDC hMemDC = CreateCompatibleDC(hdc);//����۸� ������ DC
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, MyDoubleBuffer::BackBit);//������� DC�� ��Ʈ���� �������� 
	BITMAP bit;//��Ʈ���� ������ ������ �� ����
	GetObject(BackBit, sizeof(BITMAP), &bit);//������� ������ ������ ����
	BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, hMemDC, x, y, SRCCOPY);//������� ������ ���� ȭ�鿡 �׷��ش�
	SelectObject(hMemDC, hOldBit);//������
	DeleteDC(hMemDC);
}

void MyDoubleBuffer::DrawBackBufferWords(HWND hWndMain, TCHAR* str, int x, int y, int size)
{
	RECT rt;
	HDC hdc = GetDC(hWndMain);//���� dc�� �������
	HDC hMemDC = CreateCompatibleDC(hdc);//����۸� �������� dc�� �������
	HFONT hFont, OldFont;
	//TCHAR printstr[255] = { 0 };
	//HDC MemDC = CreateCompatibleDC(hMemDC);//��Ʈ���� �������� dc�� �������
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, (HBITMAP)MyDoubleBuffer::BackBit);//����ۿ� �׷��� ��Ʈ���� �������� ������
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

