/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "tiles_res.h"

USING_NS_CC;

//using namespace cocostudio::timeline;

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

// on "init" you need to initialize your instance
bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _map_node = Node::create();
    _map_node->setContentSize(Size(750, 900));
    _map_node->setPosition(Point(-45, -60));
    addChild(_map_node);

    int32_t current_floor = 15;
    auto floor_info = tiles_res::GetInstance()->get_floor_info(current_floor);
    for (auto elem : floor_info->floors)
    {
        auto sprite = Sprite::create(elem.res);
        sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        sprite->setPosition(Point(elem.x, elem.y) * 75.0f);
        sprite->setFlippedX(elem.flip);
        _map_node->addChild(sprite, 0, elem.id);
    }
    for (auto elem : floor_info->blocks)
    {
        auto sprite = Sprite::create(elem.res);
        sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        sprite->setPosition(Point(elem.x, elem.y) * 75.0f);
        sprite->setFlippedX(elem.flip);
        _map_node->addChild(sprite, 1, elem.id);
    }
    for (auto elem : floor_info->npcs)
    {
        auto sprite = Sprite::create(elem.res);
        sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        sprite->setPosition(Point(elem.x, elem.y) * 75.0f);
        sprite->setFlippedX(elem.flip);
        _map_node->addChild(sprite, 2, elem.id);
    }

    auto ui_node = CSLoader::createNode("game_layer.csb");
    addChild(ui_node);

    return true;
}
