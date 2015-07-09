#pragma once

#include "cocos2d.h"

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
    void callback();

private:
    cocos2d::experimental::TMXTiledMap* _tiled_map;
    spine::SkeletonAnimation* _warrior;
    Node* _arrow_node;
    Node* _road_node;
};