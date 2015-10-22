#pragma once

#include "AStar.h"
#include "Floor.h"
#include "ModalDialog.h"

class WarriorNode;
class WarriorInfoPanel;
class FloorMapLayer : public cocos2d::Node
{
public:
    static cocos2d::Scene* scene(int floor);
    bool init(int floor);

public:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *e);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *e);

protected:
    struct npc_t;
    void confirm_attack(OKCancelDialog::RETURN_TYPE type, const Floor::npc_t& npc);
    void confirm_attack_impl(const Floor::npc_t& npc);
    void confirm_open(OKCancelDialog::RETURN_TYPE type, const Floor::npc_t& npc);
    void confirm_open_impl(const Floor::npc_t& npc);
    cocos2d::Value get_tile_prop(int32_t gid, const std::string& key);
    void step();
    void pick_up_item_impl(const Floor::npc_t& npc, const cocos2d::Vec2& target_pos, const std::function<void()>& callback);
    void pick_up_item(const Floor::npc_t& npc);
    bool interact_item(const Floor::npc_t& npc);   // 与npc交互，返回值为是否停下来交互，如开门，打斗，对话框提示等
    void stop_and_clear();

protected:
    int _floor;
    cocos2d::experimental::TMXTiledMap* _tiled_map;
    WarriorNode* _warrior;
    WarriorInfoPanel* _info_panel;
    Node* _arrow_node;
    Node* _road_node;
    std::vector<Floor::position_t> _paths;
    Floor::npc_t _stair_up;
    Floor::npc_t _stair_down;
};

//////////////////////////////////////////////////////////////////////////
class PromptLayer : public cocos2d::Node
{
public:
    static cocos2d::Scene* scene(int floor);
    bool init(int floor);

    virtual void onEnterTransitionDidFinish() override;

protected:
    int _floor;
};