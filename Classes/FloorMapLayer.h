#pragma once

#include "cocos2d.h"

class FloorMapLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* scene();
    virtual bool init() override;

    CREATE_FUNC(FloorMapLayer);
};