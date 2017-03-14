/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

#include "tiles_res.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    tiles_res::NewInstance();

    auto result = tiles_res::GetInstance()->load();
    if (!result)
    {
        return -1;  // load config file error.
    }

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();

    tiles_res::DeleteInstance();
}
