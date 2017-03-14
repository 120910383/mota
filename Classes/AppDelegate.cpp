/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "AppDelegate.h"
#include "MainScene.h"

#include "tiles_res.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    tiles_res::DeleteInstance();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("mota", Rect(0, 0, 1080, 1920));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(660, 1173, ResolutionPolicy::FIXED_WIDTH);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("data");
    FileUtils::getInstance()->addSearchPath("tiles");

    tiles_res::NewInstance();

    auto result = tiles_res::GetInstance()->load();
    if (!result)
    {
        return false;  // load config file error.
    }

    // create a scene. it's an autorelease object
    auto scene = MainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
