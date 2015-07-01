#include "FloorMapLayer.h"
#include "spine/spine-cocos2dx.h"

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
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    _tiled_map = cocos2d::experimental::TMXTiledMap::create("TileMaps/floor001.tmx");
    if (nullptr != _tiled_map)
    {
        _tiled_map->setPosition(Vec2(-50.0f, -50.0f));
        this->addChild(_tiled_map);
    }

    _warrior = spine::SkeletonAnimation::createWithFile("Spine/raptor.json", "Spine/raptor.atlas", 1.0f);
    if (nullptr != _warrior)
    {
        _warrior->setScale(0.1f);
        _warrior->setPosition(Vec2(40.0f, 40.0f));
        this->addChild(_warrior);
    }
    return true;
}

bool FloorMapLayer::onTouchBegan(Touch *touch, Event *e)
{
    return true;
}

void FloorMapLayer::onTouchEnded(Touch *touch, Event *e)
{
    auto wall_layer = _tiled_map->getLayer("wall");
    auto wall_layer_size = wall_layer->getLayerSize();
    auto pos = touch->getLocation();
    auto tiles = wall_layer->getTiles();
    std::vector<std::vector<uint32_t>> tiles_vec;
    for (uint32_t i = 0; i < (uint32_t)(wall_layer_size.height); ++i)
    {
        std::vector<uint32_t> tiles_hor;
        for (uint32_t j = 0; j < (uint32_t)(wall_layer_size.width); ++j)
        {
            tiles_hor.push_back(tiles[i * (uint32_t)(wall_layer_size.width) + j]);
        }
        tiles_vec.push_back(tiles_hor);
    }

    if (pos.x > _warrior->getPositionX()) {
        _warrior->setScaleX(0.1f);
    }
    else {
        _warrior->setScaleX(-0.1f);
    }
    _warrior->setTimeScale(2.0f);
    _warrior->setAnimation(0, "walk", true);
    auto action = Sequence::create(
        MoveTo::create(_warrior->getPosition().distance(pos) / 200.0f, pos),
        CallFunc::create([&](){ _warrior->setAnimation(0, "gungrab", false); }), nullptr);
    _warrior->stopAllActions();
    _warrior->runAction(action);
}
