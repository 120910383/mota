/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "tiles_res.h"

USING_NS_CC;
using namespace std;
using namespace cocostudio::timeline;

#define WARRIOR_OFFSET(pos) ((pos) - Point(42.0f, 70.0f))

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *e)
{
    return true;
}

void MainScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *e)
{
    auto touch_pos = _map_node->convertTouchToNodeSpace(touch);
    auto end_point = pos_t(touch_pos);

    if (_floor->is_block(end_point))
        return;

    _arrow_node->setVisible(true);
    _arrow_node->setPosition(end_point.center_pos());

    const float speed = 150.0f;
    auto distance = _warrior_node->getPosition().distance(WARRIOR_OFFSET(end_point.center_pos()));
    _warrior_node->stopAllActions();
    _warrior_node->getAnimation()->play("Frun");
    _warrior_node->runAction(Sequence::createWithTwoActions(
        MoveTo::create(distance / speed, WARRIOR_OFFSET(end_point.center_pos())),
        CallFunc::create([this]() {
                _warrior_node->getAnimation()->play("Sstand");
                _arrow_node->setVisible(false);
            })
        ));
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _map_node = Node::create();
    _map_node->setContentSize(Size(750, 900));
    _map_node->setPosition(Point(-45, -60));
    addChild(_map_node);

    int32_t current_floor = 12;
    _floor = tiles_res::GetInstance()->get_floor_info(current_floor);
    for_each(_floor->floors.begin(), _floor->floors.end(), bind(&MainScene::add_tile_sprite, this, placeholders::_1));
    for_each(_floor->blocks.begin(), _floor->blocks.end(), bind(&MainScene::add_tile_sprite, this, placeholders::_1));
    for_each(_floor->npcs.begin(), _floor->npcs.end(), bind(&MainScene::add_tile_sprite, this, placeholders::_1));
    add_tile_sprite(_floor->stair_down);
    add_tile_sprite(_floor->stair_up);

    _arrow_node = CSLoader::createNode("arrow_node.csb");
    _arrow_node->setPosition(Vec2::ZERO);
    _arrow_node->setVisible(false);
    _map_node->addChild(_arrow_node);

    auto timeline = CSLoader::createTimeline("arrow_node.csb");
    timeline->play("updown", true);
    _arrow_node->runAction(timeline);

    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("spine/hero.ExportJson");
    _warrior_node = cocostudio::Armature::create("hero");
    _warrior_node->getAnimation()->play("Sstand");
    _warrior_node->setScaleX(_floor->stair_down.flip ? 1.0f : -1.0f);
    auto warrior_pos = _floor->stair_down.pos.offset(_floor->stair_down.flip ? -1 : 1, 0).center_pos();
    _warrior_node->setPosition(WARRIOR_OFFSET(warrior_pos));
    addChild(_warrior_node);

    auto ui_node = CSLoader::createNode("game_layer.csb");
    addChild(ui_node);

    return true;
}

void MainScene::add_tile_sprite(const tile_t& elem)
{
    auto sprite = Sprite::create(elem.res);
    sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    sprite->setPosition(elem.pos.origin_pos());
    sprite->setFlippedX(elem.flip);
    _map_node->addChild(sprite, 0, elem.id);
}