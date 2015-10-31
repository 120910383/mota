#pragma once

#include "Player.h"

namespace cocos2d { class LabelAtlas; }
class WarriorInfoPanel : public cocos2d::Node, public PlayerDelegate
{
public:
    CREATE_FUNC(WarriorInfoPanel);

    bool init();
    virtual void on_player_attr_changed() override;

    enum node_type { attack, defend, jb, hun, hp, key_red, key_blue, key_yellow };
    cocos2d::Vec2 get_node_position_in_world(node_type type);
    void update_data(bool anim);

private:
    cocos2d::LabelAtlas* _attack_num;
    cocos2d::LabelAtlas* _defend_num;
    cocos2d::LabelAtlas* _jb_num;
    cocos2d::LabelAtlas* _hun_num;
    cocos2d::LabelAtlas* _hp_num;
    cocos2d::LabelAtlas* _key_red_num;
    cocos2d::LabelAtlas* _key_blue_num;
    cocos2d::LabelAtlas* _key_yellow_num;
    cocos2d::ProgressTimer* _progress;
    int32_t _max_hp;
};