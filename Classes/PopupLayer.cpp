#include "PopupLayer.h"

PopupLayer::PopupLayer() :
m__pMenu(NULL)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m__sfBackGround(NULL)
, m__s9BackGround(NULL)
, m__ltContentText(NULL)
, m__ltTitle(NULL)
{

}

PopupLayer::~PopupLayer(){
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
    CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopupLayer::init(){
    if (!LayerColor::init()){
        return false;
    }
    // 初始化需要的 Menu
    Menu* menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    setMenuButton(menu);

    //add layer touch event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(PopupLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PopupLayer::onTouchEnded, this);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    setColor(ccc3(0, 0, 0));
    setOpacity(128);

    return true;
}

bool PopupLayer::onTouchBegan(Touch *touch, Event *event){
    return true;
}

void PopupLayer::onTouchMoved(Touch *touch, Event *event){

}

void PopupLayer::onTouchEnded(Touch* touch, Event* event){

}

PopupLayer* PopupLayer::create(const char* backgroundImage, Size dialogSize){

    PopupLayer* layer = PopupLayer::create();

    //	layer->setSpriteBackGround(Sprite::create(backgroundImage));
    layer->setSprite9BackGround(Scale9Sprite::create(backgroundImage));

    layer->m_dialogContentSize = dialogSize;

    return layer;
}

void PopupLayer::setTitle(const char* title, int fontsize /* = 20 */){
    LabelTTF* label = LabelTTF::create(title, "", fontsize);
    setLabelTitle(label);
}

void PopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop){
    LabelTTF* ltf = LabelTTF::create(text, "", fontsize);
    setLabelContentText(ltf);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(Ref* target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;
}

bool PopupLayer::addButton(const char* normalImage, const char* selectedImage, const char* title, int tag /* = 0 */){

    auto size = Director::getInstance()->getWinSize();
    auto center = Point(size.width / 2, size.height / 2);

    // 创建图片菜单按钮
    auto item = MenuItemImage::create(
        normalImage,
        selectedImage,
        CC_CALLBACK_1(PopupLayer::buttonCallBack, this));
    item->setTag(tag);
    item->setPosition(center);

    // 添加文字说明并设置位置
    Size itemSize = item->getContentSize();
    LabelTTF* ttf = LabelTTF::create(title, "", 20);
    ttf->setColor(Color3B(0, 0, 0));
    ttf->setPosition(Point(itemSize.width / 2, itemSize.height / 2));
    item->addChild(ttf);

    getMenuButton()->addChild(item);

    return true;
}

void PopupLayer::buttonCallBack(Ref* pSender){
    Node* node = dynamic_cast<Node*>(pSender);
    if (m_callback && m_callbackListener){
        (m_callbackListener->*m_callback)(node);
    }
    this->removeFromParent();
}

void PopupLayer::onEnter(){
    LayerColor::onEnter();

    Size winSize = CCDirector::getInstance()->getWinSize();
    Point pCenter = Point(winSize.width / 2, winSize.height / 2);

    //	Size contentSize ;
    // 设定好参数，在运行时加载
    //如果没有设置 ContentSize ，那么采取的方案是，窗口大小与传入图片一样大
    // 	if (getContentSize().equals(this->getParent()->getContentSize())) {
    // 		getSpriteBackGround()->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    // 		this->addChild(getSpriteBackGround(), 0, 0);
    // 		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
    // 	} else {
    // 		Scale9Sprite *background = getSprite9BackGround();
    // 		background->setContentSize(getContentSize());
    // 		background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    // 		this->addChild(background, 0, 0);
    // 		contentSize = getContentSize();
    // 	}
    //添加背景图片
    Scale9Sprite *background = getSprite9BackGround();
    background->setContentSize(m_dialogContentSize);
    background->setPosition(Point(winSize.width / 2, winSize.height / 2));
    this->addChild(background, 0, 0);

    // 弹出效果
    Action* popupLayer = Sequence::create(
        ScaleTo::create(0.0, 0.0),
        ScaleTo::create(0.2, 1.05),
        ScaleTo::create(0.2, 0.95),
        ScaleTo::create(0.1, 1.0),
        CallFunc::create(CC_CALLBACK_0(PopupLayer::backgroundFinish, this)),
        NULL
        );
    background->runAction(popupLayer);



}

void PopupLayer::backgroundFinish(){

    Size winSize = CCDirector::getInstance()->getWinSize();
    Point pCenter = Point(winSize.width / 2, winSize.height / 2);

    // 添加按钮，并设置其位置
    this->addChild(getMenuButton());
    float btnWidth = m_dialogContentSize.width / (getMenuButton()->getChildrenCount() + 1);

    Vector<Node*> vector = getMenuButton()->getChildren();
    Ref* pObj = NULL;
    int i = 0;
    for (Node* pObj : vector){
        Node* node = dynamic_cast<Node*>(pObj);
        node->setPosition(Point(winSize.width / 2 - m_dialogContentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - m_dialogContentSize.height / 3));
        i++;
    }

    // 显示对话框标题
    if (getLabelTitle()){
        getLabelTitle()->setPosition(ccpAdd(pCenter, ccp(0, m_dialogContentSize.height / 2 - 35.0f)));
        this->addChild(getLabelTitle());
    }

    // 显示文本内容
    if (getLabelContentText()){
        CCLabelTTF* ltf = getLabelContentText();
        ltf->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        ltf->setDimensions(CCSizeMake(m_dialogContentSize.width - m_contentPadding * 2, m_dialogContentSize.height - m_contentPaddingTop));
        ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
        this->addChild(ltf);
    }
}



void PopupLayer::onExit(){

    CCLayerColor::onExit();
}