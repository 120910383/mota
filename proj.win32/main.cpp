#include "pre_header.h"

#include "main.h"
#include "../Classes/AppDelegate.h"

USING_NS_CC;

int APIENTRY _tWinMain(__in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in LPTSTR lpCmdLine,
    __in int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 创建应用程序实例
    AppDelegate app;
    return Application::getInstance()->run();
}