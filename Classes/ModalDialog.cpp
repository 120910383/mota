#include "pre_header.h"

#include "ModalDialog.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

class BlockLayer : public Layer
{
public:
    CREATE_FUNC(BlockLayer);

    bool init() override
    {
        if (!Layer::init())
            return false;

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(BlockLayer::onTouchBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        return true;
    }

    virtual bool onTouchBegan(Touch* touch, Event* event) override
    {
        return true;
    }
};

void ModalDialogManager::pushDialog(cocos2d::Node* dialog)
{
    auto container = BlockLayer::create();
    Director::getInstance()->getRunningScene()->addChild(container, 9999);
    
    auto background = LayerColor::create();
    container->addChild(background, 0);
    background->runAction(FadeTo::create(0.15f, 80));

    dialog->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto win_size = Director::getInstance()->getWinSize();
    dialog->setPosition(win_size / 2);
    container->addChild(dialog, 1);

    dialog->setScale(0.8f);
    dialog->runAction(
        Sequence::create(
            ScaleTo::create(0.1f, 1.1f),
            ScaleTo::create(0.07f, 0.9f),
            ScaleTo::create(0.05f, 1.0f),
            nullptr
            )
        );

    _dialogs.pushBack(container);
}

void ModalDialogManager::popDialog()
{
    if (_dialogs.size() > 0)
    {
        _dialogs.back()->removeFromParentAndCleanup(true);
        _dialogs.popBack();
    }
}

bool ModalDialogManager::isShown()
{
    return !_dialogs.empty();
}

OKCancelDialog * OKCancelDialog::create(const std::string & title, const std::string & content)
{
    OKCancelDialog* ret = new (std::nothrow) OKCancelDialog();
    if (ret && ret->init(title, content))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool OKCancelDialog::init(const std::string & title, const std::string & content)
{
    const Size DIALOG_SIZE(540.0f, 360.0f);
    setContentSize(DIALOG_SIZE);

    auto background = ui::Scale9Sprite::create("Images/UI_shared_bg.png");
    if (background != nullptr)
    {
        background->setContentSize(DIALOG_SIZE);
        background->setAnchorPoint(Vec2::ZERO);
        background->setPosition(Vec2::ZERO);
        addChild(background);
    }

    auto title_label = Label::createWithSystemFont(title, "", 28);
    if (title_label != nullptr)
    {
        title_label->setPosition(Vec2(DIALOG_SIZE.width / 2, 300.0f));
        addChild(title_label);
    }

    auto content_label = Label::createWithSystemFont(content, "", 28);
    if (content_label != nullptr)
    {
        content_label->setPosition(Vec2(DIALOG_SIZE.width / 2, 200.0f));
        addChild(content_label);
    }

    auto cancel_button = MenuItemImage::create(
        "Images/UI_tip_fangqi.png",
        "Images/UI_tip_fangqi_selected.png",
        [&](Ref* ref) {
        if (_callback)
            _callback(CANCEL);
        ModalDialogManager::GetInstance()->popDialog();
    });

    auto ok_button = MenuItemImage::create(
        "Images/UI_tip_queding.png",
        "Images/UI_tip_queding_selected.png",
        [&](Ref* ref) {
        if (_callback)
            _callback(OK);
        ModalDialogManager::GetInstance()->popDialog();
    });

    if (cancel_button == nullptr || ok_button == nullptr)
        return false;
    auto menu = Menu::create(cancel_button, ok_button, nullptr);
    if (menu == nullptr)
        return false;

    menu->alignItemsHorizontallyWithPadding(60.0f);
    menu->setPosition(Vec2(DIALOG_SIZE.width / 2, 80.0f));
    addChild(menu);

    return true;
}

void PromptDialog::show(const std::string & content)
{
    PromptDialog* ret = new (std::nothrow) PromptDialog();
    if (!ret || !ret->init(content))
    {
        CC_SAFE_DELETE(ret);
        return;
    }

    ret->autorelease();

    auto win_size = Director::getInstance()->getWinSize();
    ret->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    ret->setPosition(win_size / 2);
    ret->setCascadeOpacityEnabled(true);
    Director::getInstance()->getRunningScene()->addChild(ret, 99999);
    ret->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(0.5f), CallFunc::create([ret]() {
        ret->removeFromParentAndCleanup(true);
    }), nullptr));
}

bool PromptDialog::init(const std::string & content)
{
    const Size DIALOG_SIZE(540.0f, 360.0f);
    setContentSize(DIALOG_SIZE);

    auto background = ui::Scale9Sprite::create("Images/UI_shared_bg.png");
    if (background != nullptr)
    {
        background->setContentSize(DIALOG_SIZE);
        background->setAnchorPoint(Vec2::ZERO);
        background->setPosition(Vec2::ZERO);
        addChild(background);
    }

    auto content_label = Label::createWithSystemFont(content, "", 28);
    if (content_label == nullptr)
        return false;

    content_label->setPosition(Vec2(DIALOG_SIZE.width / 2, 200.0f));
    addChild(content_label);

    return true;
}
