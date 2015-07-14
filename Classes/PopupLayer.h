#pragma once

#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class PopupLayer : public LayerColor{
public:
    PopupLayer();
    ~PopupLayer();

    virtual bool init();
    CREATE_FUNC(PopupLayer);
    static PopupLayer* create(const char* backgroundImage, Size dialogSize);

    //touch事件监听 屏蔽向下触摸
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch* touch, Event* event);

    //标题
    void setTitle(const char* title, int fontsize = 20);
    //文本
    void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);
    //设置button回调事件
    void setCallbackFunc(Ref* target, SEL_CallFuncN callfun);
    //添加button
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);

    virtual void onEnter();
    virtual void onExit();

    void backgroundFinish();

private:

    void buttonCallBack(Ref* pSender);

    // 文字内容两边的空白区
    int m_contentPadding;
    int m_contentPaddingTop;

    Size m_dialogContentSize;

    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;

    //set and get
    CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
    CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(LabelTTF*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(LabelTTF*, m__ltContentText, LabelContentText);
};