#include "MyWin.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    C_MYWIN::createWin();
    C_MYWIN::getWin()->init(hInstance);
    C_MYWIN::getWin()->updateMsg();
    C_MYWIN::getWin()->release();
    C_MYWIN::releaseWin();

    return 0;
}