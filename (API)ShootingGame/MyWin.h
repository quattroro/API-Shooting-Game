#pragma once
#include <windows.h>
#include "MyDoubleBuffer.h"
#include "GameMainController.h"

class C_MYWIN
{
private:
    HINSTANCE   m_hInstance;
    HWND        m_hWnd;
    

private:
    static C_MYWIN* m_pMyWin;

public:
    static void createWin();
    static C_MYWIN* getWin();
    static void releaseWin();
    HWND hWndMain;
    MyDoubleBuffer* doublebuffer;
    GameMainController* maincontroller;



private:
    C_MYWIN();
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK myProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    bool init(HINSTANCE hInstance);
    void updateMsg();
    void release();
};

