#include "FloorMapLayer.h"

USING_NS_CC;

Scene* FloorMapLayer::scene()
{
    auto ret_scene = Scene::create();
    FloorMapLayer* floor_layer = FloorMapLayer::create();
    ret_scene->addChild(floor_layer);
    return ret_scene;
}

bool FloorMapLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto map = cocos2d::experimental::TMXTiledMap::create("TileMaps/floor001.tmx");
    if (nullptr != map)
    {
        map->setPosition(Vec2(-50.0f, -50.0f));
        this->addChild(map);
    }

    return true;
}