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

    int32_t current_floor = 6;
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