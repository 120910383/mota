#pragma once

#include "cocos2d.h"
#include "AStar.h"

namespace spine { class SkeletonAnimation; }
class FloorMapLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* scene();
    virtual bool init() override;

    CREATE_FUNC(FloorMapLayer);

public:
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *e) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *e) override;

private:
    void step();

private:
    cocos2d::experimental::TMXTiledMap* _tiled_map;
    spine::SkeletonAnimation* _warrior;
    Node* _arrow_node;
    Node* _road_node;
    vector<node_t> _paths;
};