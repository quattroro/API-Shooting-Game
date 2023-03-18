#include "MyWin.h"
C_MYWIN* C_MYWIN::m_pMyWin = nullptr;

C_MYWIN::C_MYWIN() :
    m_hInstance(NULL),
    m_hWnd(NULL),
    doublebuffer(NULL)
{
}

void C_MYWIN::createWin()
{
    if (!m_pMyWin)
        m_pMyWin = new C_MYWIN();
}

C_MYWIN* C_MYWIN::getWin()
{
    return m_pMyWin;
}

void C_MYWIN::releaseWin()
{
    if (m_pMyWin)
    {
        delete m_pMyWin;
        m_pMyWin = nullptr;
    }
}

bool C_MYWIN::init(HINSTANCE hInstance)
{
    //������ Ŭ���� ���
    WNDCLASS WndClass;

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = wndProc;//�����쿡�� �޽����� �߻������� �� �޽����� ó���� �Լ��� ����
    WndClass.lpszClassName = "������";
    WndClass.lpszMenuName = NULL;//������� �޴��� WndClass�� �־ ��������
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);//������ ������ ���� ������ Ŭ������ �������


    //������ ����
    m_hWnd = CreateWindow("������", "������", WS_OVERLAPPEDWINDOW,//�����츦 �������
        CW_USEDEFAULT, CW_USEDEFAULT, 960, 540,
        NULL, (HMENU)NULL, hInstance, NULL);

    if (!m_hWnd)
        return false;

    //�ʱ�ȭ �۾��� ���⿡
    m_hInstance = hInstance;

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hWnd);

    return true;
}

void C_MYWIN::updateMsg()
{
    MSG msg = {};

    // �⺻ �޽��� �����Դϴ�.
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


}

void C_MYWIN::release()
{
}

LRESULT C_MYWIN::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return m_pMyWin->myProc(hWnd, message, wParam, lParam);
}

LRESULT C_MYWIN::myProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT crt;
    switch (message)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        GetClientRect(hWnd, &crt);
        doublebuffer = MyDoubleBuffer::GetBufferInstance();
        doublebuffer->BufferInit(m_hInstance, hWnd, 0, 0, crt.right + 120, crt.bottom + 120);
        maincontroller = GameMainController::ControllerInstance();
        maincontroller->InitSetting(m_hInstance, hWnd, doublebuffer);
      
        break;

    case WM_LBUTTONDOWN:
        maincontroller->MouseClick(lParam);
        break;

    case WM_RBUTTONDOWN:
        maincontroller->MouseRClick();
        break;

    case WM_CHAR:
        break;

    case WM_KEYDOWN:
        maincontroller->KeyDownProc(wParam);
        break;

    case WM_KEYUP:
        maincontroller->KeyUpProc(wParam);
        break;

    case WM_MOUSEMOVE:
        maincontroller->MouseMove(lParam);
        break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        doublebuffer->DrawScreen(hdc, 0, 0, 0, 0);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        //delete doublebuffer;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}