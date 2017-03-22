/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "maps.h"
#include "player.h"
#include "timer_queue.h"

USING_NS_CC;
using namespace std;
using namespace std::placeholders;
using namespace cocostudio::timeline;

#define WARRIOR_OFFSET(pos) ((pos) - Point(42.0f, 70.0f))

namespace
{
    string get_direction_str(player_t::direction dir)
    {
        string result = "F";
        switch (dir)
        {
        case player_t::left:
        case player_t::right:
            result = "S";
            break;
        case player_t::up:
            result = "B";
            break;
        case player_t::down:
            result = "F";
            break;
        }
        return result;
    }
}

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

void MainScene::update(float delta)
{
    timer_queue::GetInstance()->timer_handle((uint32_t)(delta * 1000000));
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

    player_t::GetInstance()->walk_to(end_point, bind(&MainScene::on_move, this, _1), bind(&MainScene::on_end, this));

    //const float speed = 150.0f;
    //auto distance = _warrior_node->getPosition().distance(WARRIOR_OFFSET(end_point.center_pos()));
    //_warrior_node->stopAllActions();
    //_warrior_node->getAnimation()->play("Frun");
    //_warrior_node->runAction(Sequence::createWithTwoActions(
    //    MoveTo::create(distance / speed, WARRIOR_OFFSET(end_point.center_pos())),
    //    CallFunc::create([this]() {
    //            _warrior_node->getAnimation()->play("Sstand");
    //            _arrow_node->setVisible(false);
    //        })
    //    ));
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

    _floor = player_t::GetInstance()->get_floor();
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
    _warrior_node->setScaleX(player_t::GetInstance()->get_direction() == player_t::left ? 1.0f : -1.0f);
    auto warrior_pos = player_t::GetInstance()->get_pos().center_pos();
    _warrior_node->setPosition(WARRIOR_OFFSET(warrior_pos));
    addChild(_warrior_node);

    auto ui_node = CSLoader::createNode("game_layer.csb");
    addChild(ui_node);

    scheduleUpdate();
    player_t::GetInstance()->set_stair_change_call(bind(&MainScene::on_stair_change, this));
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

void MainScene::on_move(const pos_t& end)
{
    auto start = player_t::GetInstance()->get_pos();
    auto distance = abs(start.x - end.x) + abs(start.y - end.y);
    auto duration = distance / player_t::GetInstance()->get_speed();
    const string direction = end.y == start.y ? "S" : (end.y > start.y ? "B" : "F");
    const bool flip = end.x > start.x;
    _warrior_node->setPosition(WARRIOR_OFFSET(start.center_pos()));
    _warrior_node->stopAllActions();
    _warrior_node->getAnimation()->play(direction + "run");
    _warrior_node->setScaleX(flip ? -1.0f : 1.0f);
    _warrior_node->runAction(Sequence::createWithTwoActions(
        MoveTo::create(duration, WARRIOR_OFFSET(end.center_pos())),
        CallFunc::create([this, direction]() {
        _warrior_node->getAnimation()->play(direction + "stand");
    })
    ));
}

void MainScene::on_end()
{
    auto player = player_t::GetInstance();
    _arrow_node->setVisible(false);
    _warrior_node->setPosition(WARRIOR_OFFSET(player->get_pos().center_pos()));
    _warrior_node->stopAllActions();
    _warrior_node->getAnimation()->play(get_direction_str(player->get_direction()) + "stand");
    _warrior_node->setScaleX(player->get_direction() == player_t::right ? -1.0f : 1.0f);
}

void MainScene::on_stair_change()
{
    Director::getInstance()->replaceScene(MainScene::createScene());
}