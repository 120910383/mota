#pragma once

#include "cocos2d.h"
#include "Player.h"

namespace cocos2d { class LabelAtlas; }
class WarriorInfoPanel : public cocos2d::Node, public PlayerDelegate
{
public:
    CREATE_FUNC(WarriorInfoPanel);

    bool init();
    virtual void on_player_attr_changed() override;

public:
    cocos2d::LabelAtlas* _attack_num;
    cocos2d::LabelAtlas* _defend_num;
    cocos2d::LabelAtlas* _jb_num;
    cocos2d::LabelAtlas* _hun_num;
    cocos2d::LabelAtlas* _hp_num;
    cocos2d::LabelAtlas* _key_red_num;
    cocos2d::LabelAtlas* _key_blue_num;
    cocos2d::LabelAtlas* _key_yellow_num;
};