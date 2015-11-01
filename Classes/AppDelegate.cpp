#include "pre_header.h"

#include "AppDelegate.h"
#include "FloorMapLayer.h"
#include "Player.h"
#include "ModalDialog.h"
#include "Floor.h"
#include "CustomTimer.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
    // 初始化游戏单例
    CustomTimer::NewInstance();
    Player::NewInstance();
    ModalDialogManager::NewInstance();
    Floor::NewInstance();
}

AppDelegate::~AppDelegate()
{
    // 释放游戏单例
    CustomTimer::DeleteInstance();
    Player::DeleteInstance();
    ModalDialogManager::DeleteInstance();
    Floor::DeleteInstance();
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化Director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (glview == nullptr) {
        glview = GLViewImpl::create("Mota");
    }

    director->setOpenGLView(glview);

    glview->setFrameSize(1080, 1920);   // 在windows下根据自己分辨率设置窗口分辨率，作者4K因此设置比较大
    glview->setDesignResolutionSize(650, 900, ResolutionPolicy::FIXED_WIDTH);
    

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    int32_t last_floor = Player::GetInstance()->get_current_floor();
    director->runWithScene(FloorMapLayer::scene(last_floor, true));
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}