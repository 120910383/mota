#include "AppDelegate.h"
#include "FloorMapLayer.h"
#include "Player.h"
#include "ModalDialog.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化游戏单例
    Player::NewInstance();
    ModalDialogManager::NewInstance();

    // 初始化Director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (glview == nullptr) {
        glview = GLViewImpl::create("Mota");
    }

    director->setOpenGLView(glview);

    glview->setFrameSize(1080, 1920);
    glview->setDesignResolutionSize(650, 900, ResolutionPolicy::FIXED_WIDTH);
    

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    director->runWithScene(FloorMapLayer::scene(1));
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