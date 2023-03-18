#pragma once
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib,"msimg32.lib")

#include <Windows.h>
#include <math.h>

#include "resource.h"

#define DMOD_NOMAL 300
#define DMOD_FLIP_HORIZONTAL 301
#define DMOD_FLIP_VERTICAL 302

class MyDoubleBuffer
{
public:
	static MyDoubleBuffer* GetBufferInstance();
	static MyDoubleBuffer* _Instance;

	enum BITMAPS { CHARACTERFISH, MONSTERBANDIT, MONSTERMAGGOT, MONSTERMAGGOTNEST, MONSTERSCORPION, MONSTERBIGBANDIT, GUNS, BULLETS, CHESTS, HART, STARTSCREEN, CHARACTERCARD, CHARACTERCRYSTAL, CURSOR, ENTERICON, BITMAX };

	HBITMAP GetRotatedBitmap(HWND MainhWnd, HBITMAP hBitmap, int source_x, int source_y, int dest_width, int dest_height, float angle, COLORREF bkColor = RGB(63, 73, 127));
	void BufferInit(HINSTANCE hInst, HWND MainhWnd, int x, int y, int Width, int Height);
	void BufferClear(HDC hdc);//����۸� �ʱ�ȭ ���ִ� �Լ�
	void BufferClear(HWND hWndMain, int rgb = RGB(255, 255, 255));//����۸� �ʱ�ȭ ���ִ� �Լ�
	void DrawBackBufferBitmap(HWND hWndMain, HBITMAP bitmap, int x, int y, int Width, int Height, int originx, int originy, int originwidth, int originheight, int Mode = DMOD_NOMAL);//����ۿ� �޾ƿ� ��Ʈ���� �޾ƿ� ��ġ�� ũ��� �׷��ִ� �Լ� 
	void DrawBackBufferBitmap(HDC hMemDC, HBITMAP bitmap, int x, int y, int Width, int Height, int originx, int originy, int originwidth, int originheight);
	void DrawScreen(HDC hdc);//����ȭ���� dc�� �޾ƿͼ� ������� ������ ȭ������ �׷��ִ� �Լ�
	void DrawBackBufferWords(HWND hWndMain, TCHAR* str, int x, int y, int size);
	void DrawRect(HWND hWndMain, int x, int y, int width, int height, int rgb = RGB(255, 255, 255));
	void DrawInfo(HWND hWndMain, int x, int y, TCHAR* str);
	void DrawScreen(HDC hdc, int x, int y, int width, int height);
	void DrawCircle(HWND hWndMain, int x, int y, /*int width, int height,*/int radious, int rgb = RGB(255, 255, 255));
	void DrawBitBlt(HWND hWndMain, HBITMAP bitmap, int x, int y, int width, int height, int originx, int originy);

	~MyDoubleBuffer()
	{
		return;
	}
private:
	HBITMAP BackBit;

public:
	HBITMAP MainBitmaps[BITMAX];
	


};

